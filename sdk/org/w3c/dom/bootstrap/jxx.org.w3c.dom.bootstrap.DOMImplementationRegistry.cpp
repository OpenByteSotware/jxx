#include "org/w3c/dom/bootstrap/jxx.org.w3c.dom.bootstrap.DOMImplementationRegistry.h"

#include <cstddef>
#include <utility>
#include <vector>

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationSourceImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementation.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementationList.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementationSource.h"

namespace jxx::org::w3c::dom::bootstrap {

namespace {

class RegistryImplementationList final
    : public ::jxx::lang::ClassBase<
          RegistryImplementationList,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::DOMImplementationList> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        RegistryImplementationList,
        JxxSuper,
        ::jxx::org::w3c::dom::DOMImplementationList>;

    explicit RegistryImplementationList(
        std::vector<
            ::jxx::Ptr<
                ::jxx::org::w3c::dom::DOMImplementation>> values)
        : Super()
        , values_(std::move(values)) {
    }

    ::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementation> item(
        ::jxx::lang::jint index) const override {
        if (index < 0 ||
            static_cast<std::size_t>(index) >= values_.size()) {
            return nullptr;
        }

        return values_[static_cast<std::size_t>(index)];
    }

    ::jxx::lang::jint getLength() const override {
        return static_cast<::jxx::lang::jint>(values_.size());
    }

private:
    std::vector<
        ::jxx::Ptr<
            ::jxx::org::w3c::dom::DOMImplementation>> values_;
};

} // namespace

struct DOMImplementationRegistry::Impl {
    std::vector<
        ::jxx::Ptr<
            ::jxx::org::w3c::dom::DOMImplementationSource>> sources;
};

::jxx::Ptr<::jxx::lang::String>
DOMImplementationRegistry::PROPERTY =
    ::jxx::NEW<::jxx::lang::String>(
        "org.w3c.dom.DOMImplementationSourceList");

DOMImplementationRegistry::DOMImplementationRegistry()
    : Super()
    , impl_(std::make_shared<Impl>()) {
    impl_->sources.push_back(
        ::jxx::org::w3c::dom::internal::implementationSource());
}

DOMImplementationRegistry::~DOMImplementationRegistry() = default;

::jxx::Ptr<DOMImplementationRegistry>
DOMImplementationRegistry::newInstance() {
    ::jxx::Ptr<DOMImplementationRegistry> registry(
        new DOMImplementationRegistry());

    ::jxx::detail::initializethisPtr_(registry);

    return registry;
}

::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementation>
DOMImplementationRegistry::getDOMImplementation(
    const ::jxx::Ptr<::jxx::lang::String>& features) {
    for (const auto& source : impl_->sources) {
        const auto implementation =
            source->getDOMImplementation(features);

        if (implementation != nullptr) {
            return implementation;
        }
    }

    return nullptr;
}

::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementationList>
DOMImplementationRegistry::getDOMImplementationList(
    const ::jxx::Ptr<::jxx::lang::String>& features) {
    std::vector<
        ::jxx::Ptr<
            ::jxx::org::w3c::dom::DOMImplementation>> values;

    for (const auto& source : impl_->sources) {
        const auto sourceValues =
            source->getDOMImplementationList(features);

        if (sourceValues == nullptr) {
            continue;
        }

        for (::jxx::lang::jint index = 0;
             index < sourceValues->getLength();
             ++index) {
            const auto candidate = sourceValues->item(index);
            if (candidate == nullptr) {
                continue;
            }

            bool duplicate = false;
            for (const auto& current : values) {
                if (current == candidate) {
                    duplicate = true;
                    break;
                }
            }

            if (!duplicate) {
                values.push_back(candidate);
            }
        }
    }

    return ::jxx::NEW<RegistryImplementationList>(
        std::move(values));
}

void DOMImplementationRegistry::addSource(
    const ::jxx::Ptr<
        ::jxx::org::w3c::dom::DOMImplementationSource>& source) {
    if (source == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    impl_->sources.push_back(source);
}

} // namespace jxx::org::w3c::dom::bootstrap
