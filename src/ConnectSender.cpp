#include <zmq.hpp>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <zmq_endpoint> <BallID>\n";
        return 1;
    }
    std::string endpoint = argv[1];
    std::string ball_id = argv[2];

    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::req);
    socket.connect(endpoint);

    zmq::message_t request(ball_id.size());
    memcpy(request.data(), ball_id.data(), ball_id.size());
    socket.send(request, zmq::send_flags::none);

    zmq::message_t reply;
    socket.recv(reply, zmq::recv_flags::none);
    std::string response(static_cast<char*>(reply.data()), reply.size());
    std::cout << "Paths for BallID '" << ball_id << "':\n" << response << std::endl;

    return 0;
}
