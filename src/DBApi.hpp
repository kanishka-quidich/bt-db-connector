#pragma once
#include <string>
#include <cassandra.h>
#include <vector> // Include vector for the updated return type

namespace quidich {

class CameraTable {
    CassSession* session;
public:
    CameraTable(CassSession* sess);
    void insert(const std::string& camera, int ssn, const std::string& ip, const std::string& system);
    std::string get(const std::string& camera);
};

class SystemTable {
    CassSession* session;
public:
    SystemTable(CassSession* sess);
    void insert(const std::string& system, const std::string& ip);
    std::string get(const std::string& system);
};

class BallTable {
    CassSession* session;
public:
    BallTable(CassSession* sess);
    void insert(const std::string& ballId, const std::string& timestamp, 
                const std::string& disk, const std::string& camera);
    std::vector<std::string> get(const std::string& ballId); // changed return type
};

} // namespace quidich
