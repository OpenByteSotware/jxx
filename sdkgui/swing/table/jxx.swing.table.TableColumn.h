#pragma once

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::swing::table
{
    class TableCellEditor;
    class TableCellRenderer;

    class TableColumn : public ::jxx::lang::ClassBase<
        TableColumn, ::jxx::lang::Object, ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<
            TableColumn, JxxSuper, ::jxx::io::SerializableI>;

        static constexpr ::jxx::lang::jint DEFAULT_WIDTH = 75;

        TableColumn();
        explicit TableColumn(::jxx::lang::jint modelIndex);
        TableColumn(
            ::jxx::lang::jint modelIndex,
            ::jxx::lang::jint width);
        TableColumn(
            ::jxx::lang::jint modelIndex,
            ::jxx::lang::jint width,
            const ::jxx::Ptr<TableCellRenderer>& renderer,
            const ::jxx::Ptr<TableCellEditor>& editor);
        ~TableColumn() override = default;

        void setModelIndex(::jxx::lang::jint modelIndex);
        ::jxx::lang::jint getModelIndex() const;
        void setIdentifier(
            const ::jxx::Ptr<::jxx::lang::Object>& identifier);
        ::jxx::Ptr<::jxx::lang::Object> getIdentifier() const;
        void setHeaderValue(
            const ::jxx::Ptr<::jxx::lang::Object>& value);
        ::jxx::Ptr<::jxx::lang::Object> getHeaderValue() const;
        void setHeaderRenderer(
            const ::jxx::Ptr<TableCellRenderer>& renderer);
        ::jxx::Ptr<TableCellRenderer> getHeaderRenderer() const;
        void setCellRenderer(
            const ::jxx::Ptr<TableCellRenderer>& renderer);
        ::jxx::Ptr<TableCellRenderer> getCellRenderer() const;
        void setCellEditor(const ::jxx::Ptr<TableCellEditor>& editor);
        ::jxx::Ptr<TableCellEditor> getCellEditor() const;
        void setWidth(::jxx::lang::jint width);
        ::jxx::lang::jint getWidth() const;
        void setPreferredWidth(::jxx::lang::jint width);
        ::jxx::lang::jint getPreferredWidth() const;
        void setMinWidth(::jxx::lang::jint width);
        ::jxx::lang::jint getMinWidth() const;
        void setMaxWidth(::jxx::lang::jint width);
        ::jxx::lang::jint getMaxWidth() const;
        void setResizable(::jxx::lang::jbool resizable);
        ::jxx::lang::jbool getResizable() const;
        void sizeWidthToFit();

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

    private:
        static ::jxx::lang::jint clampWidth(
            ::jxx::lang::jint width,
            ::jxx::lang::jint minimum,
            ::jxx::lang::jint maximum);

        ::jxx::lang::jint modelIndex_ = 0;
        ::jxx::lang::jint width_ = DEFAULT_WIDTH;
        ::jxx::lang::jint preferredWidth_ = DEFAULT_WIDTH;
        ::jxx::lang::jint minWidth_ = 15;
        ::jxx::lang::jint maxWidth_ = 2147483647;
        ::jxx::lang::jbool resizable_ = true;
        ::jxx::Ptr<::jxx::lang::Object> identifier_;
        ::jxx::Ptr<::jxx::lang::Object> headerValue_;
        ::jxx::Ptr<TableCellRenderer> headerRenderer_;
        ::jxx::Ptr<TableCellRenderer> cellRenderer_;
        ::jxx::Ptr<TableCellEditor> cellEditor_;
    };
}
