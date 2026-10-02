#include <gtest/gtest.h>
#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <vector>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslKeyStoreKeyManager.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"
#include "security/internal/jxx.security.internal.EncodedPrivateKey.h"
#include "security/jxx.security.KeyStore.h"
namespace {
::jxx::lang::ByteArray bytes(const unsigned char* data,int length){auto r=::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte,1U>>(length);for(int i=0;i<length;++i)(*r)[i]=static_cast<::jxx::lang::jbyte>(data[i]);return r;}
struct Identity{::jxx::Ptr<::jxx::security::PrivateKey> key;::jxx::Ptr<::jxx::security::KeyStore::CertificateArray> chain;};
Identity newIdentity(const char* cn,long serial){EVP_PKEY* nk=EVP_PKEY_new();RSA* rsa=RSA_new();BIGNUM* e=BN_new();BN_set_word(e,RSA_F4);RSA_generate_key_ex(rsa,2048,e,nullptr);EVP_PKEY_assign_RSA(nk,rsa);BN_free(e);X509* cert=X509_new();X509_set_version(cert,2);ASN1_INTEGER_set(X509_get_serialNumber(cert),serial);X509_gmtime_adj(X509_get_notBefore(cert),0);X509_gmtime_adj(X509_get_notAfter(cert),3600);X509_set_pubkey(cert,nk);X509_NAME* name=X509_get_subject_name(cert);X509_NAME_add_entry_by_txt(name,"CN",MBSTRING_ASC,reinterpret_cast<const unsigned char*>(cn),-1,-1,0);X509_set_issuer_name(cert,name);X509_sign(cert,nk,EVP_sha256());int kl=i2d_PrivateKey(nk,nullptr);std::vector<unsigned char> kd(kl);unsigned char* kp=kd.data();i2d_PrivateKey(nk,&kp);int cl=i2d_X509(cert,nullptr);std::vector<unsigned char> cd(cl);unsigned char* cp=cd.data();i2d_X509(cert,&cp);Identity r;r.key=::jxx::NEW<::jxx::security::internal::EncodedPrivateKey>(::jxx::NEW<::jxx::lang::String>("RSA"),bytes(kd.data(),kl));r.chain=::jxx::NEW<::jxx::security::KeyStore::CertificateArray>(1);(*r.chain)[0]=::jxx::NEW<::jxx::ext::net::ssl::internal::OpenSslX509Certificate>(bytes(cd.data(),cl));X509_free(cert);EVP_PKEY_free(nk);return r;}
::jxx::Ptr<::jxx::security::KeyStore> two(){auto s=::jxx::security::KeyStore::getInstance(::jxx::NEW<::jxx::lang::String>("PKCS12"));s->load(nullptr,nullptr);auto a=newIdentity("first",1);auto b=newIdentity("second",2);s->setKeyEntry(::jxx::NEW<::jxx::lang::String>("first"),a.key,nullptr,a.chain);s->setKeyEntry(::jxx::NEW<::jxx::lang::String>("second"),b.key,nullptr,b.chain);return s;}
}
TEST(MultipleIdentityAliasSelectionTest,RetainsIndependentAliases){auto s=two();EXPECT_EQ(2,s->size());EXPECT_TRUE(s->isKeyEntry(::jxx::NEW<::jxx::lang::String>("first")));EXPECT_TRUE(s->isKeyEntry(::jxx::NEW<::jxx::lang::String>("second")));}
TEST(MultipleIdentityAliasSelectionTest,ReturnsEveryMatchingAlias){auto m=::jxx::NEW<::jxx::ext::net::ssl::internal::OpenSslKeyStoreKeyManager>(two(),nullptr);auto a=m->getClientAliases(::jxx::NEW<::jxx::lang::String>("RSA"),nullptr);ASSERT_NE(nullptr,a);ASSERT_EQ(2,a->length);EXPECT_EQ("first",(*a)[0]->utf8());EXPECT_EQ("second",(*a)[1]->utf8());}
TEST(MultipleIdentityAliasSelectionTest,HonorsKeyTypePreference){auto m=::jxx::NEW<::jxx::ext::net::ssl::internal::OpenSslKeyStoreKeyManager>(two(),nullptr);auto t=::jxx::NEW<::jxx::ext::net::ssl::X509KeyManager::StringArray>(2);(*t)[0]=::jxx::NEW<::jxx::lang::String>("EC");(*t)[1]=::jxx::NEW<::jxx::lang::String>("RSA");auto a=m->chooseClientAlias(t,nullptr,nullptr);ASSERT_NE(nullptr,a);EXPECT_EQ("first",a->utf8());}
TEST(MultipleIdentityAliasSelectionTest,UnknownAliasReturnsNull){auto m=::jxx::NEW<::jxx::ext::net::ssl::internal::OpenSslKeyStoreKeyManager>(two(),nullptr);auto a=::jxx::NEW<::jxx::lang::String>("missing");EXPECT_EQ(nullptr,m->getPrivateKey(a));EXPECT_EQ(nullptr,m->getCertificateChain(a));}
