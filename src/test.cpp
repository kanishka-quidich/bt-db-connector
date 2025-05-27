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
        
        // Test camera table
        quidich::CameraTable camera(session);
        camera.insert("cam1", 123, "192.168.1.1", "sys1");
        std::cout << camera.get("cam1") << std::endl;
        
        // Cleanup
        cass_session_free(session);
        cass_cluster_free(cluster);
    } else {
        std::cout << "Failed to connect" << std::endl;
    }
    
    cass_future_free(connect_future);
    return 0;
}

