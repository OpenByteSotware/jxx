#include "jxx.util.tzif.h"

#include <fstream>
#include <cstring>

namespace jxx::util::tz {

static int32_t read_i32(std::ifstream& f) {
    unsigned char b[4];
    f.read(reinterpret_cast<char*>(b), 4);
    return (int32_t)((uint32_t(b[0])<<24) | (uint32_t(b[1])<<16) | (uint32_t(b[2])<<8) | uint32_t(b[3]));
}

static int64_t read_i64(std::ifstream& f) {
    unsigned char b[8];
    f.read(reinterpret_cast<char*>(b), 8);
    int64_t v = 0;
    for (int i=0;i<8;i++) v = (v<<8) | int64_t(b[i]);
    return v;
}

struct Counts { int32_t ttisgmtcnt, ttisstdcnt, leapcnt, timecnt, typecnt, charcnt; };

static Counts read_counts(std::ifstream& f) {
    Counts c;
    c.ttisgmtcnt = read_i32(f);
    c.ttisstdcnt = read_i32(f);
    c.leapcnt    = read_i32(f);
    c.timecnt    = read_i32(f);
    c.typecnt    = read_i32(f);
    c.charcnt    = read_i32(f);
    return c;
}

bool load_tzif(const std::string& path, const std::string& zone_id, Zone& out) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return false;

    char magic[4];
    f.read(magic, 4);
    if (std::strncmp(magic, "TZif", 4) != 0) return false;

    char version = '\0';
    f.read(&version, 1);
    f.ignore(15);

    Counts c1 = read_counts(f);

    // Skip v1 block
    f.ignore((std::streamsize)c1.timecnt * 4);
    f.ignore((std::streamsize)c1.timecnt);
    f.ignore((std::streamsize)c1.typecnt * 6);
    f.ignore((std::streamsize)c1.charcnt);
    f.ignore((std::streamsize)c1.leapcnt * 8);
    f.ignore((std::streamsize)c1.ttisstdcnt);
    f.ignore((std::streamsize)c1.ttisgmtcnt);

    // v2/v3 block (64-bit transition times)
    Counts c = read_counts(f);

    std::vector<int64_t> times(c.timecnt);
    for (int i=0;i<c.timecnt;i++) times[i] = read_i64(f);

    std::vector<unsigned char> indices(c.timecnt);
    if (c.timecnt) f.read(reinterpret_cast<char*>(indices.data()), c.timecnt);

    struct TT { int32_t off; unsigned char isdst; unsigned char ab; };
    std::vector<TT> tt(c.typecnt);
    for (int i=0;i<c.typecnt;i++) {
        tt[i].off = read_i32(f);
        f.read(reinterpret_cast<char*>(&tt[i].isdst), 1);
        f.read(reinterpret_cast<char*>(&tt[i].ab), 1);
    }

    std::string abbrev(c.charcnt, '\0');
    if (c.charcnt) f.read(abbrev.data(), c.charcnt);

    out = Zone{};
    out.id = zone_id;
    out.transitions.clear();
    out.transitions.reserve(times.size());

    for (int i=0;i<c.timecnt;i++) {
        unsigned char tti = indices[i];
        if (tti >= tt.size()) continue;
        Transition tr;
        tr.at_utc = times[i];
        tr.offset = tt[tti].off;
        tr.is_dst = tt[tti].isdst != 0;
        tr.abbrev = &abbrev[tt[tti].ab];
        out.transitions.push_back(std::move(tr));
    }

    // Default type: first non-DST or type 0
    bool found = false;
    for (int i=0;i<c.typecnt;i++) {
        if (tt[i].isdst == 0) {
            out.default_offset = tt[i].off;
            out.default_is_dst = false;
            out.default_abbrev = &abbrev[tt[i].ab];
            found = true;
            break;
        }
    }
    if (!found && c.typecnt > 0) {
        out.default_offset = tt[0].off;
        out.default_is_dst = tt[0].isdst != 0;
        out.default_abbrev = &abbrev[tt[0].ab];
    }

    return true;
}

} // namespace jxx::util::tz
