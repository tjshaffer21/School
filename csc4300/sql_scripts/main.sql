CREATE TABLE users (
    id           INTEGER PRIMARY KEY AUTO_INCREMENT,
    user_name    VARCHAR(255) NOT NULL,
    pass         VARCHAR(32) NOT NULL,
    priv         CHAR(1) NOT NULL,
    fname        VARCHAR(255) NOT NULL,
    mname        VARCHAR(255) NOT NULL,
    lname        VARCHAR(255) NOT NULL,
    email        VARCHAR(255) NOT NULL,
    address      VARCHAR(255) NOT NULL,
    phone        CHAR(10) NOT NULL
);

CREATE TABLE employees (
    eid INTEGER PRIMARY KEY AUTO_INCREMENT,
    user_name VARCHAR(255) UNIQUE NOT NULL,
    began_emp CHAR(10) NOT NULL,
    salary  INTEGER NOT NULL DEFAULT 40000,
    FOREIGN KEY(user_name) REFERENCES users (user_name)
);

INSERT INTO users ( user_name, pass, priv, fname, mname, lname, email,address,phone ) VALUES ( 'Admin', 'e3afed0047b08059d0fada10f400c1e5', '0', 'Admin', 'Admin', 'Admin', 'Admin@library.net', '23 Admin Aminate AN, 12345', '1234521234' );
INSERT INTO employees ( user_name, began_emp, salary ) VALUES ( 'Admin', '2010-04-25', 40000 );

CREATE TABLE author (
    aid     INTEGER PRIMARY KEY AUTO_INCREMENT,
    a_last  VARCHAR(255) NOT NULL,
    a_mid   VARCHAR(255),
    a_first VARCHAR(255) NOT NULL,
    bio     TEXT
);

CREATE TABLE books (
    isbn    CHAR(13) PRIMARY KEY,
    call_num   CHAR(25) UNIQUE,
    title   VARCHAR(255) NOT NULL,
    genre   VARCHAR(255) NOT NULL,
    description TEXT NOT NULL,
    aid     INTEGER,
    avail   INTEGER NOT NULL,
    total   INTEGER NOT NULL,
    FOREIGN KEY(aid) REFERENCES author (aid)
);

CREATE TABLE checked_out (
    id INTEGER PRIMARY KEY AUTO_INCREMENT,
    user_name   VARCHAR(255)    NOT NULL,
    call_num    CHAR(25)        NOT NULL,
    FOREIGN KEY (user_name) REFERENCES users (user_name),
    FOREIGN KEY (call_num) REFERENCES books (call_num)
);

CREATE TABLE logs (
    log_id INTEGER PRIMARY KEY AUTO_INCREMENT,
    time TIMESTAMP(8),
    message VARCHAR(255)
);

CREATE VIEW out_of_stock AS SELECT call_num,title FROM books WHERE avail <= 0 ORDER BY call_num DESC;

DELIMITER //
CREATE PROCEDURE checkout_proc (in cnum char(25), in u_name varchar(255))
    begin
        UPDATE books SET avail = avail-1 WHERE call_num=cnum;
        INSERT INTO checked_out (user_name,call_num) VALUES (u_name,cnum);
    end //
DELIMITER ;

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
DELIMITER ;

INSERT INTO author (aid,a_last,a_first,bio) VALUES (1,'Dostoyevsky', 'Fyodor', 'Fyodor Dostoyevsky...' );
INSERT INTO author (aid,a_last,a_first,bio) VALUES (2,'Tolstoy', 'Leo', 'Leo Tolstoy...');
INSERT INTO author (aid,a_last,a_first,bio) VALUES (3,'Tolkien', 'John', 'J.R.R. Tolkien...');

INSERT INTO books( isbn, call_num, title, genre, description, aid, avail, total ) VALUES ( '9781770451247', 'DOS103', 'Crime and Punishment', 'Literature', 'Crime and Punishment...', 1, 0, 30 );
INSERT INTO books( isbn, call_num, title, genre, description, aid, avail, total ) VALUES ( '9780374528379', 'DOS104', 'The Brothers Karamazov', 'Literature','The Brothers Karamazov...', 1, 15, 15 );
INSERT INTO books( isbn, call_num, title, genre, description, aid, avail, total ) VALUES ( '9781400079988', 'TOL1234', 'War and Peace', 'Classic', 'War and Peace...', 2, 5, 5  );

INSERT INTO users ( user_name, pass, priv, fname, mname, lname, email,address,phone ) VALUES ( 'tjshaffer21', '29df23feb31ac12ba42ea15364df5304', 2, 'Thomas', 'Jay', 'Shaffer', 'tjshaffer21@tntech.edu', '59 Lone Oak Dr, Jackson TN 38305', '1231231234' );


