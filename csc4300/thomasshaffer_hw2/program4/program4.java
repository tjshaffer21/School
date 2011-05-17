import java.sql.*;

public class program4 {
    public static void main(String args[]) {
        if ( args.length < 4 ) {
            System.out.println( "java program4 host user db password" );
        } else {
            try {
                Statement stmt;

                Class.forName("com.mysql.jdbc.Driver");

                String url = "jdbc:mysql://" + args[0] + ":3306/" + args[2] ;
                Connection con = DriverManager.getConnection( url, args[1], args[3] );
                stmt = con.createStatement();

                stmt.executeUpdate("CREATE TABLE Grades (TNumber char(8), CourseID char(10), Grade char(1)) ENGINE = innodb");
                stmt.executeUpdate("INSERT INTO Grades (TNumber, CourseID, Grade) VALUES (00003256, 'CSC4300', 'A')" );
                stmt.executeUpdate("INSERT INTO Grades (TNumber, CourseID, Grade) VALUES (00012345, 'MAT1910', 'B')" );
                stmt.executeUpdate("INSERT INTO Grades (TNumber, CourseID, Grade) VALUES (00001423, 'BIOL1010', 'D')" );
                stmt.executeUpdate("INSERT INTO Grades (TNumber, CourseID, Grade) VALUES (00015366, 'CSC2110', 'C')" );
                stmt.executeUpdate("INSERT INTO Grades (TNumber, CourseID, Grade) VALUES (00003256, 'CSC4100', 'A')" );
                con.close();
            } catch (Exception ex) {
                System.out.println( ex.getMessage());
            }
        }
    }
}
