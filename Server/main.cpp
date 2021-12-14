#include "../StructConfigs/headers.hpp"

enum class Msg : uint32_t{
    RandomNumber,

};

/* Test Server */
class TestServer : public custom::server::server_interface<Msg>{
    public:
        TestServer(uint16_t port) : custom::server::server_interface<Msg>(port){

        }
    protected:
        virtual bool onClientConnect(std::shared_ptr<custom::connection::connection<Msg>> client){
            return true;
        }
        
        virtual void onClientDisconnect(std::shared_ptr<custom::connection::connection<Msg>> client){

        }

        virtual void onMessage(custom::message::message<Msg>& msg, std::shared_ptr<custom::connection::connection<Msg>> client){

        }
};

int main(){
    std::clog << "Server start" << std::endl;
    TestServer server(60000);
    server.Start();
    while (true){
        server.Update();
    }
    
    
    return -1;
}