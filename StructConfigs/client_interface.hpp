#pragma once
#include "connection.hpp"

namespace custom{
    
    namespace client{

        template<class T>
        class client_interface{



            public:

                client_interface() :
                         client_socket(tcp_context)
                {

                }
                 virtual ~client_interface(){
                    Disconnect();
                  }

                bool Connect(const std::string& host, const uint16_t port){

                    /*try{
                        tcp_connection = std::make_unique<connection<T>>();

                        boost::asio::ip::tcp::resolver resolver(tcp_context);
                        auto tcp_endpoints = resolver.resolve(host, std::to_string(port));
                        tcp_connection->ConnectToServer(tcp_endpoints);
                        tcp_ContextThread = std::thread([this](){ tcp_context->run()});
                        
                    }
                    catch(std::exception& error){
                        std::cerr << " Client Error: " << error.what() << std::endl;
                        return false;
                    }*/
                    return false;
                }

                void Disconnect(){
                    if(IsConnected()){
                        tcp_connection->Disconnect();
                    }
                    
                    tcp_context.stop();
                    if (tcp_ContextThread.joinable()) tcp_ContextThread.join();

                    tcp_connection.release();
                    
                }

                bool IsConnected(){

                    if(tcp_connection)
                        return tcp_connection->IsConnected();
                    else
                        return false;

                }

                tcpqueue::TcpQueue<message::message_sender<T>>& get_msgQueueRespond(){
                    return tcp_msqQueueRespond;
                }


            protected:

                boost::asio::io_context tcp_context;

                std::thread tcp_ContextThread;

                boost::asio::ip::tcp::socket client_socket;
                
                std::unique_ptr<connection::connection<T>> tcp_connection;

            private:
                tcpqueue::TcpQueue<message::message_sender<T>> tcp_msqQueueRespond;
        };

    }

}