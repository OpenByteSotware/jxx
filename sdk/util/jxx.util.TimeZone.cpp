#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>

#include "util/jxx.util.TimeZone.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.tzif.h"

namespace jxx::util {
::jxx::Ptr<::jxx::lang::ClassAny> TimeZone::Class() {
    return JxxClassInfoMarker::Class();
}

::jxx::lang::jbool TimeZone::hasSameRules(
    const ::jxx::Ptr<TimeZone>& other) const {
    return other != nullptr &&
           getRawOffset() == other->getRawOffset() &&
           useDaylightTime() == other->useDaylightTime();
}

using jxx::util::tz::Zone;
using jxx::util::tz::Transition;
using jxx::util::tz::load_tzif;

static std::mutex g_lock;
static std::unordered_map<std::string, Zone> g_zoneCache;

static std::string base_dir() {
    if (const char* e = std::getenv("JXX_TIME_ZONE_INFO")) {
        return std::string(e);
    }
    const std::filesystem::path standardRoot("/usr/share/zoneinfo");
    std::error_code error;
    if (std::filesystem::is_directory(standardRoot, error) && !error) {
        return standardRoot.string();
    }
    return std::string(".");
}

static inline ::jxx::lang::jint parse_gmt_offset_millis(const std::string& s) {
    if (s.size() < 4) return 0;
    std::size_t pos = 3;
    if (pos >= s.size()) return 0;
    if (s[pos] != '+' && s[pos] != '-') return 0;
    int sign = (s[pos] == '-') ? -1 : 1;
    ++pos;
    if (pos + 1 >= s.size()) return 0;
    int hh = std::stoi(s.substr(pos, 2));
    pos += 2;
    if (pos < s.size() && s[pos] == ':') ++pos;
    int mm = 0;
    if (pos + 1 < s.size()) mm = std::stoi(s.substr(pos, 2));
    return static_cast<::jxx::lang::jint>(
        sign * ((hh * 60 + mm) * 60 * 1000));
}

class FixedOffsetTimeZone final : public TimeZone {
    std::string id_;
    ::jxx::lang::jint off_ms_ = 0;

public:
    FixedOffsetTimeZone(std::string id, ::jxx::lang::jint off_ms)
        : id_(std::move(id)), off_ms_(off_ms) {}

    ::jxx::Ptr<::jxx::lang::String> getID() const override {
        return ::jxx::NEW<::jxx::lang::String>(id_.c_str());
    }

    ::jxx::lang::jint getOffset(
        ::jxx::lang::jlong /*epochMillis*/) const override {
        return off_ms_;
    }

    ::jxx::Ptr<::jxx::lang::String> getAbbreviation(
        ::jxx::lang::jlong /*epochMillis*/) const override {
        return ::jxx::NEW<::jxx::lang::String>(id_.c_str());
    }

    ::jxx::lang::jint getRawOffset() const override { return off_ms_; }
    ::jxx::lang::jbool useDaylightTime() const override { return false; }

    ::jxx::lang::jbool inDaylightTime(
        const ::jxx::Ptr<Date>& /*date*/) const override {
        return false;
    }

    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& other) const override {
        if (other.get() == this) return true;
        const auto zone = ::jxx::CAST<TimeZone>(other);
        if (zone == nullptr) return false;
        const auto otherId = zone->getID();
        return otherId != nullptr && id_ == otherId->utf8() && hasSameRules(zone);
    }

    ::jxx::lang::jint hashCode() const override {
        return getID()->hashCode();
    }
};

class TzdbTimeZone final : public TimeZone {
    Zone z_;

public:
    explicit TzdbTimeZone(Zone z) : z_(std::move(z)) {}

    ::jxx::Ptr<::jxx::lang::String> getID() const override {
        return ::jxx::NEW<::jxx::lang::String>(z_.id.c_str());
    }

    ::jxx::lang::jint getOffset(
        ::jxx::lang::jlong epochMillis) const override {
        int64_t sec = static_cast<int64_t>(epochMillis / 1000);
        const auto& transitions = z_.transitions;
        if (transitions.empty()) {
            return static_cast<::jxx::lang::jint>(z_.default_offset * 1000);
        }
        auto it = std::upper_bound(
            transitions.begin(), transitions.end(), sec,
            [](int64_t value, const Transition& transition) {
                return value < transition.at_utc;
            });
        if (it == transitions.begin()) {
            return static_cast<::jxx::lang::jint>(z_.default_offset * 1000);
        }
        --it;
        return static_cast<::jxx::lang::jint>(it->offset * 1000);
    }

    ::jxx::Ptr<::jxx::lang::String> getAbbreviation(
        ::jxx::lang::jlong epochMillis) const override {
        int64_t sec = static_cast<int64_t>(epochMillis / 1000);
        const auto& transitions = z_.transitions;
        if (transitions.empty()) {
            return ::jxx::NEW<::jxx::lang::String>(z_.default_abbrev.c_str());
        }
        auto it = std::upper_bound(
            transitions.begin(), transitions.end(), sec,
            [](int64_t value, const Transition& transition) {
                return value < transition.at_utc;
            });
        if (it == transitions.begin()) {
            return ::jxx::NEW<::jxx::lang::String>(z_.default_abbrev.c_str());
        }
        --it;
        return ::jxx::NEW<::jxx::lang::String>(it->abbrev.c_str());
    }

