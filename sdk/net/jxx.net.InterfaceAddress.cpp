#include <cstdint>
#include <string>
#include <utility>

#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InterfaceAddress.h"

namespace jxx::net {

    InterfaceAddress::InterfaceAddress(
        const jxx::Ptr<InetAddress>& address,
        const jxx::Ptr<InetAddress>& broadcast,
        jxx::lang::jshort prefixLength)
        : address_(address),
          broadcast_(broadcast),
          prefixLength_(prefixLength) {
    }

    jxx::Ptr<InetAddress>
        InterfaceAddress::getAddress() const {
        return address_;
    }

    jxx::Ptr<InetAddress>
        InterfaceAddress::getBroadcast() const {
        return broadcast_;
    }

    jxx::lang::jshort
        InterfaceAddress::getNetworkPrefixLength() const noexcept {
        return prefixLength_;
    }

    jxx::Ptr<jxx::lang::String>
        InterfaceAddress::toString() const {
        std::string value =
            address_ != nullptr &&
            address_->getHostAddress() != nullptr
            ? address_->getHostAddress()->utf8()
            : std::string();

        value += "/";
        value += std::to_string(prefixLength_);

        if (broadcast_ != nullptr &&
            broadcast_->getHostAddress() != nullptr) {

            value += " [";
            value += broadcast_->getHostAddress()->utf8();
            value += "]";
        }

        return jxx::NEW<jxx::lang::String>(value);
    }

    jxx::lang::jbool
        InterfaceAddress::equals(
            const jxx::Ptr<jxx::lang::Object>& other) const {

        const auto value =
            std::dynamic_pointer_cast<InterfaceAddress>(other);

        if (value == nullptr) {
            return false;
        }

        const auto addressesEqual =
            address_ == nullptr
            ? value->address_ == nullptr
            : address_->equals(value->address_);

        const auto broadcastsEqual =
            broadcast_ == nullptr
            ? value->broadcast_ == nullptr
            : broadcast_->equals(value->broadcast_);

        return static_cast<jxx::lang::jbool>(
            addressesEqual &&
            broadcastsEqual &&
            prefixLength_ == value->prefixLength_);
    }

    jxx::lang::jint
        InterfaceAddress::hashCode() const {
        jxx::lang::jint result =
            address_ != nullptr
            ? address_->hashCode()
            : 0;

        result =
            31 * result +
            (broadcast_ != nullptr
                ? broadcast_->hashCode()
                : 0);

        result =
            31 * result +
            static_cast<jxx::lang::jint>(
                prefixLength_);

        return result;
    }

} // namespace jxx::net
