#include "security/jxx.security.SecureString.h"

#include <cstring>
#include <utility>

namespace jxx::security {

SecureString::SecureString() = default;
SecureString::SecureString(const char* data, std::size_t length) { assign(data, length); }
SecureString::SecureString(const std::string& value) { assign(value); }
SecureString::SecureString(SecureString&& other) noexcept { move_from(std::move(other)); }
SecureString& SecureString::operator=(SecureString&& other) noexcept {
    if (this != &other) {
        secure_wipe();
        move_from(std::move(other));
    }
    return *this;
}
SecureString::~SecureString() { secure_wipe(); }

void SecureString::assign(const char* dataValue, std::size_t length) {
    secure_wipe();
    buffer_.resize(length + 1);
    if (length != 0) {
        std::memcpy(buffer_.data(), dataValue, length);
    }
    buffer_[length] = '\0';
}
void SecureString::assign(const std::string& value) { assign(value.data(), value.size()); }
void SecureString::clear() { secure_wipe(); }
void SecureString::push_back(char value) {
    const auto currentSize = size();
    buffer_.insert(buffer_.begin() + static_cast<std::ptrdiff_t>(currentSize), value);
    if (buffer_.empty() || buffer_.back() != '\0') {
        buffer_.push_back('\0');
    } else {
        buffer_.back() = '\0';
    }
}
std::size_t SecureString::size() const noexcept { return buffer_.empty() ? 0 : buffer_.size() - 1; }
bool SecureString::empty() const noexcept { return size() == 0; }
const char* SecureString::c_str() const noexcept { return buffer_.empty() ? "" : buffer_.data(); }
char* SecureString::data() noexcept { return buffer_.empty() ? nullptr : buffer_.data(); }
const char* SecureString::data() const noexcept { return buffer_.empty() ? nullptr : buffer_.data(); }
void SecureString::secure_wipe() noexcept {
    if (buffer_.empty()) return;
    volatile char* value = reinterpret_cast<volatile char*>(buffer_.data());
    for (std::size_t index = 0; index < buffer_.size(); ++index) value[index] = 0;
    buffer_.clear();
    buffer_.shrink_to_fit();
}
void SecureString::move_from(SecureString&& other) noexcept {
    buffer_ = std::move(other.buffer_);
    other.secure_wipe();
}

} // namespace jxx::security
