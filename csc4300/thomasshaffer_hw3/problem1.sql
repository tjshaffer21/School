CREATE TABLE grade_distribution (grade CHAR(1), count INTEGER ) ENGINE=innodb;
INSERT INTO grade_distribution (grade, count) VALUES ('A', 0);
INSERT INTO grade_distribution (grade, count) VALUES ('B', 0);
INSERT INTO grade_distribution (grade, count) VALUES ('C', 0);
INSERT INTO grade_distribution (grade, count) VALUES ('D', 0);
INSERT INTO grade_distribution (grade, count) VALUES ('F', 0);

DELIMITER |
CREATE TRIGGER tr_grades_insert AFTER INSERT ON Grades
FOR EACH ROW
BEGIN
    UPDATE grade_distribution SET count = count+1 WHERE grade=NEW.grade;
END |
DELIMITER ;

TRUNCATE TABLE Grades;
INSERT INTO Grades (TNumber, CourseID, Grade) VALUES (00003256, 'CSC4300',  'A' );
INSERT INTO Grades (TNumber, CourseID, Grade) VALUES (00012345, 'MAT1910',  'B' );
INSERT INTO Grades (TNumber, CourseID, Grade) VALUES (00001423, 'BIOL1010', 'D' );
INSERT INTO Grades (TNumber, CourseID, Grade) VALUES (00015366, 'CSC2110',  'C' );
INSERT INTO Grades (TNumber, CourseID, Grade) VALUES (00003256, 'CSC4100',  'A' );
SELECT * FROM grade_distribution;
