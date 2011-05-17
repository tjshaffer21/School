CREATE TABLE author (
    a_last  VARCHAR(255),
    a_mid   VARCHAR(255),
    a_first VARCHAR(255),
    bio     TEXT,
    PRIMARY KEY( a_last, a_first, a_mid )
);
