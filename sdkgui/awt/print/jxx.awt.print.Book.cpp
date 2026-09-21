#include "awt/print/jxx.awt.print.Book.h"

#include <stdexcept>

#include "awt/print/jxx.awt.print.PageFormat.h"
#include "awt/print/jxx.awt.print.Printable.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt::print
{
    Book::Book() : Super(){}
    const Book::Entry& Book::pageAt(::jxx::lang::jint i)const{if(i<0||i>=static_cast<::jxx::lang::jint>(pages_.size()))throw std::out_of_range("pageIndex");return pages_[static_cast<std::size_t>(i)];}
    ::jxx::lang::jint Book::getNumberOfPages()const{return static_cast<::jxx::lang::jint>(pages_.size());}
    ::jxx::Ptr<PageFormat> Book::getPageFormat(::jxx::lang::jint i)const{return pageAt(i).format;}
    ::jxx::Ptr<Printable> Book::getPrintable(::jxx::lang::jint i)const{return pageAt(i).painter;}
    void Book::setPage(::jxx::lang::jint i,const ::jxx::Ptr<Printable>& p,const ::jxx::Ptr<PageFormat>& f){if(!p||!f)throw ::jxx::lang::IllegalArgumentException("page");pageAt(i);pages_[static_cast<std::size_t>(i)]={p,f};}
    void Book::append(const ::jxx::Ptr<Printable>& p,const ::jxx::Ptr<PageFormat>& f){append(p,f,1);}
    void Book::append(const ::jxx::Ptr<Printable>& p,const ::jxx::Ptr<PageFormat>& f,::jxx::lang::jint n){if(!p||!f||n<0)throw ::jxx::lang::IllegalArgumentException("page");for(::jxx::lang::jint i=0;i<n;++i)pages_.push_back({p,f});}
}
