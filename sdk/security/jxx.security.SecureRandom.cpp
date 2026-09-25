#include "security/jxx.security.SecureRandom.h"
#include <openssl/rand.h>
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::security { SecureRandom::SecureRandom()=default; void SecureRandom::nextBytes(const ::jxx::lang::ByteArray& b){if(b==nullptr)throw ::jxx::lang::NullPointerException();if(b->length>0&&RAND_bytes(reinterpret_cast<unsigned char*>(&(*b)[0]),b->length)!=1)throw ::jxx::io::IOException("OpenSSL RAND_bytes failed");} }
