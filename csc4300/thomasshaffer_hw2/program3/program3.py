#!/usr/bin/python
import sys
import MySQLdb

if (len(sys.argv) < 4 ):
    print "python program3.py host user db password"
else:
    try:
        conn = MySQLdb.connect( host=sys.argv[1] , user=sys.argv[2] , passwd=sys.argv[4], db=sys.argv[3] )
    except MySQLdb.Error, e:
        print "Error %d: %s" % (e.args[0], e.args[1])
        sys.exit(1)

    cursor = conn.cursor()
    cursor.execute( "SELECT * FROM Students WHERE Students.LastName = 'Smith'" )

    while(1):
        row = cursor.fetchone()
        if row == None:
            break
        print "%s, %s, %s, %s" % (row[0], row[1], row[2], row[3])
