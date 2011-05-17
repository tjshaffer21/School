#! /usr/bin/tclsh
lappend auto_path /Library/Tcl/lib/mysqltcl-3.05
if { $argc < 4 } { 
    puts "tclsh $argv0 hostname user database_name database_password"
} else {
    if {[catch {
        package require mysqltcl
        
        set m [mysqlconnect -host [lindex $argv 0] -user [lindex $argv 1] -password [lindex $argv 3]]
        mysqluse $m [lindex $argv 2]
        
        mysqlexec $m "CREATE TABLE Students (TNumber char(8) PRIMARY KEY, FirstName varchar(20), LastName varchar(20), DateOfBirth date) ENGINE = innodb"
        
        mysqlclose $m
    } res]} { 
        puts $res
    }
}
