#include "net/jxx.net.URLStreamHandler.h"

#include "net/jxx.net.URL.h"

namespace jxx::net
{
    jxx::lang::jint URLStreamHandler::getDefaultPort() const
    {
        return -1;
    }

    jxx::lang::jbool URLStreamHandler::equals(const jxx::Ptr<URL>& u1,
                                              const jxx::Ptr<URL>& u2) const
    {
        return u1 && u2 && u1->equals(u2);
    }

    jxx::lang::jint URLStreamHandler::hashCode(const jxx::Ptr<URL>& u) const
    {
        return u ? u->hashCode() : 0;
    }

    jxx::lang::jbool URLStreamHandler::sameFile(const jxx::Ptr<URL>& u1,
                                                const jxx::Ptr<URL>& u2) const
    {
        return u1 && u2 && u1->sameFile(u2);
    } 

    jxx::Ptr<jxx::lang::String> URLStreamHandler::toExternalForm(const jxx::Ptr<URL>& u) const
    {
        return u ? u->toExternalForm() : nullptr;
    }
}
