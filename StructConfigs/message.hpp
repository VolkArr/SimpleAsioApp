#include "headers.hpp"

namespace custom{

    namespace message{
        
        template<class T>
        struct message_head{
            T id{};
            uint32_t size = 0;
        };

        template<class T>
        struct message{
            message_head<T> head{};
            std::vector<uint8_t> body;


            size_t size() const{

                return sizeof(message_head<T>) + body.size();
            }

            friend std::ostream& operator << (std::ostream& os,     const message<T>& msg){

                os << "Head id: " << static_cast<int>(msg.head.id) << " Bytes: " << msg.head.size;

                return os;
            }
            
            template<class ClassOfData>
            friend message<T>& operator << (message<T>& msg, const ClassOfData& data){

                static_assert(std::is_standard_layout<ClassOfData>::value, "Cant push in vector! Data too complex");

                size_t vector_bytes  = msg.body.size();

                msg.body.resize(msg.body.size() + sizeof(ClassOfData));

                std::memcpy(msg.body.data() + vector_bytes, &data, sizeof(ClassOfData));

                msg.head.size = msg.size();

                return msg;
            }

            template<class ClassOfData>
            friend message<T>& operator >> (message<T>& msg, const ClassOfData& data){

                size_t vector_bytes = msg.body.size() - sizeof(ClassOfData);

                std::memcpy(&data, msg.body.data() + vector_bytes, sizeof(ClassOfData));

                msg.body.resize(vector_bytes);
                
                msg.head.size = msg.size();

                return msg;
            }
        };

    }
}