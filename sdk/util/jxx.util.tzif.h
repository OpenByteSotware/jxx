#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace jxx::util::tz {

struct Transition {
    int64_t at_utc;      // UTC seconds
    int32_t offset;      // total offset seconds AFTER transition
    bool is_dst;
    std::string abbrev;
};

struct Zone {
    std::string id;
    int32_t default_offset = 0;
    bool default_is_dst = false;
    std::string default_abbrev;
    std::vector<Transition> transitions; // sorted
};

// Parse a TZif v2/v3 file (RFC 8536) and populate Zone.
bool load_tzif(const std::string& path, const std::string& zone_id, Zone& out);

} // namespace jxx::util::tz
