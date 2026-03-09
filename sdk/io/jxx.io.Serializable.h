#pragma once
#ifndef __JXX_SERIALIZABLE_H__
#define __JXX_SERIALIZABLE_H__

#include <cstdint>
#include <cstring>
#include <functional>
#include <istream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

// -------------------- Serialization Errors --------------------
namespace jxx {
    namespace io {
        struct SerializationError : std::runtime_error {
            using std::runtime_error::runtime_error;
        };

        // -------------------- Serializable Interface ------------------
        class Serializable {
        public:
            virtual ~Serializable() = default;

            // Java-like metadata
            //virtual const char* className() const noexcept = 0;
            //virtual std::uint64_t serialVersionUID() const noexcept = 0;

            // Java-like custom serialization hooks
            virtual void writeObject(class ObjectOutputStream& out) const = 0;
            virtual void readObject(class ObjectInputStream& in, std::uint64_t storedUid) = 0;
        };

        // -------------------- Registry (name -> factory) --------------
        class SerializableRegistry {
        public:
            using Factory = std::unique_ptr<Serializable>(*)();
            struct Entry { Factory factory; std::uint64_t uid; };

        private:
            std::unordered_map<std::string, Entry> map_;

        public:
            static SerializableRegistry& instance() {
                static SerializableRegistry inst;
                return inst;
            }

            void registerClass(std::string name, std::uint64_t uid, Factory f) {
                map_.emplace(std::move(name), Entry{ f, uid });
            }

            const Entry* find(const std::string& name) const {
                auto it = map_.find(name);
                return (it == map_.end()) ? nullptr : &it->second;
            }
        };

        template <typename T>
        struct Registrar {
            Registrar(const char* name, std::uint64_t uid) {
                SerializableRegistry::instance().registerClass(
                    name, uid, [] { return std::make_unique<T>(); }
                );
            }
        };

        // -------------------- Wire Format Helpers ---------------------
        namespace detail {

            // We choose a simple little-endian wire format for primitives.
            template <typename T>
            inline void writePodLE(std::ostream& os, T v) {
                static_assert(std::is_trivially_copyable<T>::value, "POD only");
                if constexpr (sizeof(T) == 1) {
                    os.put(static_cast<char>(v));
                }
                else if constexpr (sizeof(T) == 2) {
                    std::uint16_t x; std::memcpy(&x, &v, 2);
                    std::uint16_t y = (x >> 8) | (x << 8);
                    os.write(reinterpret_cast<const char*>(&y), 2);
                }
                else if constexpr (sizeof(T) == 4) {
                    std::uint32_t x; std::memcpy(&x, &v, 4);
                    std::uint32_t y = ((x & 0x000000FFu) << 24) |
                        ((x & 0x0000FF00u) << 8) |
                        ((x & 0x00FF0000u) >> 8) |
                        ((x & 0xFF000000u) >> 24);
                    os.write(reinterpret_cast<const char*>(&y), 4);
                }
                else if constexpr (sizeof(T) == 8) {
                    std::uint64_t x; std::memcpy(&x, &v, 8);
                    std::uint64_t y =
                        ((x & 0x00000000000000FFull) << 56) |
                        ((x & 0x000000000000FF00ull) << 40) |
                        ((x & 0x0000000000FF0000ull) << 24) |
                        ((x & 0x00000000FF000000ull) << 8) |
                        ((x & 0x000000FF00000000ull) >> 8) |
                        ((x & 0x0000FF0000000000ull) >> 24) |
                        ((x & 0x00FF000000000000ull) >> 40) |
                        ((x & 0xFF00000000000000ull) >> 56);
                    os.write(reinterpret_cast<const char*>(&y), 8);
                }
                else {
                    static_assert(!sizeof(T*), "Unsupported size");
                }
                if (!os) throw SerializationError("Write failure");
            }

