#include "../StructConfigs/headers.hpp"
#include "../StructConfigs/message.hpp"

enum class Msg : uint32_t{
    RandomNumber,

};


int main(){
    std::clog << "Client start" << std::endl;
    custom::message::message<Msg> msg;
    msg.head.id = Msg::RandomNumber;
    return -1;
}