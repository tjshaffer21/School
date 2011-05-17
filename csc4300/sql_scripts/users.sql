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
