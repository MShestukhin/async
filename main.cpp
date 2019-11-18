#include <iostream>
#include <set>
#include "async.h"
#include "thread"
#include "mutex"
std::mutex lk;
void thr_first(){
    for( int a=0;a<10;a++){
        lk.lock();
        std::cout<<a<<"";
        lk.unlock();
    }
}
void thr_second(){
    for( int a=10; a<20;a++){
        lk.lock();
        std::cout<<a<<"\t";
        lk.unlock();
    }
}
void thr_last(){
    std::cout<<"\n";
}
int main(int, char *[]) {
    std::set<string> s;
//    std::size_t bulk = 5;
//    auto h = async::connect(bulk);
//    auto h2 = async::connect(bulk);
//    async::receive(h, "1", 1);
//    async::receive(h2, "1\n", 2);
//    async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
//    async::receive(h, "b\nc\nd\n}\n89\n", 11);
//    async::disconnect(h);
//    async::disconnect(h2);
    std::thread first(&thr_first);
    std::thread second(&thr_second);
//    std::thread last(&thr_last);
    first.join();
    second.join();
//    last.detach();

    return 0;
}
