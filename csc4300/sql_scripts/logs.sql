CREATE TABLE logs ( 
    log_id INTEGER PRIMARY KEY AUTO_INCREMENT,
    time TIMESTAMP(8),
    message VARCHAR(255)
);

/* Should not be used in live environment because it does not
   tell who triggered the events
*/
DELIMITER |
CREATE TRIGGER tr_delete_employee BEFORE DELETE ON employees
FOR EACH ROW
BEGIN
    INSERT INTO logs (message) VALUES ("OLD.fname OLD.lname deleted from the system.");
END |

CREATE TRIGGER tr_insert_employee AFTER INSERT ON employees
FOR EACH ROW
BEGIN
    INSERT INTO logs (message) VALUES ("NEW.fname NEW.lname inserted into the system.");
END |
