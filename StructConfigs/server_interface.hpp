#pragma once
#include "connection.hpp"

namespace custom{

    namespace server{
        template<class T>
        class server_interface{
          public:
                server_interface(uint16_t port):
                            tcp_Acceptor(tcp_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
                {

                }  

                virtual ~server_interface(){ Stop(); }

                bool Start(){
                    try{
                        WaitForClientConnection();
                        tcp_ContextThread = std::thread([this](){ tcp_context.run(); });
                    }
                    catch(std::exception& error){
                        std::cerr << " Server Error: " << error.what() << std::endl;
                        return false;
                    }

                    std::clog << " Server start script run! " << std::endl;
                    return true;
                }

                void Stop(){
                    tcp_context.stop();

                    if (tcp_ContextThread.joinable()) tcp_ContextThread.join();

                    std::clog << " Server stopped! " << std::endl;

                }

                //ASIO context work
                void WaitForClientConnection(){
                    tcp_Acceptor.async_accept(
                        [this](std::error_code error, boost::asio::ip::tcp::socket socket){
                            if(!error){
                                std::clog << " New connection!: " << socket.remote_endpoint() << std::endl;

                                // std::shared_ptr<connection::connection<T>> NewConnection = std::make_shared<connection::connection<T>>(
                                //     connection::connection<T>::owner::server, tcp_context, std::move(socket), tcp_msqQueueRespond );
                                // if (onClientConnect(NewConnection)){
                                //     tcp_deqConnections.push_back(std::move(NewConnection));
                                //     tcp_deqConnections.back()->ConnectToClient(_IDclient++);
                                //     std::clog << " [" << tcp_deqConnections.back()->GetID() <<  "] Connect accepted! " << std::endl;
                                // }
                                // else std::clog << " Connection denied! " << std::endl;
                            }
                            else std::cerr << " Server new connection error: " << error.message() << std::endl; 
                            WaitForClientConnection();
                        });
                }

                void SendMsgToClient(std::shared_ptr<connection::connection<T>> client, const message::message<T>& msg){
                    if(client && client->IsConnected()){
                        client->Send(msg);
                    }
                    else{
                        onClientDisconnect(client);
                        client.reset();
                        tcp_deqConnections.erase(std::remove(tcp_deqConnections.begin(), tcp_deqConnections.end(), client), tcp_deqConnections.end());
                    }
                }

                void AlertAllClients(const message::message<T>& msg, std::shared_ptr<connection::connection<T>> IgnoreClient = nullptr){
                    bool ErrorConnections = false;
                    for(auto& client : tcp_deqConnections){
                        if (client && client->IsConnected()){
                            if (client != IgnoreClient) client->Send(msg);
                        }
                        else{
                            onClientDisconnect(client);
                            client.reset();
                            ErrorConnections = true;
                        }
                    }
                    
                    if(ErrorConnections == true){
                        tcp_deqConnections.erase(std::remove(tcp_deqConnections.begin(), tcp_deqConnections.end(), nullptr), tcp_deqConnections.end());
                    }

                }

                void Update(size_t _MaxMsg = -1){ // size_t is uint that mean -1 == max

                    size_t _msgCounter = 0;
                    while (_msgCounter < _MaxMsg && !tcp_msqQueueRespond.empty()){
                        auto tmpMsg = tcp_msqQueueRespond.pop_front();

                        onMessage(tmpMsg.remote, tmpMsg.msg);

                        _msgCounter++;
                    }
                    

                }

            protected:

                virtual bool onClientConnect(std::shared_ptr<connection::connection<T>> client){
                    return false;
                }

                virtual void onClientDisconnect(std::shared_ptr<connection::connection<T>> client){

                }

                virtual void onMessage(std::shared_ptr<connection::connection<T>> client, message::message<T>& msg ){

                }


                std::deque<std::shared_ptr<connection::connection<T>>> tcp_deqConnections;

                tcpqueue::TcpQueue<message::message_sender<T>> tcp_msqQueueRespond;

                boost::asio::io_context tcp_context;
                std::thread tcp_ContextThread;

                boost::asio::ip::tcp::acceptor tcp_Acceptor;

                uint32_t  _IDclient = 10000;
            

        };
    }
}