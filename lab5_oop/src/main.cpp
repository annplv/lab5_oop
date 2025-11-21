#include "../include/ReusableMemory.hpp"
#include "../include/SinglyList.hpp"
#include <iostream>
#include <string>

struct Complex {
    int a;
    double b;
    std::string s;
};

int main() {
    ReusableMemory mem;

    SinglyList<int> li(&mem);
    li.push_back(10);
    li.push_back(20);
    li.push_back(30);

    std::cout << "Ints: ";
    for (auto& v : li) std::cout << v << " ";
    std::cout << "\n";

    SinglyList<Complex> lc(&mem);
    lc.push_back({ 1, 2.5, "memory" });
    lc.push_back({ 17, 3.75, "cat" });

    std::cout << "Complex:\n";
    for (auto& v : lc)
        std::cout << v.a << ", " << v.b << ", " << v.s << "\n";

    return 0;
}

