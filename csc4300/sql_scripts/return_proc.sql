DELIMITER //
CREATE PROCEDURE return_proc (in user_name varchar(255))
    BEGIN
        DECLARE x varchar(255);
        DECLARE done BOOLEAN DEFAULT 0;
        DECLARE num CHAR(25);
        DECLARE c CURSOR FOR SELECT call_num FROM checked_out WHERE user_name = x;
        DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done=1;
        OPEN c;

        SET x = user_name;
        REPEAT
            FETCH c INTO num;
            UPDATE books SET avail=avail+1 WHERE call_num = num;
            DELETE FROM checked_out WHERE user_name = x;
        UNTIL done END REPEAT;
        CLOSE c;
    END //
DELIMITER ;
