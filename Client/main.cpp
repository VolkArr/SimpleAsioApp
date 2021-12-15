#include "../StructConfigs/client_libs.hpp"

enum class Msg : uint32_t{
    RandomNumber,

};


int main(){
    std::clog << "Client start" << std::endl;
    custom::framework::message<Msg> msg;
    msg.head.id = Msg::RandomNumber;
    return -1;
}