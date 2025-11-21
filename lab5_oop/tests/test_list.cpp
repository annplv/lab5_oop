#include <gtest/gtest.h>
#include "../include/ReusableMemory.hpp"
#include "../include/SinglyList.hpp"
#include <string>
#include <vector>

struct Complex {
    int a;
    double b;
    std::string s;
};

TEST(SinglyListTest, PushInts) {
    ReusableMemory mem;
    SinglyList<int> list(&mem);

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    std::vector<int> out;
    for (auto& v : list) out.push_back(v);

    EXPECT_EQ(out.size(), 3);
    EXPECT_EQ(out[0], 1);
    EXPECT_EQ(out[1], 2);
    EXPECT_EQ(out[2], 3);
}

TEST(SinglyListTest, ComplexValues) {
    ReusableMemory mem;
    SinglyList<Complex> list(&mem);

    list.push_back({ 5, 7.84, "mai" });
    list.push_back({ 22, 15.3, "cat" });

    auto it = list.begin();
    EXPECT_EQ(it->a, 5);
    EXPECT_DOUBLE_EQ(it->b, 7.84);
    EXPECT_EQ(it->s, "mai");
    ++it;
    EXPECT_EQ(it->a, 22);
    EXPECT_DOUBLE_EQ(it->b, 15.3);
    EXPECT_EQ(it->s, "cat");
}

TEST(SinglyListTest, MemoryReuse) {
    ReusableMemory mem;
    SinglyList<int> lst(&mem);

    lst.push_back(10);
    lst.push_back(20);
    lst.clear();
    lst.push_back(30);

    EXPECT_EQ(*lst.begin(), 30);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
