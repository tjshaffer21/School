CREATE TABLE checked_out (
    id INTEGER PRIMARY KEY AUTO_INCREMENT,
    user_name   VARCHAR(255)    NOT NULL,
    call_num    CHAR(25)        NOT NULL,
    FOREIGN KEY (user_name) REFERENCES users (user_name),
    FOREIGN KEY (call_num) REFERENCES books (call_num)
);
