#include "awt/print/jxx.awt.print.PageFormat.h"

#include "awt/print/jxx.awt.print.Paper.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt::print
{
    PageFormat::PageFormat() : Super(), paper_(::jxx::NEW<Paper>()){}
    ::jxx::Ptr<::jxx::lang::Object> PageFormat::clone() const
    {
        auto result=::jxx::NEW<PageFormat>();result->orientation_=orientation_;
        result->paper_=::jxx::CAST<Paper>(paper_->clone());return result;
    }
    ::jxx::lang::jdouble PageFormat::getWidth()const{return orientation_==PORTRAIT?paper_->getWidth():paper_->getHeight();}
    ::jxx::lang::jdouble PageFormat::getHeight()const{return orientation_==PORTRAIT?paper_->getHeight():paper_->getWidth();}
    ::jxx::lang::jdouble PageFormat::getImageableX()const{return orientation_==PORTRAIT?paper_->getImageableX():orientation_==LANDSCAPE?paper_->getImageableY():paper_->getHeight()-(paper_->getImageableY()+paper_->getImageableHeight());}
    ::jxx::lang::jdouble PageFormat::getImageableY()const{return orientation_==PORTRAIT?paper_->getImageableY():orientation_==LANDSCAPE?paper_->getWidth()-(paper_->getImageableX()+paper_->getImageableWidth()):paper_->getImageableX();}
    ::jxx::lang::jdouble PageFormat::getImageableWidth()const{return orientation_==PORTRAIT?paper_->getImageableWidth():paper_->getImageableHeight();}
    ::jxx::lang::jdouble PageFormat::getImageableHeight()const{return orientation_==PORTRAIT?paper_->getImageableHeight():paper_->getImageableWidth();}
    ::jxx::Ptr<Paper> PageFormat::getPaper()const{return ::jxx::CAST<Paper>(paper_->clone());}
    void PageFormat::setPaper(const ::jxx::Ptr<Paper>& p){if(!p)throw ::jxx::lang::IllegalArgumentException("paper");paper_=::jxx::CAST<Paper>(p->clone());}
    ::jxx::lang::jint PageFormat::getOrientation()const{return orientation_;}
    void PageFormat::setOrientation(::jxx::lang::jint v){if(v<LANDSCAPE||v>REVERSE_LANDSCAPE)throw ::jxx::lang::IllegalArgumentException("orientation");orientation_=v;}
    PageFormat::Matrix PageFormat::getMatrix()const{auto m=::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jdouble,1>>(6);if(orientation_==PORTRAIT){(*m)[0]=1;(*m)[3]=1;}else if(orientation_==LANDSCAPE){(*m)[1]=1;(*m)[2]=-1;(*m)[4]=paper_->getHeight();}else{(*m)[1]=-1;(*m)[2]=1;(*m)[5]=paper_->getWidth();}return m;}
}
