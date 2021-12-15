#pragma once
#include "headers.hpp"

namespace custom{

    namespace framework{
            template<class T>
            class TcpQueue{

                public:
                    TcpQueue() = default;
                    TcpQueue(const TcpQueue<T>&) =delete;
                    virtual ~TcpQueue(){ clear(); }

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

                        std::unique_lock<std::mutex> uniq_lock(CVmtx);
                        CV.notify_one();
                    }

                    void push_front(const T& data){
                        std::scoped_lock lock(mtx);
                        deq.emplace_front(std::move(data));

                        std::unique_lock<std::mutex> uniq_lock(CVmtx);
                        CV.notify_one();
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

                    void wait(){
                        while(empty()){
                            std::unique_lock<std::mutex> uniq_lock(CVmtx);
                            CV.wait(uniq_lock); 
                        }
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
                    std::condition_variable CV;
                    std::mutex CVmtx;
            };

    }
}
