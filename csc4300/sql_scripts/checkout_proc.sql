DELIMITER //
CREATE PROCEDURE checkout_proc (in cnum char(25), in u_name varchar(255))
    begin
        UPDATE books SET avail = avail-1 WHERE call_num=cnum;
        INSERT INTO checked_out (user_name,call_num) VALUES (u_name,cnum);
    end //
DELIMITER ;
