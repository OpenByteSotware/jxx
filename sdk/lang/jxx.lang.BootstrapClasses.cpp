#include "lang/jxx.lang.BootstrapClasses.h"
#include <typeindex>
#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::lang::class_info_detail {
namespace { jxx::Ptr<ClassAny> reg(const char* n,const std::type_index& id,const ClassAny::InstancePredicate& p){try{return ClassAny::forType(id);}catch(const IllegalStateException&){ClassAny::Meta m;m.binaryName=n;m.typeId=id;m.isInterface=true;m.modifiers=0x0001|0x0200|0x0400;m.instancePredicate=p;return ClassAny::registerClass(m);}} }
//jxx::Ptr<ClassAny> ensureObjectRegistered(){static const auto d=[](){try{return ClassAny::forType(typeid(Object));}catch(const IllegalStateException&){ClassAny::Meta m;m.binaryName="java.lang.Object";m.typeId=typeid(Object);m.modifiers=0x0001;m.factory=[]()->jxx::Ptr<Object>{return jxx::NEW<Object>();};m.instancePredicate=[](const jxx::Ptr<Object>& o)->jbool{return o!=nullptr;};return ClassAny::registerClass(m);}}();return d;}
jxx::Ptr<ClassAny> ensureSerializableRegistered(){static const auto d=reg("java.io.Serializable",typeid(jxx::io::Serializable),[](const jxx::Ptr<Object>& o)->jbool{return o&&std::dynamic_pointer_cast<jxx::io::Serializable>(o)!=nullptr;});return d;}
jxx::Ptr<ClassAny> ensureCloneableRegistered(){static const auto d=reg("java.lang.Cloneable",typeid(Cloneable),[](const jxx::Ptr<Object>& o)->jbool{return o&&std::dynamic_pointer_cast<Cloneable>(o)!=nullptr;});return d;}
jxx::Ptr<ClassAny> ensureCharSequenceRegistered(){static const auto d=reg("java.lang.CharSequence",typeid(CharSequence),[](const jxx::Ptr<Object>& o)->jbool{return o&&std::dynamic_pointer_cast<CharSequence>(o)!=nullptr;});return d;}
jxx::Ptr<ClassAny> ensureComparableRegistered(){static const auto d=reg("java.lang.Comparable",typeid(Comparable<String>),[](const jxx::Ptr<Object>& o)->jbool{return o&&std::dynamic_pointer_cast<Comparable<String>>(o)!=nullptr;});return d;}
jxx::Ptr<ClassAny> ensureStringRegistered(){static const auto d=[](){try{return ClassAny::forType(typeid(String));}catch(const IllegalStateException&){ClassAny::Meta m;m.binaryName="java.lang.String";m.typeId=typeid(String);m.superClass=ensureObjectRegistered();m.interfaces={ensureSerializableRegistered(),ensureComparableRegistered(),ensureCharSequenceRegistered()};m.modifiers=0x0001|0x0010;m.factory=[]()->jxx::Ptr<Object>{return jxx::CAST<Object>(jxx::NEW<String>());};m.instancePredicate=[](const jxx::Ptr<Object>& o)->jbool{return o&&jxx::CAST<String>(o)!=nullptr;};return ClassAny::registerClass(m);}}();return d;}
}
