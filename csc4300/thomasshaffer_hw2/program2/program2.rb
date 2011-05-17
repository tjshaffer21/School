#!/usr/bin/ruby
require 'rubygems'
require "mysql"

begin
    if ARGV.size < 4
        print "ruby program2.rb [host] [user] [database] [pass]\n"
    else
        dbh = Mysql.real_connect( ARGV[0], ARGV[1], ARGV[3], ARGV[2] )
        dbh.query( "INSERT INTO Students (TNumber, FirstName, LastName, DateOfBirth) VALUES ( 00003256, 'John', 'Doe', DATE('1970-07-15') )" )
        dbh.query( "INSERT INTO Students (TNumber, FirstName, LastName, DateOfBirth) VALUES ( 00001423, 'Mary', 'Smith', DATE('1992-01-01') )" )
        dbh.query( "INSERT INTO Students (TNumber, FirstName, LastName, DateOfBirth) VALUES ( 00001234, 'Joe', 'Smith', DATE('1950-08-12') )" )
        dbh.query( "INSERT INTO Students (TNumber, FirstName, LastName, DateOfBirth) VALUES ( 00015366, 'William', 'Williamson', DATE('1991-05-23') )" )
        dbh.query( "INSERT INTO Students (TNumber, FirstName, LastName, DateOfBirth) VALUES ( 00012345, 'Katie', 'Did', DATE('1992-09-27') )" )
    end
ensure
    dbh.close
end
