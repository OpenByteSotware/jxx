#include "swing/jxx.swing.JTextArea.h"
#include <algorithm>
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::swing
{
    namespace { ::jxx::Ptr<::jxx::lang::String> emptyText(){return ::jxx::NEW<::jxx::lang::String>("");} }
    JTextArea::JTextArea():JTextArea(emptyText(),0,0){} JTextArea::JTextArea(const ::jxx::Ptr<::jxx::lang::String>& text):JTextArea(text,0,0){} JTextArea::JTextArea(::jxx::lang::jint rows,::jxx::lang::jint columns):JTextArea(emptyText(),rows,columns){}
    JTextArea::JTextArea(const ::jxx::Ptr<::jxx::lang::String>& text,::jxx::lang::jint rows,::jxx::lang::jint columns):Super(text){setRows(rows);setColumns(columns);}
    void JTextArea::append(const ::jxx::Ptr<::jxx::lang::String>& value){insert(value,static_cast<::jxx::lang::jint>(getText()->utf8().size()));}
    void JTextArea::insert(const ::jxx::Ptr<::jxx::lang::String>& value,::jxx::lang::jint position){auto current=getText()->utf8();if(position<0||position>static_cast<::jxx::lang::jint>(current.size()))throw ::jxx::lang::IllegalArgumentException("position");current.insert(position,value?value->utf8():std::string());setText(::jxx::NEW<::jxx::lang::String>(current));}
    void JTextArea::replaceRange(const ::jxx::Ptr<::jxx::lang::String>& value,::jxx::lang::jint start,::jxx::lang::jint end){auto current=getText()->utf8();if(start<0||end<start||end>static_cast<::jxx::lang::jint>(current.size()))throw ::jxx::lang::IllegalArgumentException("range");current.replace(start,end-start,value?value->utf8():std::string());setText(::jxx::NEW<::jxx::lang::String>(current));}
    ::jxx::lang::jint JTextArea::getRows() const{return rows_;} void JTextArea::setRows(::jxx::lang::jint value){if(value<0)throw ::jxx::lang::IllegalArgumentException("rows");rows_=value;invalidate();} ::jxx::lang::jint JTextArea::getColumns() const{return columns_;} void JTextArea::setColumns(::jxx::lang::jint value){if(value<0)throw ::jxx::lang::IllegalArgumentException("columns");columns_=value;invalidate();}
    ::jxx::lang::jbool JTextArea::getLineWrap() const{return lineWrap_;} void JTextArea::setLineWrap(::jxx::lang::jbool value){lineWrap_=value;} ::jxx::lang::jbool JTextArea::getWrapStyleWord() const{return wrapStyleWord_;} void JTextArea::setWrapStyleWord(::jxx::lang::jbool value){wrapStyleWord_=value;}
    ::jxx::Ptr<::jxx::awt::Dimension> JTextArea::getPreferredScrollableViewportSize() const{return ::jxx::NEW<::jxx::awt::Dimension>(columns_*8+16,rows_*16+16);}
}
