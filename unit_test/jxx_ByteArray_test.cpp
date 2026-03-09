#include <gtest/gtest.h>
#include <cstddef>       // std::byte, std::to_integer
#include "jxx.h" // Your ByteNDArray implementation
#include <cstddef>   // std::byte, std::to_integer
#include <random>
#include <map>
#include <tuple>
#include <string>
#include "jxx.h"

using namespace std;
using namespace jxx::lang;

class ByteArrayTest : public ::testing::Test {
public:
    std::mt19937 rng;
    std::uniform_int_distribution<int> byteDist;
    std::uniform_int_distribution<int> sizeDist;

    void SetUp() override {
        std::random_device rd;
        rng.seed(rd());
        byteDist = std::uniform_int_distribution<int>(0, 255);
        sizeDist = std::uniform_int_distribution<int>(1, 4); // small sizes for test
    }

    byte randomByte() {
        return byte{ static_cast<char>(byteDist(rng)) };
    }

    int randomSize() {
        return sizeDist(rng);
    }

    std::string pathToString(const std::vector<int>& path) {
        std::string s = "[";
        for (size_t i = 0; i < path.size(); ++i) {
            s += std::to_string(path[i]);
            if (i + 1 < path.size()) s += ",";
        }
        s += "]";
        return s;
    }

    void allocateJagged(ByteArray::ArrayProxy proxy, int depth, int maxDepth) {
        if (depth == maxDepth - 1) {
            proxy.allocate(ByteArrayTest::randomSize(), true); // leaf
        }
        else {
            int rows = randomSize();
            proxy.allocate(rows, false);
            for (int i = 0; i < rows; ++i) {
                allocateJagged(proxy[i], depth + 1, maxDepth);
            }
        }
    }

    void fillJagged(ByteArray::ArrayProxy proxy, int depth, int maxDepth,
        std::map<std::vector<int>, byte>& expected,
        std::vector<int> path) {
        if (depth == maxDepth - 1) {
            for (int i = 0; i < proxy.leaf().size(); ++i) {
                auto val = randomByte();
                proxy.leaf()[i] = val;
                auto fullPath = path;
                fullPath.push_back(i);
                expected[fullPath] = val;
            }
        }
        else {
            for (int i = 0; i < proxy.size(); ++i) {
                auto newPath = path;
                newPath.push_back(i);
                fillJagged(proxy[i], depth + 1, maxDepth, expected, newPath);
            }
        }
    }

    void verifyJagged(ByteArray::ArrayProxy proxy, int depth, int maxDepth,
        const std::map<std::vector<int>, byte>& expected,
        std::vector<int> path) {
        if (depth == maxDepth - 1) {
            for (int i = 0; i < proxy.leaf().size(); ++i) {
                auto fullPath = path;
                fullPath.push_back(i);
                ASSERT_EQ(proxy.leaf()[i], expected.at(fullPath))
                    << "Mismatch at path: " << pathToString(fullPath);
            }
        }
        else {
            for (int i = 0; i < proxy.size(); ++i) {
                auto newPath = path;
                newPath.push_back(i);
                verifyJagged(proxy[i], depth + 1, maxDepth, expected, newPath);
            }
        }
    }
};

/*
TEST_F(ByteArrayTest, RandomJaggedStressTest) {
   
    const int maxDepth = 3; // e.g., byte[][][]
    ByteArray arr(this->randomSize()); // random first dimension

    allocateJagged(arr[0], 0, maxDepth);

    std::map<std::vector<int>, byte> expected;
    fillJagged(arr[0], 0, maxDepth, expected, {});

    verifyJagged(arr[0], 0, maxDepth, expected, {});
}
*/