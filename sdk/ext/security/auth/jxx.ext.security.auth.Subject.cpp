#include "ext/security/auth/jxx.ext.security.auth.Subject.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::ext::security::auth {
Subject::Subject()
    : principals_(::jxx::NEW<::jxx::util::HashSet<::jxx::security::Principal>>()),
      publicCredentials_(::jxx::NEW<::jxx::util::HashSet<::jxx::lang::Object>>()),
      privateCredentials_(::jxx::NEW<::jxx::util::HashSet<::jxx::lang::Object>>()) {}
Subject::Subject(::jxx::lang::jbool ro, const ::jxx::Ptr<::jxx::util::Set<::jxx::security::Principal>>& p, const ::jxx::Ptr<::jxx::util::Set<::jxx::lang::Object>>& pub, const ::jxx::Ptr<::jxx::util::Set<::jxx::lang::Object>>& priv) : Subject() {
    if (p == nullptr || pub == nullptr || priv == nullptr) throw ::jxx::lang::NullPointerException();
    auto pi = p->iterator(); while (pi->hasNext()) principals_->add(pi->next());
    auto ui = pub->iterator(); while (ui->hasNext()) publicCredentials_->add(ui->next());
    auto ri = priv->iterator(); while (ri->hasNext()) privateCredentials_->add(ri->next());
    readOnly_ = ro;
}
void Subject::setReadOnly() { readOnly_ = true; }
::jxx::lang::jbool Subject::isReadOnly() const { return readOnly_; }
::jxx::Ptr<::jxx::util::Set<::jxx::security::Principal>> Subject::getPrincipals() { return principals_; }
::jxx::Ptr<::jxx::util::Set<::jxx::lang::Object>> Subject::getPublicCredentials() { return publicCredentials_; }
::jxx::Ptr<::jxx::util::Set<::jxx::lang::Object>> Subject::getPrivateCredentials() { return privateCredentials_; }
::jxx::lang::jbool Subject::equals(const ::jxx::Ptr<::jxx::lang::Object>& o) const { auto s=::jxx::CAST<Subject>(o); return s!=nullptr && readOnly_==s->readOnly_ && principals_->equals(s->principals_) && publicCredentials_->equals(s->publicCredentials_) && privateCredentials_->equals(s->privateCredentials_); }
::jxx::lang::jint Subject::hashCode() const { return principals_->hashCode() ^ publicCredentials_->hashCode() ^ privateCredentials_->hashCode(); }
::jxx::Ptr<::jxx::lang::String> Subject::toString() const { return ::jxx::NEW<::jxx::lang::String>("Subject"); }
void Subject::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) { if (out==nullptr) throw ::jxx::lang::NullPointerException(); out->writeBoolean(readOnly_); out->writeObject(principals_); out->writeObject(publicCredentials_); out->writeObject(privateCredentials_); }
void Subject::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) { if (in==nullptr) throw ::jxx::lang::NullPointerException(); readOnly_=in->readBoolean(); principals_=::jxx::CAST<::jxx::util::HashSet<::jxx::security::Principal>>(in->readObject()); publicCredentials_=::jxx::CAST<::jxx::util::HashSet<::jxx::lang::Object>>(in->readObject()); privateCredentials_=::jxx::CAST<::jxx::util::HashSet<::jxx::lang::Object>>(in->readObject()); }
void Subject::readObjectNoData() { readOnly_=false; principals_=::jxx::NEW<::jxx::util::HashSet<::jxx::security::Principal>>(); publicCredentials_=::jxx::NEW<::jxx::util::HashSet<::jxx::lang::Object>>(); privateCredentials_=::jxx::NEW<::jxx::util::HashSet<::jxx::lang::Object>>(); }
} // namespace jxx::ext::security::auth
