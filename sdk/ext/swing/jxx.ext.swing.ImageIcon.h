#pragma once

#include "awt/jxx.awt.Image.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx_types.h"

namespace jxx::io {
class ObjectInputStream;
class ObjectOutputStream;
}

namespace jxx::ext::swing {

class ImageIcon : public ::jxx::lang::ClassBase<
    ImageIcon, ::jxx::lang::Object, ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        ImageIcon, JxxSuper, ::jxx::io::SerializableI>;

    ImageIcon();
    explicit ImageIcon(
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>& data);
    ImageIcon(
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>& data,
        const ::jxx::Ptr<::jxx::lang::String>& description);
    explicit ImageIcon(const ::jxx::Ptr<::jxx::awt::Image>& image);
    ImageIcon(const ::jxx::Ptr<::jxx::awt::Image>& image,
        const ::jxx::Ptr<::jxx::lang::String>& description);

    ::jxx::Ptr<::jxx::awt::Image> getImage() const;
    void setImage(const ::jxx::Ptr<::jxx::awt::Image>& image);
    ::jxx::Ptr<::jxx::lang::String> getDescription() const;
    void setDescription(const ::jxx::Ptr<::jxx::lang::String>& description);
    ::jxx::lang::jint getIconWidth() const;
    ::jxx::lang::jint getIconHeight() const;

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
    void readObjectNoData() override;

private:
    void decode_(
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>& data);

    ::jxx::Ptr<::jxx::awt::Image> image_;
    ::jxx::Ptr<::jxx::lang::String> description_;
};

} // namespace jxx::ext::swing
