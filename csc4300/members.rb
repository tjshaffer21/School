class Member
    attr_accessor :user_name, :cart

    def initialize( user_name )
        @user_name = user_name
        @cart = Array.new
    end

    def get_user_name
        return @user_name
    end

    def get_cart
        return @cart
    end

    #
    #   add_to_cart
    #   Adds a book to the user's cart
    #   Preconditions:
    #       None
    #   Postconditions:
    #       If the book is available, add to the cart.
    #
    def add_to_cart
        print "Call #: "
        call = STDIN.gets.chomp
    
        rs = $con.query( "SELECT call_num,title,aid FROM books WHERE call_num='" + call + "' AND avail > 0" )

        if rs.num_rows == 0
            print "Unable to find book.\n\n"
        else
            addtocart = true
            rs.each do |r|
                @cart.each do |s|
                    if s[0].eql? r[0]
                        addtocart = false
                    end
                end

                if addtocart
                    @cart << r
                else
                    print "Copy already exists in the cart.\n\n"
                end
            end
            rs.free
        end
    end

    def view_cart
        if @cart.empty?
            print "\nCart: None\n\n"
        else
            print "\n"
            @cart.each do |b|
                rs = $con.query( "SELECT a_last,a_first FROM author WHERE aid=#{b[2]}" )
                if rs.num_rows == 0
                    print "Book not available\n\n"
                    return
                end

                rs.each do |s|
                    puts "Call #: #{b[0]}\tTitle: #{b[1]} Author: #{s[0]}, #{s[1]}"
                end
            end
        end
    end

    #
    #   remove_from_cart
    #   Removes specified book from the cart.
    #   Preconditions:
    #       None.
    #   Postconditions:
    #       If book is found, it is removed from array
    #
    def remove_from_cart
        print "Call #: "
        call = STDIN.gets.chomp

        @cart.each do |c|
            if c[0].eql? call
                @cart.delete(c)
            end
        end
    end
  
    def checkout
        if @cart.empty?
            puts "Cart is empty"
        else
            avail = 0
            @cart.each do |s|
                begin
                    $con.query("CALL checkout_proc('" + s[0] + "','" + @user_name + "')")
                    $con.commit
                rescue
                    puts "Transaction failed"
                    $con.rollback
                end

                @cart.delete(s)
            end
        end
    end

    def view_profile 
        rs = $con.query("SELECT * FROM users WHERE user_name=\'#{@user_name}\'")
        rs.each_hash do |row|
            puts "   User Name: #{row["user_name"]}"
            puts "  First Name: #{row["fname"]}"
            puts " Middle Name: #{row["mname"]}"
            puts "   Last Name: #{row["lname"]}"
            puts "      E-mail: #{row["email"]}"
            puts "     Address: #{row["address"]}"
            puts "Phone Number: #{row["phone"]}"
        end
        rs.free

        # Should be a stored procedure
        rs = $con.query( "SELECT call_num FROM checked_out WHERE user_name=\'#{@user_name}\'")
        print "\nChecked out books:\n\n"
        if rs.num_rows == 0
            print "No books checked out.\n\n"
        else
            rs.each do |r|
                rs2 = $con.query( "SELECT title,a_last FROM books WHERE call_num='" + r[0] + "'")
                rs2.each do |r2|
                    puts "\tTitle: #{r2[0]} Author: #{r2[0]}"
                end
            end
        end

        print "\n"
    end
end
