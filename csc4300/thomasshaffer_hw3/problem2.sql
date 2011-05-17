DELIMITER |
CREATE TRIGGER tr_grades_update BEFORE UPDATE ON Grades
FOR EACH ROW
BEGIN
    UPDATE grade_distribution SET count = count-1 WHERE grade=OLD.grade;
    UPDATE grade_distribution SET count = count+1 WHERE grade=NEW.grade;
END |

CREATE TRIGGER tr_grades_delete AFTER DELETE ON Grades 
FOR EACH ROW
BEGIN
    UPDATE grade_distribution SET count = count-1 WHERE grade=OLD.grade;
END |

UPDATE Grades SET grade='A' WHERE TNumber=00012345 AND CourseID='MAT1910';
SELECT * FROM grade_distribution;
