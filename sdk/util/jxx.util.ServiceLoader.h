#pragma once

#include <cctype>
#include <set>
#include <string>
#include <vector>

#include "io/jxx.io.InputStream.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassLoader.h"
#include "lang/jxx.lang.Iterable.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx.lang.Throwable.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.ServiceConfigurationError.h"

namespace jxx::util {

template <typename S>
class ServiceLoader final
    : public ::jxx::lang::ClassBase<
          ServiceLoader<S>,
          ::jxx::lang::Object,
          ::jxx::lang::Iterable<S>> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        ServiceLoader<S>, JxxSuper, ::jxx::lang::Iterable<S>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

private:
    ::jxx::Ptr<::jxx::lang::ClassAny> service_;
    ::jxx::Ptr<::jxx::lang::ClassLoader> loader_;
    std::vector<::jxx::Ptr<S>> providers_;
    std::vector<std::string> providerNames_;
    ::jxx::lang::jbool configurationsLoaded_ = false;
    std::size_t generation_ = 0U;

    static std::string trim_(const std::string& value) {
        std::size_t first = 0U;
        while (first < value.size() &&
               std::isspace(static_cast<unsigned char>(value[first]))) ++first;
        std::size_t last = value.size();
        while (last > first &&
               std::isspace(static_cast<unsigned char>(value[last - 1U]))) --last;
        return value.substr(first, last - first);
    }

    static ::jxx::lang::jbool validProviderName_(const std::string& name) {
        if (name.empty() || name.front() == '.' || name.back() == '.') return false;
        ::jxx::lang::jbool segmentStart = true;
        for (unsigned char ch : name) {
            if (ch == '.') {
                if (segmentStart) return false;
                segmentStart = true;
                continue;
            }
            if (segmentStart) {
                if (!(std::isalpha(ch) || ch == '_' || ch == '$')) return false;
                segmentStart = false;
            } else if (!(std::isalnum(ch) || ch == '_' || ch == '$')) {
                return false;
            }
        }
        return !segmentStart;
    }

    [[noreturn]] void fail_(const std::string& message) const {
        throw ServiceConfigurationError(
            ::jxx::NEW<::jxx::lang::String>(message));
    }

    void parse_(const ::jxx::Ptr<::jxx::io::InputStream>& input) {
        if (input == nullptr) return;
        std::string contents;
        try {
            for (;;) {
                const auto value = input->read();
                if (value < 0) break;
                contents.push_back(static_cast<char>(value & 0xff));
            }
            input->close();
        } catch (...) {
            try { input->close(); } catch (...) {}
            fail_("Error reading service configuration file");
        }

        std::set<std::string> known(providerNames_.begin(), providerNames_.end());
        std::size_t position = 0U;
        while (position <= contents.size()) {
            const auto end = contents.find('\n', position);
            auto line = contents.substr(position,
                end == std::string::npos ? std::string::npos : end - position);
            if (!line.empty() && line.back() == '\r') line.pop_back();
            const auto comment = line.find('#');
            if (comment != std::string::npos) line.erase(comment);
            line = trim_(line);
            if (!line.empty()) {
                if (!validProviderName_(line)) {
                    fail_("Illegal provider-class name: " + line);
                }
                if (known.insert(line).second) providerNames_.push_back(line);
            }
            if (end == std::string::npos) break;
            position = end + 1U;
        }
    }

    void loadConfigurations_() {
        if (configurationsLoaded_) return;
        configurationsLoaded_ = true;
        const auto serviceName = service_->getName();
        const auto resourceName = ::jxx::NEW<::jxx::lang::String>(
            std::string("META-INF/services/") + serviceName->utf8());
        try {
            const auto resources = loader_->getResources(resourceName);
            while (resources != nullptr && resources->hasMoreElements()) {
                const auto url = resources->nextElement();
                if (url != nullptr) parse_(url->openStream());
            }
        } catch (const ServiceConfigurationError&) {
            throw;
        } catch (...) {
            fail_("Error locating service configuration files");
        }
    }

