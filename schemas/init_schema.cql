-- Create keyspace
CREATE KEYSPACE IF NOT EXISTS quidich
WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 1};

-- Use the keyspace
USE quidich;

-- Table 1: System table
CREATE TABLE IF NOT EXISTS system (
    System text PRIMARY KEY,
    System_IP text
);

-- Table 2: Camera table
CREATE TABLE IF NOT EXISTS camera (
    Camera text PRIMARY KEY,
    SSN int,
    Camera_IP text,
    System text
);

-- Table 3: Ball table
CREATE TABLE IF NOT EXISTS ball (
    BallID uuid PRIMARY KEY,
    Timestamp timestamp,
    Disk text,
    Camera text
);
