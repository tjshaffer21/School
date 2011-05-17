CREATE TABLE book (
    isbn    CHAR(13) PRIMARY KEY,
    call#   CHAR(25) UNIQUE,
    title   VARCHAR(255) NOT NULL,
    genre   VARCHAR(255) NOT NULL,
    description TEXT NOT NULL DEFAULT "None.",
    a_last  VARCHAR(255) NOT NULL,
    a_mid   VARCHAR(255) NOT NULL,
    a_first VARCHAR(255) NOT NULL,
    avail   INTEGER NOT NULL,
    total   INTEGER NOT NULL,
    FOREIGN KEY(a_last, a_first, a_mid) REFERENCES author
);
