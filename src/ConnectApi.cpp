#include "ConnectApi.hpp"
#include "DBApi.hpp"
#include <zmq.hpp>
#include <vector>
#include <string>

namespace quidich {

void run_zmq_ball_path_server(const std::string& zmq_endpoint, void* cassandra_session) {
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind(zmq_endpoint);

    BallTable ball_table(static_cast<CassSession*>(cassandra_session));

    while (true) {
        zmq::message_t request;
        socket.recv(request, zmq::recv_flags::none);
        std::string ball_id(static_cast<char*>(request.data()), request.size());

        std::vector<std::string> paths = ball_table.get(ball_id);

        std::string response;
        for (const auto& path : paths) {
            response += path + "\n";
        }
        if (!response.empty() && response.back() == '\n') {
            response.pop_back();
        }

        zmq::message_t reply(response.size());
        memcpy(reply.data(), response.data(), response.size());
        socket.send(reply, zmq::send_flags::none);
    }
}

}
