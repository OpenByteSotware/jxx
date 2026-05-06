#include "jxx.util.TimeZone.h"
#include "jxx.util.Date.h"
#include "jxx.util.tz.tzif.h"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <mutex>
#include <string>
#include <unordered_map>

namespace jxx::util {

using jxx::util::tz::Zone;
using jxx::util::tz::Transition;
using jxx::util::tz::load_tzif;

static std::mutex g_lock;
static std::unordered_map<std::string, Zone> g_zoneCache;

static std::string base_dir() {
    if (const char* e = std::getenv("JXX_TIME_ZONE_INFO")) {
        return std::string(e);
    }
#ifdef __linux__
    return std::string("/usr/share/zoneinfo");
#else
    return std::string(".");
#endif
}

static inline jint parse_gmt_offset_millis(const std::string& s) {
    // Accept GMT+hh:mm, GMT-hh:mm, UTC+..., UTC-...
    if (s.size() < 4) return 0;
    std::size_t pos = 3;
    if (pos >= s.size()) return 0;
    if (s[pos] != '+' && s[pos] != '-') return 0;
    int sign = (s[pos] == '-') ? -1 : 1;
    ++pos;
    if (pos + 1 >= s.size()) return 0;
    int hh = std::stoi(s.substr(pos,2));
    pos += 2;
    if (pos < s.size() && s[pos] == ':') ++pos;
    int mm = 0;
    if (pos + 1 < s.size()) mm = std::stoi(s.substr(pos,2));
    return (jint)(sign * ((hh*60 + mm) * 60 * 1000));
}

class FixedOffsetTimeZone final : public TimeZone {
    std::string id_;
    jint off_ms_ = 0;
public:
    FixedOffsetTimeZone(std::string id, jint off_ms) : id_(std::move(id)), off_ms_(off_ms) {}
    jxx::Ptr<jxx::lang::String> getID() const override { return std::make_shared<jxx::lang::String>(id_.c_str()); }
    jint getOffset(jlong /*epochMillis*/) const override { return off_ms_; }
    jxx::Ptr<jxx::lang::String> getAbbreviation(jlong /*epochMillis*/) const override {
        // Java often uses GMT+hh:mm for fixed offsets; keep ID.
        return std::make_shared<jxx::lang::String>(id_.c_str());
    }
    jint getRawOffset() const override { return off_ms_; }
    jbool useDaylightTime() const override { return false; }
    jbool inDaylightTime(jxx::Ptr<Date> /*d*/) const override { return false; }
};

class TzdbTimeZone final : public TimeZone {
    Zone z_;
public:
    explicit TzdbTimeZone(Zone z) : z_(std::move(z)) {}

    jxx::Ptr<jxx::lang::String> getID() const override {
        return std::make_shared<jxx::lang::String>(z_.id.c_str());
    }

    jint getOffset(jlong epochMillis) const override {
        int64_t sec = (int64_t)(epochMillis / 1000);
        const auto& t = z_.transitions;
        if (t.empty()) return (jint)(z_.default_offset * 1000);
        auto it = std::upper_bound(t.begin(), t.end(), sec,
            [](int64_t v, const Transition& tr){ return v < tr.at_utc; });
        if (it == t.begin()) return (jint)(z_.default_offset * 1000);
        --it;
        return (jint)(it->offset * 1000);
    }

    jxx::Ptr<jxx::lang::String> getAbbreviation(jlong epochMillis) const override {
        int64_t sec = (int64_t)(epochMillis / 1000);
        const auto& t = z_.transitions;
        if (t.empty()) return std::make_shared<jxx::lang::String>(z_.default_abbrev.c_str());
        auto it = std::upper_bound(t.begin(), t.end(), sec,
            [](int64_t v, const Transition& tr){ return v < tr.at_utc; });
        if (it == t.begin()) return std::make_shared<jxx::lang::String>(z_.default_abbrev.c_str());
        --it;
        return std::make_shared<jxx::lang::String>(it->abbrev.c_str());
    }

    jint getRawOffset() const override { return (jint)(z_.default_offset * 1000); }
    jbool useDaylightTime() const override { return true; }

