#pragma once
#include "headers.hpp"
#include "message.hpp"
#include "TcpQueue.hpp"


namespace custom{

    namespace connection{
        
        template<class T>
        class connection : public std::enable_shared_from_this<connection<T>>{
            public:

                enum class owner{
                    server,
                    client
                };

                connection(owner parent, boost::asio::io_context& _context, boost::asio::ip::tcp::socket socket, tcpqueue::TcpQueue<message::message_sender<T>>& in):
                                tcp_context(_context),
                                tcp_socket( socket),
                                tcp_msqQueueRespond(in)

                            
                {
                    Parent = parent;
                    id = 0;
                }

                virtual ~connection(){}

                uint32_t GetID() const{ return id; }

                bool ConnectToServer(const boost::asio::ip::tcp::resolver::results_type& endpoints){
                    if(Parent == owner::client){
                        boost::asio::async_connect(tcp_socket, endpoints,
                        [this](std::error_code& error, boost::asio::ip::tcp::endpoint endpoint){
                            if(!error) ReadHeader();
                        });
                    }
                    
                }//Call only by clients
                bool ConnectToClient(uint32_t pid = 0 ){
                    if(Parent == owner::server){
                        if(tcp_socket.is_open()){
                            id = pid;
                            ReadHeader();
                        }
                    }
                }
                bool Disconnect() {
                    if (IsConnected())
					boost::asio::post(tcp_context, [this]() { tcp_socket.close(); });
                } // Call by clients and server
                bool IsConnected() const { return tcp_socket.is_open(); }
                void StartListening() {}

                bool Send(const message::message<T>& msg){
                    boost::asio::serial_port(tcp_context, [this, msg](){
                        bool WritingMessage = !tcp_msqQueueRequest.empty();
                        tcp_msqQueueRequest.push_back(msg);
                        if (!WritingMessage) WriteHeader();
                    });
                }

                private:
                void WriteHeader(){}
                void WriteBody() {}
                void ReadHeader() {}
                void ReadBody() {}

                void AddToIncomingMessageQueue() {}

                protected:
                boost::asio::ip::tcp::socket tcp_socket;
                boost::asio::io_context& tcp_context;
                tcpqueue::TcpQueue<message::message<T>> tcp_msqQueueRequest; // out
                tcpqueue::TcpQueue<message::message_sender<T>>& tcp_msqQueueRespond; // in
                message::message<T> tcp_msgTMP_in;
                owner Parent = owner::server;
                uint32_t id;
                    


        };

    }
}