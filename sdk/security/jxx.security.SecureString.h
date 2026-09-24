#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "lang/jxx.lang.Object.h"

namespace jxx::security {

class SecureString : public jxx::lang::Object {
public:
    SecureString();
    SecureString(const char* data, std::size_t length);
    explicit SecureString(const std::string& value);
    SecureString(const SecureString&) = delete;
    SecureString& operator=(const SecureString&) = delete;
    SecureString(SecureString&& other) noexcept;
    SecureString& operator=(SecureString&& other) noexcept;
    ~SecureString() override;

    void assign(const char* data, std::size_t length);
    void assign(const std::string& value);
    void clear();
    void push_back(char value);
    std::size_t size() const noexcept;
    bool empty() const noexcept;
    const char* c_str() const noexcept;
    char* data() noexcept;
    const char* data() const noexcept;
    void secure_wipe() noexcept;

private:
    void move_from(SecureString&& other) noexcept;
    std::vector<char> buffer_;
};

} // namespace jxx::security
