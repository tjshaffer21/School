/* CHECK Constraint is parsed by MySQL; however, it is ignored by all storage engines. */
ALTER TABLE Grades ADD CONSTRAINT CHECK(Grade in ('A', 'B', 'C', 'D', 'F'));
INSERT INTO Grades (TNumber, CourseID, Grade) VALUES( 00003256, 'CSC2110', 'E' );
SELECT DISTINCT 'FAILED' FROM Grades where Grade <> 'A' AND Grade <> 'B' AND Grade <> 'C' AND Grade <> 'D' AND Grade <> 'F';