    jbool inDaylightTime(jxx::Ptr<Date> d) const override {
        if (!d) return false;
        jint off = getOffset(d->getTime());
        return off != getRawOffset();
    }
};

static bool load_zone_file(const std::string& filePath, const std::string& zoneId, Zone& z) {
    return load_tzif(filePath, zoneId, z);
}

jxx::Ptr<TimeZone> TimeZone::getTimeZone(jxx::Ptr<jxx::lang::String> id) {
    if (!id) return getDefault();
    std::string zid = id->utf8();

    if (zid == "UTC" || zid == "GMT") return std::make_shared<FixedOffsetTimeZone>(zid, 0);

    if (zid.rfind("GMT", 0) == 0 || zid.rfind("UTC", 0) == 0) {
        jint off = parse_gmt_offset_millis(zid);
        return std::make_shared<FixedOffsetTimeZone>(zid, off);
    }

    std::lock_guard<std::mutex> lk(g_lock);
    auto it = g_zoneCache.find(zid);
    if (it != g_zoneCache.end()) return std::make_shared<TzdbTimeZone>(it->second);

    Zone z;
    std::string path = base_dir() + "/" + zid;
    if (!load_zone_file(path, zid, z)) {
        return std::make_shared<FixedOffsetTimeZone>("UTC", 0);
    }

    g_zoneCache.emplace(zid, z);
    return std::make_shared<TzdbTimeZone>(z);
}

jxx::Ptr<TimeZone> TimeZone::getDefault() {
    const std::string base = base_dir();

    std::lock_guard<std::mutex> lk(g_lock);

#ifdef _WIN32
    // Windows option 2: load ${JXX_TIME_ZONE_INFO}/localtime if present
    {
        std::filesystem::path p = std::filesystem::path(base) / "localtime";
        std::error_code ec;
        if (std::filesystem::exists(p, ec) && !ec) {
            const std::string key = "@localtime";
            auto it = g_zoneCache.find(key);
            if (it != g_zoneCache.end()) return std::make_shared<TzdbTimeZone>(it->second);
            Zone z;
            if (load_zone_file(p.string(), "localtime", z)) {
                g_zoneCache.emplace(key, z);
                return std::make_shared<TzdbTimeZone>(z);
            }
        }
    }
    return std::make_shared<FixedOffsetTimeZone>("UTC", 0);
#else
    std::filesystem::path etcLocal("/etc/localtime");
    std::error_code ec;

    if (std::filesystem::exists(etcLocal, ec) && !ec) {
        if (std::filesystem::is_symlink(etcLocal, ec) && !ec) {
            auto target = std::filesystem::read_symlink(etcLocal, ec);
            if (!ec) {
                std::filesystem::path absTarget = target.is_absolute() ? target : (etcLocal.parent_path() / target);
                absTarget = std::filesystem::weakly_canonical(absTarget, ec);
                if (!ec) {
                    std::filesystem::path basePath = std::filesystem::weakly_canonical(base, ec);
                    if (!ec) {
                        auto absStr = absTarget.generic_string();
                        auto baseStr = basePath.generic_string();
                        if (absStr.rfind(baseStr + "/", 0) == 0) {
                            std::string zoneId = absStr.substr(baseStr.size() + 1);
                            auto it2 = g_zoneCache.find(zoneId);
                            if (it2 != g_zoneCache.end()) return std::make_shared<TzdbTimeZone>(it2->second);
                            Zone z;
                            std::string p = base + "/" + zoneId;
                            if (load_zone_file(p, zoneId, z)) {
                                g_zoneCache.emplace(zoneId, z);
                                return std::make_shared<TzdbTimeZone>(z);
                            }
                        }
                    }
                }
            }
        }

        // Load /etc/localtime as TZif
        {
            const std::string key = "@etc_localtime";
            auto it = g_zoneCache.find(key);
            if (it != g_zoneCache.end()) return std::make_shared<TzdbTimeZone>(it->second);
            Zone z;
            if (load_zone_file(etcLocal.string(), "localtime", z)) {
                g_zoneCache.emplace(key, z);
                return std::make_shared<TzdbTimeZone>(z);
            }
        }
    }

    return std::make_shared<FixedOffsetTimeZone>("UTC", 0);
#endif
}

} // namespace jxx::util
