#pragma once
#include <string>

namespace quidich {

void run_zmq_ball_path_server(const std::string& zmq_endpoint, void* cassandra_session);

}
