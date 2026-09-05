#include "lang/jxx.lang.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Throwable.h"
#include "lang/jxx.lang.Exception.h"
#include "lang/jxx.lang.RuntimeException.h"
#include "lang/jxx.lang.Error.h"
#include "lang/jxx.lang.ReflectiveOperationException.h"
#include "lang/jxx.lang.LinkageError.h"
#include "lang/jxx.lang.IncompatibleClassChangeError.h"
#include "lang/jxx.lang.VirtualMachineError.h"
#include "lang/jxx.lang.AbstractMethodError.h"
#include "lang/jxx.lang.ArithmeticException.h"
#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.ArrayStoreException.h"
#include "lang/jxx.lang.AssertionError.h"
#include "lang/jxx.lang.BootstrapMethodError.h"
#include "lang/jxx.lang.ClassCastException.h"
#include "lang/jxx.lang.ClassCircularityError.h"
#include "lang/jxx.lang.ClassFormatError.h"
#include "lang/jxx.lang.ClassNotFoundException.h"
#include "lang/jxx.lang.CloneNotSupportedException.h"
#include "lang/jxx.lang.EnumConstantNotPresentException.h"
#include "lang/jxx.lang.ExceptionInInitializerError.h"
#include "lang/jxx.lang.IllegalAccessError.h"
#include "lang/jxx.lang.IllegalAccessException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalMonitorStateException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.IllegalThreadStateException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.InstantiationError.h"
#include "lang/jxx.lang.InstantiationException.h"
#include "lang/jxx.lang.InternalError.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.NegativeArraySizeException.h"
#include "lang/jxx.lang.NoClassDefFoundError.h"
#include "lang/jxx.lang.NoSuchFieldError.h"
#include "lang/jxx.lang.NoSuchFieldException.h"
#include "lang/jxx.lang.NoSuchMethodError.h"
#include "lang/jxx.lang.NoSuchMethodException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.NumberFormatException.h"
#include "lang/jxx.lang.OutOfMemoryError.h"
#include "lang/jxx.lang.SecurityException.h"
#include "lang/jxx.lang.StackOverflowError.h"
#include "lang/jxx.lang.StringIndexOutOfBoundsException.h"
#include "lang/jxx.lang.TypeNotPresentException.h"
#include "lang/jxx.lang.UnknownError.h"
#include "lang/jxx.lang.UnsatisfiedLinkError.h"
#include "lang/jxx.lang.UnsupportedClassVersionError.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "lang/jxx.lang.VerifyError.h"
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Byte.h"
#include "lang/jxx.lang.Character.h"
#include "lang/jxx.lang.Charset.h"
#include "lang/jxx.lang.ClassLoader.h"
#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.Float.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.Package.h"
#include "lang/jxx.lang.Short.h"
#include "lang/jxx.lang.StringBuffer.h"
#include "lang/jxx.lang.StringBuilder.h"

namespace jxx::lang {

namespace {

template <typename T>
void registerType()
{
    jxx::lang::class_info_detail::registerClassInfo<T>();
}

} // namespace

void initializeClasses()
{
    static std::once_flag flag;

    std::call_once(
        flag,
        [] {
            (void)jxx::lang::class_info_detail::ensureObjectRegistered();
            registerType<Throwable>();
            registerType<Exception>();
            registerType<RuntimeException>();
            registerType<Error>();
            registerType<ReflectiveOperationException>();
            registerType<LinkageError>();
            registerType<IncompatibleClassChangeError>();
            registerType<VirtualMachineError>();
            registerType<AbstractMethodError>();
            registerType<ArithmeticException>();
            registerType<ArrayIndexOutOfBoundsException>();
            registerType<ArrayStoreException>();
            registerType<AssertionError>();
            registerType<BootstrapMethodError>();
            registerType<ClassCastException>();
            registerType<ClassCircularityError>();
            registerType<ClassFormatError>();
            registerType<ClassNotFoundException>();
            registerType<CloneNotSupportedException>();
            registerType<EnumConstantNotPresentException>();
            registerType<ExceptionInInitializerError>();
            registerType<IllegalAccessError>();
            registerType<IllegalAccessException>();
            registerType<IllegalArgumentException>();
            registerType<IllegalMonitorStateException>();
            registerType<IllegalStateException>();
            registerType<IllegalThreadStateException>();
            registerType<IndexOutOfBoundsException>();
            registerType<InstantiationError>();
            registerType<InstantiationException>();
            registerType<InternalError>();
            registerType<InterruptedException>();
            registerType<NegativeArraySizeException>();
            registerType<NoClassDefFoundError>();
            registerType<NoSuchFieldError>();
            registerType<NoSuchFieldException>();
            registerType<NoSuchMethodError>();
            registerType<NoSuchMethodException>();
            registerType<NullPointerException>();
            registerType<NumberFormatException>();
            registerType<OutOfMemoryError>();
            registerType<SecurityException>();
            registerType<StackOverflowError>();
            registerType<StringIndexOutOfBoundsException>();
            registerType<TypeNotPresentException>();
            registerType<UnknownError>();
            registerType<UnsatisfiedLinkError>();
            registerType<UnsupportedClassVersionError>();
            registerType<UnsupportedOperationException>();
            registerType<VerifyError>();
            registerType<Boolean>();
            registerType<Byte>();
            registerType<Character>();
            registerType<Charset>();
            registerType<ClassLoader>();
            registerType<Double>();
            registerType<Float>();
            registerType<Integer>();
            registerType<Long>();
            registerType<Package>();
            registerType<Short>();
            registerType<StringBuffer>();
            registerType<StringBuilder>();
        });
}

} // namespace jxx::lang
