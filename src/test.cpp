#include "DBApi.hpp"
#include <iostream>

int main() {
    // Initialize the Cassandra cluster configuration
    CassCluster* cluster = cass_cluster_new();
    CassSession* session = cass_session_new();
    
    // Set the contact points
    cass_cluster_set_contact_points(cluster, "127.0.0.1");
    
    // Connect to the cluster
    CassFuture* connect_future = cass_session_connect(session, cluster);
    
    if (cass_future_error_code(connect_future) == CASS_OK) {
        std::cout << "Connected to cluster" << std::endl;
        
        // Test system table
        quidich::SystemTable system(session);
        system.insert("sys1", "192.168.1.2");
        std::cout << system.get("sys1") << std::endl;
        
        // Test camera table
        quidich::CameraTable camera(session);
        camera.insert("cam1", 123, "192.168.1.1", "sys1");
        std::cout << camera.get("cam1") << std::endl;
        
        // Test ball table with multiple entries
        quidich::BallTable ball(session);
        std::cout << "Inserting ball data..." << std::endl;
        ball.insert("ball1", "2023-01-01T00:00:00Z", "/mnt/disk1", "cam1");
        ball.insert("ball1", "2023-01-01T00:00:01Z", "/mnt/disk2", "cam2");
        ball.insert("ball1", "2023-01-01T00:00:02Z", "/mnt/disk3", "cam3");
        
        // Add a small delay to ensure data is written
        cass_cluster_set_request_timeout(cluster, 5000);
        
        std::cout << "Retrieving ball paths..." << std::endl;
        std::string result = ball.get("ball1");
        std::cout << "Ball paths: " << result << std::endl;
        
        cass_session_free(session);
        cass_cluster_free(cluster);
    } else {
        std::cout << "Failed to connect" << std::endl;
    }
    
    cass_future_free(connect_future);
    return 0;
}

