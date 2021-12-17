#pragma once
#include <memory>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <algorithm>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>

//asio
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

#include "server_interface.hpp"
#include "client_interface.hpp"
#include "connection.hpp"
#include "message.hpp"
#include "TcpQueue.hpp"


//boost
#include <boost/bind/bind.hpp>
#include <boost/bind.hpp>

//beast
//#include <boost/beast.hpp>