            template <typename T>
            inline T readPodLE(std::istream& is) {
                static_assert(std::is_trivially_copyable<T>::value, "POD only");
                T v{};
                if constexpr (sizeof(T) == 1) {
                    int ch = is.get();
                    if (ch == std::char_traits<char>::eof()) throw SerializationError("Unexpected EOF");
                    v = static_cast<T>(static_cast<unsigned char>(ch));
                }
                else if constexpr (sizeof(T) == 2) {
                    std::uint16_t y{};
                    is.read(reinterpret_cast<char*>(&y), 2);
                    if (!is) throw SerializationError("Unexpected EOF");
                    std::uint16_t x = (y >> 8) | (y << 8);
                    std::memcpy(&v, &x, 2);
                }
                else if constexpr (sizeof(T) == 4) {
                    std::uint32_t y{};
                    is.read(reinterpret_cast<char*>(&y), 4);
                    if (!is) throw SerializationError("Unexpected EOF");
                    std::uint32_t x = ((y & 0x000000FFu) << 24) |
                        ((y & 0x0000FF00u) << 8) |
                        ((y & 0x00FF0000u) >> 8) |
                        ((y & 0xFF000000u) >> 24);
                    std::memcpy(&v, &x, 4);
                }
                else if constexpr (sizeof(T) == 8) {
                    std::uint64_t y{};
                    is.read(reinterpret_cast<char*>(&y), 8);
                    if (!is) throw SerializationError("Unexpected EOF");
                    std::uint64_t x =
                        ((y & 0x00000000000000FFull) << 56) |
                        ((y & 0x000000000000FF00ull) << 40) |
                        ((y & 0x0000000000FF0000ull) << 24) |
                        ((y & 0x00000000FF000000ull) << 8) |
                        ((y & 0x000000FF00000000ull) >> 8) |
                        ((y & 0x0000FF0000000000ull) >> 24) |
                        ((y & 0x00FF000000000000ull) >> 40) |
                        ((y & 0xFF00000000000000ull) >> 56);
                    std::memcpy(&v, &x, 8);
                }
                else {
                    static_assert(!sizeof(T*), "Unsupported size");
                }
                return v;
            }

            inline void writeString(std::ostream& os, const std::string& s) {
                writePodLE<std::uint32_t>(os, static_cast<std::uint32_t>(s.size()));
                os.write(s.data(), static_cast<std::streamsize>(s.size()));
                if (!os) throw SerializationError("Write failure (string)");
            }

            inline std::string readString(std::istream& is) {
                std::uint32_t n = readPodLE<std::uint32_t>(is);
                std::string s; s.resize(n);
                is.read(&s[0], static_cast<std::streamsize>(n));
                if (!is) throw SerializationError("Unexpected EOF (string)");
                return s;
            }

        } // namespace detail

        // -------------------- Tags & Stream Header --------------------
        enum class Tag : std::uint8_t { Null = 0, Ref = 1, New = 2 };

        // Simple magic/version to detect corruption and format version
        static constexpr std::uint32_t STREAM_MAGIC = 0x4A58534Du; // "JXSM"
        static constexpr std::uint16_t STREAM_VERSION = 0x0001u;

        // -------------------- ObjectOutputStream ----------------------
        class ObjectOutputStream {
            std::ostream& os_;
            std::unordered_map<const void*, std::uint32_t> ids_;
            std::uint32_t nextId_{ 1 };

        public:
            explicit ObjectOutputStream(std::ostream& os) : os_(os) {
                // header
                detail::writePodLE<std::uint32_t>(os_, STREAM_MAGIC);
                detail::writePodLE<std::uint16_t>(os_, STREAM_VERSION);
            }

            // Primitive writers
            void writeBool(bool b) { detail::writePodLE<std::uint8_t>(os_, b ? 1u : 0u); }
            void writeI32(std::int32_t v) { detail::writePodLE<std::int32_t>(os_, v); }
            void writeU32(std::uint32_t v) { detail::writePodLE<std::uint32_t>(os_, v); }
            void writeI64(std::int64_t v) { detail::writePodLE<std::int64_t>(os_, v); }
            void writeU64(std::uint64_t v) { detail::writePodLE<std::uint64_t>(os_, v); }
            void writeF64(double v) { detail::writePodLE<double>(os_, v); }
            void writeString(const std::string& s) { detail::writeString(os_, s); }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
            void write(const T& v) {
                if constexpr (std::is_same<T, bool>::value) writeBool(v);
                else if constexpr (std::is_same<T, double>::value) writeF64(v);
                else if constexpr (std::is_signed<T>::value && sizeof(T) == 4) writeI32(static_cast<std::int32_t>(v));
                else if constexpr (std::is_unsigned<T>::value && sizeof(T) == 4) writeU32(static_cast<std::uint32_t>(v));
                else if constexpr (std::is_signed<T>::value && sizeof(T) == 8) writeI64(static_cast<std::int64_t>(v));
                else if constexpr (std::is_unsigned<T>::value && sizeof(T) == 8) writeU64(static_cast<std::uint64_t>(v));
                else if constexpr (std::is_unsigned<T>::value && sizeof(T) == 1) detail::writePodLE<std::uint8_t>(os_, static_cast<std::uint8_t>(v));
                else static_assert(!sizeof(T*), "Unsupported arithmetic type size");
            }

            void write(const std::string& s) { writeString(s); }

            template <typename T>
            void writeVector(const std::vector<T>& v) {
                writeU32(static_cast<std::uint32_t>(v.size()));
                for (const auto& e : v) write(e);
            }

