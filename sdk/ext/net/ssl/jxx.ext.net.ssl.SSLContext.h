#pragma once
#include <memory>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::security { class SecureRandom; }
namespace jxx::ext::net::ssl { class KeyManager; class TrustManager; class SSLSocketFactory;
class SSLContext final : public ::jxx::lang::ClassBase<SSLContext,::jxx::lang::Object> { public:
using KeyManagerArray=::jxx::lang::JxxArray<::jxx::Ptr<KeyManager>,1U>; using TrustManagerArray=::jxx::lang::JxxArray<::jxx::Ptr<TrustManager>,1U>;
static ::jxx::Ptr<SSLContext> getInstance(const ::jxx::Ptr<::jxx::lang::String>& protocol); static ::jxx::Ptr<SSLContext> getDefault(); static void setDefault(const ::jxx::Ptr<SSLContext>& context);
void init(const ::jxx::Ptr<KeyManagerArray>& keys,const ::jxx::Ptr<TrustManagerArray>& trusts,const ::jxx::Ptr<::jxx::security::SecureRandom>& random);
::jxx::Ptr<SSLSocketFactory> getSocketFactory(); ::jxx::Ptr<::jxx::lang::String> getProtocol()const;
~SSLContext() override;

// Public for jxx::NEW<SSLContext>(...) construction. Public application
// construction remains discouraged; use getInstance() or getDefault().
explicit SSLContext(const ::jxx::Ptr<::jxx::lang::String>& protocol);

private:
class Native;
std::shared_ptr<Native> native_;
::jxx::Ptr<::jxx::lang::String> protocol_;
}; }
