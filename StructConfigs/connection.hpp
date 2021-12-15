#pragma once
#include "headers.hpp"
#include "message.hpp"
#include "TcpQueue.hpp"


template<class T>
class TcpQueue;

namespace custom{

    namespace framework{
        
        template<class T>
        class connection : public std::enable_shared_from_this<connection<T>>{
            public:

                enum class owner{
                    server,
                    client
                };

                connection(owner parent, boost::asio::io_context& _context, boost::asio::ip::tcp::socket socket, TcpQueue<message_sender<T>>& in):
                                tcp_context(_context),
                                tcp_socket( socket),
                                tcp_msqQueueRespond(in)

                            
                {
                    Parent = parent;
                    id = 0;
                }


                virtual ~connection(){}


                uint32_t GetID() const{ return id; }


                void ConnectToServer(const boost::asio::ip::tcp::resolver::results_type& endpoints){
                    if(Parent == owner::client){
                        boost::asio::async_connect(tcp_socket, endpoints,
                        [this](std::error_code& error, boost::asio::ip::tcp::endpoint endpoint){
                            if(!error) ReadHeader();
                        });
                    }
                    
                }//Call only by clients
                void ConnectToClient(uint32_t pid = 0 ){
                    if(Parent == owner::server){
                        if(tcp_socket.is_open()){
                            id = pid;
                            ReadHeader();
                        }
                    }
                }

                void Disconnect() {
                    if (IsConnected()) boost::asio::post(tcp_context, [this]() { tcp_socket.close(); });
                } // Call by clients and server


                bool IsConnected() const { return tcp_socket.is_open(); }


                void StartListening() {}

                void Send(const message<T>& msg){
                    boost::asio::post(tcp_context, [this, msg](){
                        bool WritingMessage = !tcp_msqQueueRequest.empty();
                        tcp_msqQueueRequest.push_back(msg);
                        if (!WritingMessage) WriteHeader();
                    });
                }

                private:
                void WriteHeader(){
                    boost::asio::async_write(tcp_socket, boost::asio::buffer(&tcp_msqQueueRequest.front().head, sizeof(message_head<T>)), 
                    [this](std::error_code error, size_t length){
                        if(!error){
                            if(tcp_msqQueueRequest.front().body.size() > 0){
                                WriteBody();
                            }
                            else{
                                tcp_msqQueueRequest.pop_front();
                                if(!tcp_msqQueueRequest.empty()) WriteHeader();
                            }
                        }
                        else{
                            std::cerr << " [" << id  << "] Write head error: " << error.message() << std::endl;
                            tcp_socket.close();

                        }
                    });
                }

                void WriteBody() {
                    boost::asio::async_write(tcp_socket, boost::asio::buffer(tcp_msqQueueRequest.front().body.data(), tcp_msqQueueRequest.front().head.size()),
                    [this](std::error_code error, size_t length){
                        if(!error){

                                tcp_msqQueueRequest.pop_front();
                                if(!tcp_msqQueueRequest.empty()) WriteHeader();
                        }
                        else{
                            std::cerr << " [" << id  << "] Write body error: " << error.message() << std::endl;
                            tcp_socket.close();

                        }
                    });
                }

                void ReadHeader() {
                    boost::asio::async_read(tcp_socket, boost::asio::buffer(&tmpMsg.head, sizeof(message_head<T>)),
                    [this](std::error_code error, size_t length){
                        if(!error){
                            if(tmpMsg.head.size > 0){
                                tmpMsg.body.resize(tmpMsg.head.size);
                                ReadBody();
                            }
                                
                            else AddToIncomingMessageQueue();
                        }
                        else{
                            std::cerr << " [" <<  id  << "] Read head error: " << error.message() << std::endl;
                            tcp_socket.close();
                        }
                    });
                }

                void ReadBody() {
                    boost::asio::async_read(tcp_socket, boost::asio::buffer(tmpMsg.body.data(), tmpMsg.body.size()),
                    [this](std::error_code error, size_t length){
                        if (!error) AddToIncomingMessageQueue();
                        else {
                            std::cerr << " [" << id << "] Read body error: "  << error.message() << std::endl;
							tcp_socket.close();
                        }
                    });
                }


                void AddToIncomingMessageQueue() {
                    if(Parent == owner::server)  tcp_msqQueueRespond.push_back({this->shared_from_this(), tmpMsg});
                    else tcp_msqQueueRespond.push_back({nullptr, tmpMsg});
                    ReadHeader();
                }
                

            


                protected:
                boost::asio::ip::tcp::socket tcp_socket;
                boost::asio::io_context& tcp_context;
                TcpQueue<message<T>> tcp_msqQueueRequest; // out
                TcpQueue<message_sender<T>>& tcp_msqQueueRespond; // in
                message<T> tmpMsg;
                message<T> tcp_msgTMP_in;
                owner Parent = owner::server;
                uint32_t id;
                    


        };

    }
}