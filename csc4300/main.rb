require 'mysql'
require 'digest/md5'
require 'members.rb'
require 'employees.rb'


#
#   Systems main screen
#
def welcome_screen
    while 1
        puts "1) Login\n2) Run simulation\n3) Create Member Account\n4) Exit"
        print "Choice: "
        choice = STDIN.gets

        case choice.to_i
            when 1
                r = login
                if r == 2
                    member_screen
                else
                    employee_screen
                end
            when 2
                begin_simulation
            when 3
                join
            when 4 
                exit
            else
                puts "Invalid choice!\n"
        end
    end
end

###############################################################################
#                                 Employee Functions                           #
###############################################################################
#
#   Main employee screen
#
def employee_screen
   print "\nWelcome, #{$user.get_user_name}\n\n"

   while 1
        puts "1) Add book\n2) Remove book\n3) View out of stock\n4) View catalog\n5) Add employee\n6) Remove employee\n7) Logout"
        print "Choice: "
        choice = STDIN.gets
        
        case choice.to_i
            when 1
                $user.add_book
            when 2
                $user.remove_book
            when 3
                $user.view_out_of_stock
            when 4
                $user.view_catalog
            when 5
                $user.add_employee
            when 6
                $user.remove_employee
            when 7
                logout
            else
                puts "Invalid choice"
        end
   end
end

###############################################################################
#                                Member Functions                              #
###############################################################################
#
#   join
#   Allows for a person to join the system.
#   Preconditions:
#       None.
#   Postconditions:
#       User table is updated.
#
def join
    print "User Name: "
    uname = STDIN.gets.chomp
       
    print "Password: "
    pass = STDIN.gets.chomp
    pass = Digest::MD5.hexdigest(pass)

    print "First Name: "
    fname = STDIN.gets.chomp

    print "Middle Name: "
    mname = STDIN.gets.chomp

    print "Last Name: "
    lname = STDIN.gets.chomp

    print "E-mail:"
    email = STDIN.gets.chomp

    print "Address: "
    adrss = STDIN.gets.chomp

    print "Phone Number: "
    phone = STDIN.gets.chomp

    # Should have more indepth checking for multiple accounts.
    rs = $con.query( "SELECT user_name FROM users WHERE user_name='" + uname + "'" )
    
    if rs.num_rows == 0
        begin
            rs = $con.query( "INSERT INTO users (user_name, pass, priv, fname, mname, lname, email, address, phone) VALUES ('" + uname + "','" + pass + "', 2, '" + fname + "','" + mname + "','" + lname + "','" + email + "','" + adrss + "','" + phone + "')" )
            $con.commit()
        rescue
            puts "Transaction failed"
            $con.rescue()
        end
    else
        rs.free
        print "Username has been taken.\n\n"

        join
    end
end

#
#   Main member screen
#
def member_screen
    print "\nWelcome, #{$user.get_user_name}\n\n"
    while 1
        puts "1) View profile\n2) Search book\n3) Logout"
        print "Choice: "
        choice = STDIN.gets
        
        case choice.to_i
            when 1
                $user.view_profile
            when 2
                search_book
            when 3
                logout
            else
                puts "Invalid choice"
        end
    end
end

#
#   Main menu for book searching
#
def search_book
    while 1
        puts "Search for books by\n1) Author's last name\n2) Book title\n3) Exit"
        print "Choice: "
        choice = STDIN.gets

        case choice.to_i
            when 1
                print "Last Name: "
                last_name = STDIN.gets.chomp

                search_last_name last_name
                break
           when 2
               print "Title: "
               title = STDIN.gets.chomp

                search_title title
                break
            when 3
                break
            else
                puts "Invalid choice!"
        end
    end
end

def search_last_name last_name
    rs0 = $con.query( "SELECT aid FROM author WHERE a_last='#{last_name.capitalize}'" )

    if rs0.num_rows == 0
        print "Author not found\n\n"
        return
    end

    rs0.each do |s|
        rs = $con.query( "SELECT * FROM books WHERE aid=#{s[0]}" )

        print "\n"
        if rs.num_rows != 0
            rs.each_hash do |row|
                puts "Call #: #{row['call_num']}\tTitle: #{row['title']}"
            end
            rs.free
        else
            print "No books where found!\n\n"
        end
    end
end

def search_title title
    rs = $con.query( "SELECT * FROM books WHERE title='" + title + "'" )
    
    print "\n"
    if rs.num_rows != 0
        rs.each_hash do |row|
            rs0 = $con.query( "SELECT a_last,a_first FROM author WHERE aid=#{row['aid']}" )

            if rs0.num_rows == 0
                print "Book not found\n\n"
                return
            end

            rs0.each_hash do |s|
                puts "Call #: #{row['call_num']}\tTitle: #{row['title']}\tAuthor: #{s['a_last']}, #{s['a_first']} #{s['a_mid']}\t#{row['avail']}/#{row['total']}"
            end
        end
        rs.free
    else
        print "No books where found!\n\n"
    end
end

###############################################################################
#                                   General Functions                          #
###############################################################################
def login
    print "\nUsername: "
    uname = STDIN.gets.chomp

    print "Password: "
    pass = STDIN.gets.chomp
    pass = Digest::MD5.hexdigest(pass)   # Convert to MD5

    rs = $con.query( "SELECT user_name,priv FROM users WHERE user_name='" + uname + "' AND pass='" + pass + "'" )

    if rs.num_rows == 0
        print "Invalid username or password\n\n"
        rs.free
        login
    else
        # Better method? Should only exist one result.
        rs.each do |row|
            if row[1].to_i >= 2
                $user = Member.new(row[0])
                return 2
            else
                $user = Employee.new(row[0],row[1])
                return 1
            end
        end

        rs.free
    end
end

def logout
    # How do you destroy ruby object?
    print "\nGoodbye\n\n"
    welcome_screen
end

###############################################################################
#                           Simulation Functions                              #
###############################################################################
def begin_simulation
    puts "\nPlease login as a member."
    login

    while 1
        puts "\n1) Search for book\n2) Add book to cart\n3) View Cart\n4) Remove book from Cart\n5) Checkout\n6) Return\n7) Quit"
        print "Choice: "
        choice = STDIN.gets
        
        print "\n"
        case choice.to_i
            when 1
                search_book
            when 2
                $user.add_to_cart
            when 3
                $user.view_cart
            when 4
                $user.remove_from_cart
            when 5
                $user.checkout
            when 6
                return_books
            when 7 
                print "Ending simulation\n\n"
                break
        end
    end
end

def return_books
    puts "Returning books"

    $con.query("CALL return_proc(\'#{$user.get_user_name}\')")

    print "Thank you\n\n"
end
###############################################################################
#                           End Simulation Functions                          #
###############################################################################

begin
    $con = Mysql.new( ARGV[0], ARGV[1], ARGV[3], ARGV[2] );
    $user = nil

    puts "Welcome to the Library Application Demo.\n"
    welcome_screen
rescue Mysql::Error => e
    puts "Error code: #{e.errno}"
    puts "Error message: #{e.error}"
    puts "Error SQLSTATE: #{e.sqlstate}" if e.respond_to?("sqlstate")
ensure
    $con.close if $con
end
