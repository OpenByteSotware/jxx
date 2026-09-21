#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::awt
{
    class JobAttributes final : public ::jxx::lang::ClassBase<
        JobAttributes, ::jxx::lang::Object>
    {
    public:
        using JxxSuper=::jxx::lang::Object;
        using Super=::jxx::lang::ClassBase<JobAttributes,JxxSuper>;
        enum class DefaultSelectionType{ALL,RANGE,SELECTION};
        enum class DestinationType{FILE,PRINTER};
        enum class DialogType{COMMON,NATIVE,NONE};
        enum class MultipleDocumentHandlingType{SEPARATE_DOCUMENTS_COLLATED_COPIES,SEPARATE_DOCUMENTS_UNCOLLATED_COPIES};
        enum class SidesType{ONE_SIDED,TWO_SIDED_LONG_EDGE,TWO_SIDED_SHORT_EDGE};
        using Range=::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint,1>>;
        using Ranges=::jxx::Ptr<::jxx::lang::JxxArray<Range,1>>;

        JobAttributes();
        explicit JobAttributes(const ::jxx::Ptr<JobAttributes>& source);
        ::jxx::lang::jint getCopies()const;void setCopies(::jxx::lang::jint value);
        DefaultSelectionType getDefaultSelection()const;void setDefaultSelection(DefaultSelectionType value);
        DestinationType getDestination()const;void setDestination(DestinationType value);
        DialogType getDialog()const;void setDialog(DialogType value);
        ::jxx::Ptr<::jxx::lang::String> getFileName()const;void setFileName(const ::jxx::Ptr<::jxx::lang::String>& value);
        ::jxx::Ptr<::jxx::lang::String> getPrinter()const;void setPrinter(const ::jxx::Ptr<::jxx::lang::String>& value);
        ::jxx::lang::jint getMinPage()const;void setMinPage(::jxx::lang::jint value);
        ::jxx::lang::jint getMaxPage()const;void setMaxPage(::jxx::lang::jint value);
        SidesType getSides()const;void setSides(SidesType value);
        void set(const ::jxx::Ptr<JobAttributes>& source);
        ::jxx::Ptr<::jxx::lang::Object> clone()const override;
        ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object)const override;
        ::jxx::lang::jint hashCode()const override;
        ::jxx::Ptr<::jxx::lang::String> toString()const override;
    private:
        ::jxx::lang::jint copies_=1,minPage_=1,maxPage_=9999;
        DefaultSelectionType selection_=DefaultSelectionType::ALL;
        DestinationType destination_=DestinationType::PRINTER;
        DialogType dialog_=DialogType::NATIVE;
        SidesType sides_=SidesType::ONE_SIDED;
        ::jxx::Ptr<::jxx::lang::String> fileName_,printer_;
    };
}
