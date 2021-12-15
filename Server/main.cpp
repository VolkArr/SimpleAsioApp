#include "../StructConfigs/server_libs.hpp"



enum class Msg : uint32_t{
    ServerPing,
    HelloFromServer,
    RandomNumber,
    Spam,
    ServerMessage,

};

/* Test Server */
class TestServer : public custom::framework::server_interface<Msg>{
    public:
        TestServer(uint16_t port) : custom::framework::server_interface<Msg>(port){

        }
    protected:
        virtual bool onClientConnect(std::shared_ptr<custom::framework::connection<Msg>> client){
            custom::framework::message<Msg> msg;
            msg.head.id = Msg::HelloFromServer;
            client->Send(msg);
            return true;
        }
        
        virtual void onClientDisconnect(std::shared_ptr<custom::framework::connection<Msg>> client){
            std::clog << " Disconnect client! id [" << client->GetID() << "] " << std::endl;
        }

        virtual void onMessage(custom::framework::message<Msg>& msg, std::shared_ptr<custom::framework::connection<Msg>> client){
            switch (msg.head.id){
            case Msg::ServerPing:{
                    std::cout << " Server pinging by [" << client->GetID() << "] client " << std::endl;
                    custom::framework::message<Msg> tmp{};
                    tmp.head.id =  Msg::HelloFromServer;
                    client->Send(tmp);
                }
                break;
                
            case Msg::Spam:{
                    std::clog << " Client [" << client->GetID() << "] send msg to all " << std::endl;
                    custom::framework::message<Msg> tmp{};
                    tmp.head.id =  Msg::ServerMessage;
                    tmp << client->GetID();
                    AlertAllClients(tmp, client);
                }
                break;
            }
        }
};

int main(){
    std::clog << "Server start" << std::endl;
    TestServer server(60000);
    server.Start();
    while (true){
        server.Update(-1, true);
    }
    
    
    return -1;
}