#pragma once
#include "connection.hpp"
#include "TcpQueue.hpp"
#include "message.hpp"
#include "headers.hpp"


namespace custom{
    
    namespace framework{

        template<class T>
        class client_interface{



            public:

                client_interface()
                {

                }

                 virtual ~client_interface(){
                    Disconnect();
                  }

                bool Connect(const std::string& host, const uint16_t port){

                    try{
                        tcp_connection = std::make_unique<connection<T>>();

                        boost::asio::ip::tcp::resolver resolver(tcp_context);
                        boost::asio::ip::tcp::resolver::results_type tcp_endpoints = resolver.resolve(host, std::to_string(port));
                        tcp_connection = std::make_unique<connection<T>>(custom::framework::connection<T>::owner::client, tcp_context, boost::asio::ip::tcp::socket(tcp_context), tcp_msqQueueRespond);
                        tcp_connection->ConnectToServer(tcp_endpoints);
                        tcp_ContextThread = std::thread([this](){ tcp_context.run(); });
                        
                    }
                    catch(std::exception& error){
                        std::cerr << " Client Error: " << error.what() << std::endl;
                        return false;
                    }
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

                    if(tcp_connection) return tcp_connection->IsConnected();
                    else return false;

                }

                void Send(const custom::framework::message<T>& msg){
                    if(IsConnected()) tcp_connection->Send(msg);
                }


                TcpQueue<message_sender<T>>& get_msgQueueRespond(){
                    return tcp_msqQueueRespond;
                }


            protected:

                boost::asio::io_context tcp_context;

                std::thread tcp_ContextThread;
             
                std::unique_ptr<connection<T>> tcp_connection;

            private:
                TcpQueue<message_sender<T>> tcp_msqQueueRespond;
        };

    }

}