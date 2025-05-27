#include "DBApi.hpp"
#include <iostream>

namespace quidich {

CameraTable::CameraTable(CassSession* sess) : session(sess) {}

    // Insert a camera row
    void CameraTable::insert(const std::string& camera, int ssn, const std::string& ip, const std::string& system) {
        const char* query = "INSERT INTO quidich.camera (Camera, SSN, IP, System) VALUES (?, ?, ?, ?);";
        CassStatement* statement = cass_statement_new(query, 4);
        cass_statement_bind_string(statement, 0, camera.c_str());
        cass_statement_bind_int32(statement, 1, ssn);
        cass_statement_bind_string(statement, 2, ip.c_str());
        cass_statement_bind_string(statement, 3, system.c_str());
        CassFuture* result_future = cass_session_execute(session, statement);
        cass_future_wait(result_future);
        cass_future_free(result_future);
        cass_statement_free(statement);
    }

    // Get a camera row by primary key
    std::string CameraTable::get(const std::string& camera) {
        const char* query = "SELECT System FROM quidich.camera WHERE Camera = ?;";
        CassStatement* statement = cass_statement_new(query, 1);
        cass_statement_bind_string(statement, 0, camera.c_str());
        CassFuture* result_future = cass_session_execute(session, statement);
        cass_future_wait(result_future);
        const CassResult* result = cass_future_get_result(result_future);
        std::string system;
        if (cass_result_row_count(result) > 0) {
            const CassRow* row = cass_result_first_row(result);
            const CassValue* val = cass_row_get_column_by_name(row, "System");
            const char* sys; size_t len;
            cass_value_get_string(val, &sys, &len);
            system.assign(sys, len);
        }
        cass_result_free(result);
        cass_future_free(result_future);
        cass_statement_free(statement);
        return system + "|||" + camera;
    }

SystemTable::SystemTable(CassSession* sess) : session(sess) {}

    void SystemTable::insert(const std::string& system, const std::string& ip) {
        const char* query = "INSERT INTO quidich.system (System, IP) VALUES (?, ?);";
        CassStatement* statement = cass_statement_new(query, 2);
        cass_statement_bind_string(statement, 0, system.c_str());
        cass_statement_bind_string(statement, 1, ip.c_str());
        CassFuture* result_future = cass_session_execute(session, statement);
        cass_future_wait(result_future);
        cass_future_free(result_future);
        cass_statement_free(statement);
    }

    std::string SystemTable::get(const std::string& system) {
        // Only system is available for output
        return system + "|||";
    }
};

BallTable::BallTable(CassSession* sess) : session(sess) {

    void BallTable::insert(const std::string& ballId, const std::string& timestamp, const std::string& disk, const std::string& camera) {
        const char* query = "INSERT INTO quidich.ball (BallID, Timestamp, Disk, Camera) VALUES (?, ?, ?, ?);";
        CassStatement* statement = cass_statement_new(query, 4);
        cass_statement_bind_string(statement, 0, ballId.c_str());
        cass_statement_bind_string(statement, 1, timestamp.c_str());
        cass_statement_bind_string(statement, 2, disk.c_str());
        cass_statement_bind_string(statement, 3, camera.c_str());
        CassFuture* result_future = cass_session_execute(session, statement);
        cass_future_wait(result_future);
        cass_future_free(result_future);
        cass_statement_free(statement);
    }

    std::string BallTable::get(const std::string& ballId) {
        const char* query = "SELECT Disk, Camera FROM quidich.ball WHERE BallID = ?;";
        CassStatement* statement = cass_statement_new(query, 1);
        cass_statement_bind_string(statement, 0, ballId.c_str());
        CassFuture* result_future = cass_session_execute(session, statement);
        cass_future_wait(result_future);
        const CassResult* result = cass_future_get_result(result_future);
        std::string disk, camera;
        if (cass_result_row_count(result) > 0) {
            const CassRow* row = cass_result_first_row(result);
            const CassValue* disk_val = cass_row_get_column_by_name(row, "Disk");
            const CassValue* camera_val = cass_row_get_column_by_name(row, "Camera");
            const char* d; size_t dlen;
            const char* c; size_t clen;
            cass_value_get_string(disk_val, &d, &dlen);
            cass_value_get_string(camera_val, &c, &clen);
            disk.assign(d, dlen);
            camera.assign(c, clen);
        }
        cass_result_free(result);
        cass_future_free(result_future);
        cass_statement_free(statement);
        // SYSTEM is not available in ball table, so leave blank
        return "|" + disk + "|" + ballId + "|" + camera;
    }
} // namespace quidich