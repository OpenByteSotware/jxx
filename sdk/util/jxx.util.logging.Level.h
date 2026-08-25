#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::util::logging {
class Level : public jxx::lang::Object {
public:
    static jxx::Ptr<Level> OFF(); static jxx::Ptr<Level> SEVERE();
    static jxx::Ptr<Level> WARNING(); static jxx::Ptr<Level> INFO();
    static jxx::Ptr<Level> CONFIG(); static jxx::Ptr<Level> FINE();
    static jxx::Ptr<Level> FINER(); static jxx::Ptr<Level> FINEST();
    static jxx::Ptr<Level> ALL();
    Level(const jxx::Ptr<jxx::lang::String>& name, jxx::lang::jint value);
    Level(const jxx::Ptr<jxx::lang::String>& name, jxx::lang::jint value,
          const jxx::Ptr<jxx::lang::String>& resourceBundleName);
    jxx::Ptr<jxx::lang::String> getName() const; jxx::Ptr<jxx::lang::String> getLocalizedName() const;
    jxx::Ptr<jxx::lang::String> getResourceBundleName() const; jxx::lang::jint intValue() const noexcept;
    static jxx::Ptr<Level> parse(const jxx::Ptr<jxx::lang::String>& name);
    jxx::Ptr<jxx::lang::String> toString() const override;
    JXX_OBJECT_CLONE(Level)
  
private: jxx::Ptr<jxx::lang::String> name_; jxx::lang::jint value_; jxx::Ptr<jxx::lang::String> bundleName_;
};
}
