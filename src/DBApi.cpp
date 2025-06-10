#include "DBApi.hpp"
#include <iostream>

namespace quidich {

CameraTable::CameraTable(CassSession* sess) : session(sess) {}

    // Insert a camera row
    void CameraTable::insert(const std::string& camera, int ssn, const std::string& ip, const std::string& system) {
        const char* query = "INSERT INTO quidich.camera (Camera, SSN, Camera_IP, System) VALUES (?, ?, ?, ?);";
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
        const char* query = "SELECT Camera, SSN, Camera_IP, System FROM quidich.camera WHERE Camera = ?;";
        CassStatement* statement = cass_statement_new(query, 1);
        cass_statement_bind_string(statement, 0, camera.c_str());
        CassFuture* result_future = cass_session_execute(session, statement);
        cass_future_wait(result_future);
        const CassResult* result = cass_future_get_result(result_future);
        std::string output;
        if (cass_result_row_count(result) > 0) {
            const CassRow* row = cass_result_first_row(result);
            const char* ip; size_t ip_len;
            const char* sys; size_t sys_len;
            cass_int32_t ssn;
            const CassValue* ip_val = cass_row_get_column_by_name(row, "Camera_IP");
            const CassValue* sys_val = cass_row_get_column_by_name(row, "System");
            const CassValue* ssn_val = cass_row_get_column_by_name(row, "SSN");
            cass_value_get_string(ip_val, &ip, &ip_len);
            cass_value_get_string(sys_val, &sys, &sys_len);
            cass_value_get_int32(ssn_val, &ssn);
            output = std::string(sys, sys_len) + "|" + std::to_string(ssn) + "|" + 
                    std::string(ip, ip_len) + "|" + camera;
        }
        cass_result_free(result);
        cass_future_free(result_future);
        cass_statement_free(statement);
        return output;
    }

SystemTable::SystemTable(CassSession* sess) : session(sess) {}

    void SystemTable::insert(const std::string& system, const std::string& ip) {
        const char* query = "INSERT INTO quidich.system (System, System_IP) VALUES (?, ?);";
        CassStatement* statement = cass_statement_new(query, 2);
        cass_statement_bind_string(statement, 0, system.c_str());
        cass_statement_bind_string(statement, 1, ip.c_str());
        CassFuture* result_future = cass_session_execute(session, statement);
        cass_future_wait(result_future);
        cass_future_free(result_future);
        cass_statement_free(statement);
    }

    std::string SystemTable::get(const std::string& system) {
        const char* query = "SELECT System, System_IP FROM quidich.system WHERE System = ?;";
        CassStatement* statement = cass_statement_new(query, 1);
        cass_statement_bind_string(statement, 0, system.c_str());
        CassFuture* result_future = cass_session_execute(session, statement);
        cass_future_wait(result_future);
        const CassResult* result = cass_future_get_result(result_future);
        std::string output;
        if (cass_result_row_count(result) > 0) {
            const CassRow* row = cass_result_first_row(result);
            const char* sys_ip; size_t ip_len;
            const CassValue* ip_val = cass_row_get_column_by_name(row, "System_IP");
            cass_value_get_string(ip_val, &sys_ip, &ip_len);
            output = system + "|" + std::string(sys_ip, ip_len) + "|";
        }
        cass_result_free(result);
        cass_future_free(result_future);
        cass_statement_free(statement);
        return output;
    }

// End SystemTable implementation

BallTable::BallTable(CassSession* sess) : session(sess) {}

    void BallTable::insert(const std::string& ballId, const std::string& timestamp, const std::string& disk, const std::string& camera) {
        const char* query = "INSERT INTO quidich.ball (BallID, Timestamp, Disk, Camera) VALUES (?, ?, ?, ?);";
        CassStatement* statement = cass_statement_new(query, 4);
        cass_statement_bind_string(statement, 0, ballId.c_str());
        cass_statement_bind_string(statement, 1, timestamp.c_str());
        cass_statement_bind_string(statement, 2, disk.c_str());
        cass_statement_bind_string(statement, 3, camera.c_str());
        
        CassFuture* result_future = cass_session_execute(session, statement);
        cass_future_wait(result_future);
        
        if (cass_future_error_code(result_future) != CASS_OK) {
            const char* message;
            size_t message_length;
            cass_future_error_message(result_future, &message, &message_length);
            std::cerr << "Error inserting ball data: " << std::string(message, message_length) << std::endl;
        }
        
        cass_future_free(result_future);
        cass_statement_free(statement);
    }

    std::string BallTable::get(const std::string& ballId) {
        const char* query = "SELECT BallID, Timestamp, Disk, Camera FROM quidich.ball WHERE BallID = ?;";
        CassStatement* statement = cass_statement_new(query, 1);
        cass_statement_bind_string(statement, 0, ballId.c_str());
        CassFuture* result_future = cass_session_execute(session, statement);
        cass_future_wait(result_future);
        const CassResult* result = cass_future_get_result(result_future);
        
        std::string paths;
        CassIterator* rows = cass_iterator_from_result(result);
        bool hasRows = false;
        
        while (cass_iterator_next(rows)) {
            hasRows = true;
            const CassRow* row = cass_iterator_get_row(rows);
            const char* disk; size_t disk_len;
            const char* cam; size_t cam_len;
            const char* ts; size_t ts_len;
            
            const CassValue* disk_val = cass_row_get_column_by_name(row, "Disk");
            const CassValue* cam_val = cass_row_get_column_by_name(row, "Camera");
            const CassValue* ts_val = cass_row_get_column_by_name(row, "Timestamp");
            
            if (disk_val && cam_val && ts_val) {
                cass_value_get_string(disk_val, &disk, &disk_len);
                cass_value_get_string(cam_val, &cam, &cam_len);
                cass_value_get_string(ts_val, &ts, &ts_len);
                
                paths +=  std::string(disk, disk_len) + "|" + 
                        ballId + "|" + 
                        std::string(cam, cam_len) + "@";
            }
        }
        
        cass_iterator_free(rows);
        cass_result_free(result);
        cass_future_free(result_future);
        cass_statement_free(statement);
        
        return hasRows ? paths : "Paths are empty";
    }
} // namespace quidich
