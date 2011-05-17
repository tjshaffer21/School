SELECT Students.TNumber,FirstName,LastName,CourseID,Grade FROM Students LEFT JOIN Grades ON Students.TNumber = Grades.TNumber;
