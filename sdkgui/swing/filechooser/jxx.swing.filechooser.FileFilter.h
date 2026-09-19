#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::io { class File; }
namespace jxx::swing::filechooser
{
    class FileFilter : public ::jxx::lang::InterfaceBase<FileFilter>
    {
    public:
        ~FileFilter() override = default;
        virtual ::jxx::lang::jbool accept(
            const ::jxx::Ptr<::jxx::io::File>& file) const = 0;
        virtual ::jxx::Ptr<::jxx::lang::String> getDescription() const = 0;
    };
}
