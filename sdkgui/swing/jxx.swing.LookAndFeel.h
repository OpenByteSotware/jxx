#pragma once

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::swing
{
    class LayoutStyle;
    class UIDefaults;

    class LookAndFeel : public ::jxx::lang::ClassBase<
        LookAndFeel, ::jxx::lang::Object, ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<
            LookAndFeel, JxxSuper, ::jxx::io::SerializableI>;

        LookAndFeel();
        ~LookAndFeel() override = default;

        virtual ::jxx::Ptr<::jxx::lang::String> getName() const = 0;
        virtual ::jxx::Ptr<::jxx::lang::String> getID() const = 0;
        virtual ::jxx::Ptr<::jxx::lang::String> getDescription() const = 0;
        virtual ::jxx::lang::jbool isNativeLookAndFeel() const = 0;
        virtual ::jxx::lang::jbool isSupportedLookAndFeel() const = 0;
        virtual ::jxx::Ptr<UIDefaults> getDefaults();
        virtual ::jxx::Ptr<LayoutStyle> getLayoutStyle();
        virtual void initialize();
        virtual void uninitialize();

        static void installColors(
            const ::jxx::Ptr<class JComponent>& component,
            const ::jxx::Ptr<::jxx::lang::String>& backgroundKey,
            const ::jxx::Ptr<::jxx::lang::String>& foregroundKey);
        static void installColorsAndFont(
            const ::jxx::Ptr<class JComponent>& component,
            const ::jxx::Ptr<::jxx::lang::String>& backgroundKey,
            const ::jxx::Ptr<::jxx::lang::String>& foregroundKey,
            const ::jxx::Ptr<::jxx::lang::String>& fontKey);

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

    private:
        ::jxx::Ptr<UIDefaults> defaults_;
    };
}
