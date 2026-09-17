#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::gui::internal
{
    class WxFileDialogPeer final :
        public ::jxx::lang::ClassBase<WxFileDialogPeer, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<WxFileDialogPeer, JxxSuper>;

        WxFileDialogPeer(
            const ::jxx::Ptr<::jxx::lang::String>& title,
            ::jxx::lang::jint mode,
            const ::jxx::Ptr<::jxx::lang::String>& directory,
            const ::jxx::Ptr<::jxx::lang::String>& file,
            ::jxx::lang::jbool multipleMode);

        ::jxx::lang::jbool showModal();
        ::jxx::Ptr<::jxx::lang::String> getDirectory() const;
        ::jxx::Ptr<::jxx::lang::String> getFile() const;

    private:
        ::jxx::Ptr<::jxx::lang::String> title_;
        ::jxx::Ptr<::jxx::lang::String> directory_;
        ::jxx::Ptr<::jxx::lang::String> file_;
        ::jxx::lang::jint mode_;
        ::jxx::lang::jbool multipleMode_;
    };
}
