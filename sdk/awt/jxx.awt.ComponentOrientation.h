#pragma once

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::util
{
    class Locale;
    class ResourceBundle;
}

namespace jxx::awt
{
    class ComponentOrientation;
    namespace component_orientation_detail {
        ::jxx::Ptr<ComponentOrientation> createSerializationInstance();
    }

    class ComponentOrientation final :
        public ::jxx::lang::ClassBase<ComponentOrientation,
            ::jxx::lang::Object,
            ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<ComponentOrientation, JxxSuper,
            ::jxx::io::SerializableI>;

        static ::jxx::Ptr<ComponentOrientation> LEFT_TO_RIGHT;
        static ::jxx::Ptr<ComponentOrientation> RIGHT_TO_LEFT;
        static ::jxx::Ptr<ComponentOrientation> UNKNOWN;

        ::jxx::lang::jbool isHorizontal() const;
        ::jxx::lang::jbool isLeftToRight() const;

        static ::jxx::Ptr<ComponentOrientation> getOrientation(
            const ::jxx::Ptr<::jxx::util::Locale>& locale);

        static ::jxx::Ptr<ComponentOrientation> getOrientation(
            const ::jxx::Ptr<::jxx::util::ResourceBundle>& bundle);

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) override;
        void readObjectNoData() override;

    private:
        friend ::jxx::Ptr<ComponentOrientation>
            component_orientation_detail::createSerializationInstance();

        static ::jxx::Ptr<ComponentOrientation> create(
            ::jxx::lang::jbool horizontal,
            ::jxx::lang::jbool leftToRight);

        ComponentOrientation(::jxx::lang::jbool horizontal,
            ::jxx::lang::jbool leftToRight);

        ::jxx::lang::jbool horizontal_;
        ::jxx::lang::jbool leftToRight_;
    };
}
