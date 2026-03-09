
#pragma once
#include <array>
#include <string>
#include <random>
#include <sstream>

namespace jxx { namespace util {

class UUID {
public:
    UUID() { data_.fill(0); }
    static UUID randomUUID() {
        UUID u; std::random_device rd; std::mt19937_64 gen(rd());
        for (size_t i=0;i<16;i+=8) { auto v = gen(); for (int b=0;b<8;++b) u.data_[i+b] = (v>>(8*b)) & 0xFF; }
        // Set version and variant to 4/variant 2 per RFC 4122
        u.data_[6] = (u.data_[6] & 0x0F) | 0x40; // version 4
        u.data_[8] = (u.data_[8] & 0x3F) | 0x80; // variant
        return u;
    }
    std::string toString() const {
        std::ostringstream ss; ss<<std::hex; auto f=[&](int i){ for(int j=0;j<i;++j){ int idx = pos_++; int v=data_[idx]; if (v<16) ss<<'0'; ss<<v; } };
        pos_=0; f(4); ss<<'-'; f(2); ss<<'-'; f(2); ss<<'-'; f(2); ss<<'-'; f(6); return ss.str();
    }
private:
    mutable int pos_=0;
    std::array<unsigned,16> data_{};
};

}} // namespace jxx::util
