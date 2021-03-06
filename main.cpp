#include <iostream>
#include <set>
#include "async.h"
#include "thread"
#include "mutex"
int main(int, char *[]) {
    std::set<string> s;
    std::size_t bulk = 5;
    auto h = async::connect(bulk);
    auto h2 = async::connect(bulk);
    async::receive(h, "1", 1);
    async::receive(h2, "1\n4\n5\n6\n7\n8", 2);
    async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    async::receive(h, "b\nc\nd\n}\n89\n", 11);
//    async::disconnect(h);
//    async::disconnect(h2);
    return 0;
}
