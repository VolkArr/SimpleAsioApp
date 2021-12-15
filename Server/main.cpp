#include "../StructConfigs/server_libs.hpp"

enum class Msg : uint32_t{
    RandomNumber,

};

/* Test Server */
class TestServer : public custom::framework::server_interface<Msg>{
    public:
        TestServer(uint16_t port) : custom::framework::server_interface<Msg>(port){

        }
    protected:
        virtual bool onClientConnect(std::shared_ptr<custom::framework::connection<Msg>> client){
            return true;
        }
        
        virtual void onClientDisconnect(std::shared_ptr<custom::framework::connection<Msg>> client){

        }

        virtual void onMessage(custom::framework::message<Msg>& msg, std::shared_ptr<custom::framework::connection<Msg>> client){

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