            // Core API: write a shared Serializable (preserves null, refs)
            void writeObject(const std::shared_ptr<Serializable>& p) {
                if (!p) {
                    detail::writePodLE<std::uint8_t>(os_, static_cast<std::uint8_t>(Tag::Null));
                    return;
                }
                auto it = ids_.find(p.get());
                if (it != ids_.end()) {
                    detail::writePodLE<std::uint8_t>(os_, static_cast<std::uint8_t>(Tag::Ref));
                    writeU32(it->second);
                    return;
                }
                // new object
                detail::writePodLE<std::uint8_t>(os_, static_cast<std::uint8_t>(Tag::New));
                std::uint32_t id = nextId_++;
                ids_.emplace(p.get(), id);
                writeU32(id);

                // class header
                //writeString(p->className());
                //writeU64(p->serialVersionUID());

                // body
                p->writeObject(*this);
            }
        };

        // -------------------- ObjectInputStream -----------------------
        class ObjectInputStream {
            std::istream& is_;
            std::unordered_map<std::uint32_t, std::shared_ptr<Serializable>> objects_;

        public:
            explicit ObjectInputStream(std::istream& is) : is_(is) {
                // header
                std::uint32_t magic = detail::readPodLE<std::uint32_t>(is_);
                std::uint16_t ver = detail::readPodLE<std::uint16_t>(is_);
                if (magic != STREAM_MAGIC) throw SerializationError("Bad stream magic");
                if (ver != STREAM_VERSION) throw SerializationError("Unsupported stream version");
            }

            // Primitive readers
            bool        readBool() { return detail::readPodLE<std::uint8_t>(is_) != 0; }
            std::int32_t  readI32() { return detail::readPodLE<std::int32_t>(is_); }
            std::uint32_t readU32() { return detail::readPodLE<std::uint32_t>(is_); }
            std::int64_t  readI64() { return detail::readPodLE<std::int64_t>(is_); }
            std::uint64_t readU64() { return detail::readPodLE<std::uint64_t>(is_); }
            double      readF64() { return detail::readPodLE<double>(is_); }
            std::string readString() { return detail::readString(is_); }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
            T read() {
                if constexpr (std::is_same<T, bool>::value) return readBool();
                else if constexpr (std::is_same<T, double>::value) return readF64();
                else if constexpr (std::is_signed<T>::value && sizeof(T) == 4) return static_cast<T>(readI32());
                else if constexpr (std::is_unsigned<T>::value && sizeof(T) == 4) return static_cast<T>(readU32());
                else if constexpr (std::is_signed<T>::value && sizeof(T) == 8) return static_cast<T>(readI64());
                else if constexpr (std::is_unsigned<T>::value && sizeof(T) == 8) return static_cast<T>(readU64());
                else if constexpr (std::is_unsigned<T>::value && sizeof(T) == 1) return static_cast<T>(detail::readPodLE<std::uint8_t>(is_));
                else { static_assert(!sizeof(T*), "Unsupported arithmetic type size"); return T{}; }
            }

            std::string read() { return readString(); }

            template <typename T>
            std::vector<T> readVector() {
                std::uint32_t n = readU32();
                std::vector<T> v; v.reserve(n);
                for (std::uint32_t i = 0; i < n; ++i) v.push_back(read<T>());
                return v;
            }

            // Core API: read a shared Serializable (preserves null, refs, cycles)
            std::shared_ptr<Serializable> readObject() {
                Tag tag = static_cast<Tag>(detail::readPodLE<std::uint8_t>(is_));
                if (tag == Tag::Null) return nullptr;
                if (tag == Tag::Ref) {
                    std::uint32_t id = readU32();
                    auto it = objects_.find(id);
                    if (it == objects_.end()) throw SerializationError("Dangling back-reference");
                    return it->second;
                }
                if (tag == Tag::New) {
                    std::uint32_t id = readU32();
                    std::string cname = readString();
                    std::uint64_t uid = readU64();

                    const auto* e = SerializableRegistry::instance().find(cname);
                    if (!e || !e->factory) throw SerializationError("Unknown class: " + cname);

                    std::shared_ptr<Serializable> obj(e->factory().release());
                    // Insert before reading to support cycles
                    objects_.emplace(id, obj);

                    // Migrate/validate using stored UID
                    obj->readObject(*this, uid);
                    return obj;
                }
                throw SerializationError("Invalid tag");
            }
        };

        // -------------------- Convenience Macros ----------------------
#define JXX_SERIALIZABLE(CLASS, NAME, UID64)                                      \
public:                                                                            \
    static constexpr const char* kClassName = NAME;                                \
    static constexpr std::uint64_t kSerialVersionUID = (UID64);                    \
    const char* className() const noexcept override { return kClassName; }         \
    std::uint64_t serialVersionUID() const noexcept override { return kSerialVersionUID; } \
private:                                                                           \
    static ::jxx::Registrar<CLASS> s_registrar_;                                   \
public:

#define JXX_SERIALIZABLE_REGISTER(CLASS)                                           \
    ::jxx::Registrar<CLASS> CLASS::s_registrar_{ CLASS::kClassName, CLASS::kSerialVersionUID };
    } // namespace io
} // namespace jxx
#endif