    ::jxx::Ptr<S> instantiate_(std::size_t index) {
        while (providers_.size() <= index) providers_.push_back(nullptr);
        if (providers_[index] != nullptr) return providers_[index];
        const auto& providerName = providerNames_[index];
        try {
            const auto providerClass = loader_->loadClass(
                ::jxx::NEW<::jxx::lang::String>(providerName));
            if (providerClass == nullptr ||
                !service_->isAssignableFrom(providerClass)) {
                fail_("Provider " + providerName + " not a subtype");
            }
            const auto object = providerClass->newInstance();
            const auto provider = ::jxx::CAST<S>(object);
            if (provider == nullptr) {
                fail_("Provider " + providerName + " could not be instantiated");
            }
            providers_[index] = provider;
            return provider;
        } catch (const ServiceConfigurationError&) {
            throw;
        } catch (...) {
            fail_("Provider " + providerName + " could not be instantiated");
        }
    }

    class ServiceIterator final
        : public ::jxx::lang::ClassBase<
              ServiceIterator, ::jxx::lang::Object, Iterator<S>> {
        ::jxx::Ptr<ServiceLoader<S>> owner_;
        std::size_t index_ = 0U;
        std::size_t generation_ = 0U;

        void checkGeneration_() const {
            if (generation_ != owner_->generation_) {
                owner_->fail_("ServiceLoader reloaded during iteration");
            }
        }

    public:
        explicit ServiceIterator(const ::jxx::Ptr<ServiceLoader<S>>& owner)
            : owner_(owner), generation_(owner->generation_) {}

        ::jxx::lang::jbool hasNext() override {
            checkGeneration_();
            owner_->loadConfigurations_();
            return index_ < owner_->providerNames_.size();
        }

        ::jxx::Ptr<S> next() override {
            if (!hasNext()) throw NoSuchElementException();
            return owner_->instantiate_(index_++);
        }

        void remove() override {
            throw ::jxx::lang::UnsupportedOperationException();
        }
    };

public:
    ServiceLoader(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& service,
        const ::jxx::Ptr<::jxx::lang::ClassLoader>& loader)
        : service_(service), loader_(loader) {
        if (service_ == nullptr) throw ::jxx::lang::NullPointerException();
        if (loader_ == nullptr) {
            loader_ = ::jxx::lang::ClassLoader::getSystemClassLoader();
        }
        if (loader_ == nullptr) throw ::jxx::lang::NullPointerException();
    }

    static ::jxx::Ptr<ServiceLoader<S>> load(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& service,
        const ::jxx::Ptr<::jxx::lang::ClassLoader>& loader) {
        return ::jxx::NEW<ServiceLoader<S>>(service, loader);
    }

    static ::jxx::Ptr<ServiceLoader<S>> load(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& service) {
        auto loader = ::jxx::lang::Thread::currentThread()->getContextClassLoader();
        if (loader == nullptr) loader = ::jxx::lang::ClassLoader::getSystemClassLoader();
        return load(service, loader);
    }

    static ::jxx::Ptr<ServiceLoader<S>> loadInstalled(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& service) {
        return load(service, ::jxx::lang::ClassLoader::getSystemClassLoader());
    }

    void reload() {
        providers_.clear();
        providerNames_.clear();
        configurationsLoaded_ = false;
        ++generation_;
    }

    ::jxx::Ptr<Iterator<S>> iterator() override {
        return ::jxx::NEW<ServiceIterator>(
            ::jxx::CAST<ServiceLoader<S>>(this->thisPtr()));
    }

    ::jxx::Ptr<::jxx::lang::String> toString() const override {
        return ::jxx::NEW<::jxx::lang::String>(
            std::string("java.util.ServiceLoader[") +
            service_->getName()->utf8() + "]");
    }
};

} // namespace jxx::util