    ::jxx::lang::jint getRawOffset() const override {
        return static_cast<::jxx::lang::jint>(z_.default_offset * 1000);
    }

    ::jxx::lang::jbool useDaylightTime() const override {
        return true;
    }

    ::jxx::lang::jbool inDaylightTime(
        const ::jxx::Ptr<Date>& date) const override {
        if (date == nullptr) return false;
        return getOffset(date->getTime()) != getRawOffset();
    }

    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& other) const override {
        if (other.get() == this) return true;
        const auto zone = ::jxx::CAST<TimeZone>(other);
        if (zone == nullptr) return false;
        const auto otherId = zone->getID();
        return otherId != nullptr && z_.id == otherId->utf8() && hasSameRules(zone);
    }

    ::jxx::lang::jint hashCode() const override {
        return getID()->hashCode();
    }
};

static bool load_zone_file(
    const std::string& filePath,
    const std::string& zoneId,
    Zone& zone) {
    return load_tzif(filePath, zoneId, zone);
}

::jxx::Ptr<TimeZone> TimeZone::getTimeZone(
    const ::jxx::Ptr<::jxx::lang::String>& id) {
    if (id == nullptr) return getDefault();
    std::string zoneId = id->utf8();
    if (zoneId == "UTC" || zoneId == "GMT") {
        return ::jxx::NEW<FixedOffsetTimeZone>(zoneId, 0);
    }
    if (zoneId.rfind("GMT", 0) == 0 || zoneId.rfind("UTC", 0) == 0) {
        return ::jxx::NEW<FixedOffsetTimeZone>(
            zoneId, parse_gmt_offset_millis(zoneId));
    }
    std::lock_guard<std::mutex> lock(g_lock);
    auto found = g_zoneCache.find(zoneId);
    if (found != g_zoneCache.end()) {
        return ::jxx::NEW<TzdbTimeZone>(found->second);
    }
    Zone zone;
    std::string path = base_dir() + "/" + zoneId;
    if (!load_zone_file(path, zoneId, zone)) {
        return ::jxx::NEW<FixedOffsetTimeZone>("UTC", 0);
    }
    g_zoneCache.emplace(zoneId, zone);
    return ::jxx::NEW<TzdbTimeZone>(zone);
}

::jxx::Ptr<TimeZone> TimeZone::getDefault() {
    const std::string base = base_dir();
    std::lock_guard<std::mutex> lock(g_lock);
#ifdef _WIN32
    {
        std::filesystem::path path = std::filesystem::path(base) / "localtime";
        std::error_code error;
        if (std::filesystem::exists(path, error) && !error) {
            const std::string key = "@localtime";
            auto found = g_zoneCache.find(key);
            if (found != g_zoneCache.end()) {
                return ::jxx::NEW<TzdbTimeZone>(found->second);
            }
            Zone zone;
            if (load_zone_file(path.string(), "localtime", zone)) {
                g_zoneCache.emplace(key, zone);
                return ::jxx::NEW<TzdbTimeZone>(zone);
            }
        }
    }
    return ::jxx::NEW<FixedOffsetTimeZone>("UTC", 0);
#else
    std::filesystem::path etcLocal("/etc/localtime");
    std::error_code error;
    if (std::filesystem::exists(etcLocal, error) && !error) {
        if (std::filesystem::is_symlink(etcLocal, error) && !error) {
            auto target = std::filesystem::read_symlink(etcLocal, error);
            if (!error) {
                std::filesystem::path absoluteTarget = target.is_absolute()
                    ? target
                    : (etcLocal.parent_path() / target);
                absoluteTarget = std::filesystem::weakly_canonical(
                    absoluteTarget, error);
                if (!error) {
                    std::filesystem::path basePath =
                        std::filesystem::weakly_canonical(base, error);
                    if (!error) {
                        auto absoluteText = absoluteTarget.generic_string();
                        auto baseText = basePath.generic_string();
                        if (absoluteText.rfind(baseText + "/", 0) == 0) {
                            std::string zoneId = absoluteText.substr(
                                baseText.size() + 1);
                            auto found = g_zoneCache.find(zoneId);
                            if (found != g_zoneCache.end()) {
                                return ::jxx::NEW<TzdbTimeZone>(found->second);
                            }
                            Zone zone;
                            std::string path = base + "/" + zoneId;
                            if (load_zone_file(path, zoneId, zone)) {
                                g_zoneCache.emplace(zoneId, zone);
                                return ::jxx::NEW<TzdbTimeZone>(zone);
                            }
                        }
                    }
                }
            }
        }
        const std::string key = "@etc_localtime";
        auto found = g_zoneCache.find(key);
        if (found != g_zoneCache.end()) {
            return ::jxx::NEW<TzdbTimeZone>(found->second);
        }
        Zone zone;
        if (load_zone_file(etcLocal.string(), "localtime", zone)) {
            g_zoneCache.emplace(key, zone);
            return ::jxx::NEW<TzdbTimeZone>(zone);
        }
    }
    return ::jxx::NEW<FixedOffsetTimeZone>("UTC", 0);
#endif
}
} // namespace jxx::util
