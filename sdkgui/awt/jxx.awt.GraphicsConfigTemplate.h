#pragma once

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::io
{
    class ObjectInputStream;
    class ObjectOutputStream;
}

namespace jxx::awt
{
    class GraphicsConfiguration;

    class GraphicsConfigTemplate : public ::jxx::lang::ClassBase<
        GraphicsConfigTemplate, ::jxx::lang::Object,
        ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<GraphicsConfigTemplate,
            JxxSuper, ::jxx::io::SerializableI>;
        using ConfigurationArray = ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::Ptr<GraphicsConfiguration>, 1>>;

        static constexpr ::jxx::lang::jint REQUIRED = 1;
        static constexpr ::jxx::lang::jint PREFERRED = 2;
        static constexpr ::jxx::lang::jint UNNECESSARY = 3;

        ~GraphicsConfigTemplate() override = default;

        virtual ::jxx::Ptr<GraphicsConfiguration> getBestConfiguration(
            const ConfigurationArray& configurations) const = 0;
        virtual ::jxx::lang::jbool isGraphicsConfigSupported(
            const ::jxx::Ptr<GraphicsConfiguration>& configuration) const = 0;

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

    protected:
        GraphicsConfigTemplate();
    };
}
