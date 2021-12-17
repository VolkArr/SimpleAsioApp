#include "../StructConfigs/client_libs.hpp"

enum class Msg : uint32_t{
    ServerPing,
    HelloFromServer,
    RandomNumber,
    Spam,
    ServerMessage,

};

class TestClient : public custom::framework::client_interface<Msg>{

    public:
        void PingServer(){
            custom::framework::message<Msg> msg;
            msg.head.id = Msg::ServerPing;

            std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();

            msg << current_time;
            Send(msg);
        }

        void AlertAll(){
            custom::framework::message<Msg> msg;
            msg.head.id = Msg::Spam;
            Send(msg);
        }
};


int main(){
    // TestClient c;
    // c.Connect("127.0.0.1", 60000);


    return -1;
}

