class Employee
    attr_accessor :user_name, :priv

    def initialize( user_name, priv )
        @user_name = user_name
        @priv = priv
    end

    def get_user_name
        return @user_name
    end

    def get_priv_level
        return @priv
    end

    def add_book
        print "ISBN: "
        isbn = STDIN.gets.chomp
        
        isbn = isbn.to_i
        while isbn <= 0
            print "ISBN: "
            isbn = STDIN.gets.chomp

            isbn = isbn.to_i
        end

        print "Call #: "
        call = STDIN.gets.chomp

        print "Title: "
        title = STDIN.gets.chomp

        print "Genre: "
        genre = STDIN.gets.chomp

        print "Description: "
        desc = STDIN.gets.chomp

        print "Last Name: "
        lname = STDIN.gets.chomp

        print "Middle Name: "
        mname = STDIN.gets.chomp

        print "First Name: "
        fname = STDIN.gets.chomp

        total = -1
        while total <= 0
            print "Total: "
            total = STDIN.gets.chomp

            total = total.to_i
        end

        rs = $con.query( "SELECT * FROM author WHERE a_last='#{lname}' AND a_mid='#{mname}'AND a_first='#{fname}'" )

        if rs.num_rows == 0
            a = add_author fname, mname, lname
            $con.query( "INSERT INTO books (isbn,call_num,title,genre,description,aid,avail,total) VALUES( '#{isbn}', '#{call}', '#{title}', '#{genre}', '#{desc}', #{a.to_i}, #{total.to_i}, #{total.to_i})")
        else
            rs = $con.query( "SELECT aid FROM author WHERE a_last='#{lname}' AND a_mid='#{mname}' AND a_first='#{fname}'" )

            a = -1
            rs.each do |s|
                a = s[0]
            end
            $con.query( "INSERT INTO books (isbn,call_num,title,genre,description,aid,avail,total) VALUES( '#{isbn}', '#{call}', '#{title}', '#{genre}', '#{desc}', #{a.to_i}, #{total}, #{total})")
        end
    end

    def remove_book
        while 1
            puts "1) Call #\n2) Title\n3) Quit"
            print "Choice: "
            choice = STDIN.gets

            case choice.to_i
                when 1
                    print "Call #: "
                    call = STDIN.gets.chomp

                    $con.query( "DELETE FROM books WHERE call_num='#{call}'" )
                when 2
                    print "Title: "
                    title = STDIN.gets.chomp

                    $con.query( "DELETE FROM books WHERE title='#{title}'")
                when 3
                    break
                else
                    print "Invalid choice\n\n"
            end
        end
    end

    def add_author fname, mname, lname
        puts "Adding author..."
        print "Bio: "
        bio = STDIN.gets.chomp

        begin
            $con.query( "INSERT INTO author (a_last,a_mid,a_first,bio) VALUES ('#{lname}', '#{mname}', '#{fname}', '#{bio}')")
            $con.commit()
        rescue
            puts "Transaction failed"
            $con.rollback()
        end

        rs = $con.query( "SELECT aid FROM author WHERE a_last='#{lname}' AND a_mid='#{mname}' AND a_first='#{fname}'" )

        a = -1
        rs.each do |s|
            a = s[0]
        end

        print "\n\n"
        return a
    end

    def add_employee
        if @priv.to_i == 0
            print "\n"
            print "Username: "
            uname = STDIN.gets.chomp

            print "Default Password: "
            dpass = STDIN.gets.chomp
            dpass = Digest::MD5.hexdigest(dpass)

            print "Priviledge (0 - Admin; 1 - Librarian): "
            priv = STDIN.gets.chomp

            print "First Name: "
            fname = STDIN.gets.chomp

            print "Middle Name: "
            mname = STDIN.gets.chomp

            print "Last Name: "
            lname = STDIN.gets.chomp

            print "E-mail: "
            email = STDIN.gets.chomp

            print "Address: "
            addrs = STDIN.gets.chomp

            print "Phone #: "
            phone = STDIN.gets.chomp

            print "Began employment (YYYY-MM-DD): "
            bemp = STDIN.gets.chomp

            rs = $con.query( "SELECT user_name FROM users WHERE user_name='#{uname}'" )

            if rs.num_rows == 0
                begin
                    $con.query( "INSERT INTO users (user_name, pass, priv, fname, mname, lname, email, address, phone) VALUES ('#{uname}', '#{dpass}', '#{priv}','#{fname}','#{mname}','#{lname}', '#{email}', '#{addrs}', '#{phone}')")
                    $con.query( "INSERT INTO employees (user_name, began_emp) VALUES ('#{uname}','#{bemp}')")
                    $con.commit
                rescue
                    puts "Transaction failed."
                    $con.rollback
                end
            else 
                print "Username taken\n\n"
            end
        else
            print "\nAdmin. function\n\n"
        end
    end

    def remove_employee
        print "Username: "
        uname = STDIN.gets.chomp

        begin
            $con.query( "DELETE FROM employees WHERE user_name='#{uname}'" )
            $con.query( "UPDATE users SET priv=2 WHERE user_name='#{uname}'")
            $con.commit()
        rescue
            puts "Transaction failed"
            $con.rollback()
        end
    end

    def view_out_of_stock
        rs = $con.query("SELECT * FROM out_of_stock" )

        if rs.num_rows == 0
            print "No books out of stock\n\n"
        else
            rs.each do |r|
                puts "Call Num: #{r[0]}\tTitle: #{r[1]}"
            end
            print "\n"
        end
    end

    def view_catalog
        rs = $con.query( "SELECT books.call_num,author.a_last,author.a_first,books.total FROM author LEFT JOIN books ON author.aid = books.aid GROUP BY books.call_num" )

        if rs.num_rows == 0
            print "Catalog not available\n\n"
        else
            rs.each do |r|
                puts "Call #: #{r[0]}\tAuthor: #{r[1]} #{r[2]}\tTotal: #{r[3]}"
            end
        end
    end
end
