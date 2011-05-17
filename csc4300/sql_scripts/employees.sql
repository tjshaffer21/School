CREATE TABLE employees (
    eid INTEGER PRIMARY KEY AUTO_INCREMENT,
    user_name VARCHAR(255) UNIQUE NOT NULL,
    began_emp CHAR(10) NOT NULL,
    salary  INTEGER NOT NULL DEFAULT 40000,
    FOREIGN KEY(user_name) REFERENCES users (user_name)
);
