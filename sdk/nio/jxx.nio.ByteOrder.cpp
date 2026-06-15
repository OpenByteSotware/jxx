#include "lang/jxx.lang.String.h"
#include "nio/jxx.nio.ByteOrder.h"

namespace
{
    inline bool isLittleEndian_()
    {
        unsigned int x = 1U;
        return *reinterpret_cast<unsigned char*>(&x) == 1U;
    }
}

namespace jxx::nio
{
    jxx::Ptr<ByteOrder> ByteOrder::BIG_ENDIAN = std::make_shared<ByteOrder>(std::make_shared<jxx::lang::String>("BIG_ENDIAN"));
    jxx::Ptr<ByteOrder> ByteOrder::LITTLE_ENDIAN = std::make_shared<ByteOrder>(std::make_shared<jxx::lang::String>("LITTLE_ENDIAN"));

    ByteOrder::ByteOrder(jxx::Ptr<jxx::lang::String> name)
        : name_(std::move(name))
    {
    }

    jxx::Ptr<ByteOrder> ByteOrder::nativeOrder()
    {
        return isLittleEndian_() ? LITTLE_ENDIAN : BIG_ENDIAN;
    }

    jxx::Ptr<jxx::lang::String> ByteOrder::toString() const { return name_; }

    jxx::lang::jbool ByteOrder::equals(jxx::Ptr<jxx::lang::Object> other) const
    {
        auto o = std::dynamic_pointer_cast<ByteOrder>(other);
        return o && name_ && o->name_ && name_->equals(o->name_);
    }

    jxx::lang::jint ByteOrder::hashCode() const { return name_ ? name_->hashCode() : 0; }
}
