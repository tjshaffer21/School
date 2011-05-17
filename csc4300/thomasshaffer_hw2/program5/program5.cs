using System;
using System.Data;
using MySql.Data.MySqlClient;

public class program5 {
    public static void Main( string[] args) {
        if ( args.Length < 4 ) {
            Console.WriteLine( "mono program5.exe host user db password" );
        } else {
            string connectionString = "Server=" + args[0] + ";" + "Database=" + args[2] + ";" + "User ID=" + args[1] + ";" + "Password=" + args[3] + ";" + "Pooling=false";
            
            IDbConnection dbcon = new MySqlConnection(connectionString);
            //MySqlConnection dbcon = new MySqlConnection(connectionString);
            
            dbcon.Open();
            
            IDbCommand dbcmd = dbcon.CreateCommand();
            
            string sql = "SELECT Students.FirstName, Students.LastName, Grades.CourseID, Grades.Grade FROM Students, Grades WHERE Grades.Grade <= 'C' AND Students.TNumber = Grades.TNumber";
            dbcmd.CommandText = sql;
            
            IDataReader reader = dbcmd.ExecuteReader();
            while( reader.Read()) {
                string FirstName = (string) reader["FirstName"];
                string LastName = (string) reader["LastName"];
                string CourseID = (string) reader["CourseID"];
                string Grade = (string) reader["Grade"];
    
                Console.WriteLine( FirstName + " " + LastName + "\n\t\t    " + CourseID + "\t" + Grade );
            }

            reader.Close();
            dbcmd.Dispose();
            dbcon.Close();
        }
    }
}
