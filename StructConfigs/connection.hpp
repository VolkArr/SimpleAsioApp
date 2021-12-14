#pragma once
#include "headers.hpp"
#include "message.hpp"
#include "TcpQueue.hpp"


namespace custom{

    namespace connection{
        
        template<class T>
        class connection : public std::enable_shared_from_this<connection<T>>{
            public:
                connection(){}
                virtual ~connection(){}

                bool ConnectToServer(); //Call only by clients
                bool Disconnect(); // Call by clients and server
                bool IsConnected() const;

                bool Send(const message::message<T>& msg);

                protected:
                boost::asio::ip::tcp::socket tcp_socket;
                boost::asio::io_context& tcp_context;
                tcpqueue::TcpQueue<message::message<T>> tcp_msqQueueRequest;
                tcpqueue::TcpQueue<message::message_sender>& tcp_msqQueueRespond;
                    


        };

    }
}