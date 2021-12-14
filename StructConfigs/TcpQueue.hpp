#include "headers.hpp"

namespace custom{

    namespace tcpqueue{
            template<class T>
            class TcpQueue{

                public:
                    TcpQueue() = default;
                    TcpQueue(const TcpQueue<T>&) =delete;
                    virtual ~TcpQueue(){ clear{}; }

                    const T& front(){
                        std::scoped_lock lock(mtx);
                        return deq.front();
                    }

                    const T& back(){
                        std::scoped_lock lock(mtx);
                        return deq.back();
                    }

                    void push_back(const T& data){
                        std::scoped_lock lock(mtx);
                        deq.emplace_back(std::move(data));
                    }

                    void push_front(const T& data){
                        std::scoped_lock lock(mtx);
                        deq.emplace_front(std::move(data));
                    }

                    void clear(){
                        std::scoped_lock lock(mtx);
                        deq.clear();
                    }

                    bool empty(){
                        std::scoped_lock lock(mtx);
                        return deq.empty();
                    }

                    size_t size(){
                        std::scoped_lock lock(mtx);
                        return deq.size();
                    }

                    T pop_front(){
                        std::scoped_lock lock(mtx);
                        auto tmp = std::move(deq.front());
                        deq.pop_front();
                        return tmp;
                    }

                     T pop_back(){
                        std::scoped_lock lock(mtx);
                        auto tmp = std::move(deq.back());
                        deq.pop_front();
                        return tmp;
                    }

                protected:
                    std::mutex mtx;
                    std::deque<T> deq; 
            };

    }
}
