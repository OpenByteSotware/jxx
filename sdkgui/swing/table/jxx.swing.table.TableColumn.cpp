#include "swing/table/jxx.swing.table.TableColumn.h"

#include <algorithm>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "swing/table/jxx.swing.table.TableCellEditor.h"
#include "swing/table/jxx.swing.table.TableCellRenderer.h"

namespace jxx::swing::table
{
    TableColumn::TableColumn() : TableColumn(0)
    {
    }

    TableColumn::TableColumn(::jxx::lang::jint modelIndex)
        : TableColumn(modelIndex, DEFAULT_WIDTH)
    {
    }

    TableColumn::TableColumn(
        ::jxx::lang::jint modelIndex,
        ::jxx::lang::jint width)
        : TableColumn(modelIndex, width, nullptr, nullptr)
    {
    }

    TableColumn::TableColumn(
        ::jxx::lang::jint modelIndex,
        ::jxx::lang::jint width,
        const ::jxx::Ptr<TableCellRenderer>& renderer,
        const ::jxx::Ptr<TableCellEditor>& editor)
        : Super(),
          modelIndex_(modelIndex),
          width_(width),
          preferredWidth_(width),
          cellRenderer_(renderer),
          cellEditor_(editor)
    {
        if (modelIndex < 0)
            throw ::jxx::lang::IllegalArgumentException("modelIndex");
        if (width < 0)
            throw ::jxx::lang::IllegalArgumentException("width");
        width_ = clampWidth(width_, minWidth_, maxWidth_);
        preferredWidth_ = clampWidth(
            preferredWidth_, minWidth_, maxWidth_);
    }

    ::jxx::lang::jint TableColumn::clampWidth(
        ::jxx::lang::jint width,
        ::jxx::lang::jint minimum,
        ::jxx::lang::jint maximum)
    {
        return std::max(minimum, std::min(maximum, width));
    }

    void TableColumn::setModelIndex(::jxx::lang::jint modelIndex)
    {
        if (modelIndex < 0)
            throw ::jxx::lang::IllegalArgumentException("modelIndex");
        modelIndex_ = modelIndex;
    }

    ::jxx::lang::jint TableColumn::getModelIndex() const
    {
        return modelIndex_;
    }

    void TableColumn::setIdentifier(
        const ::jxx::Ptr<::jxx::lang::Object>& identifier)
    {
        identifier_ = identifier;
    }

    ::jxx::Ptr<::jxx::lang::Object> TableColumn::getIdentifier() const
    {
        return identifier_ == nullptr ? headerValue_ : identifier_;
    }

    void TableColumn::setHeaderValue(
        const ::jxx::Ptr<::jxx::lang::Object>& value)
    {
        headerValue_ = value;
    }

    ::jxx::Ptr<::jxx::lang::Object> TableColumn::getHeaderValue() const
    {
        return headerValue_;
    }

    void TableColumn::setHeaderRenderer(
        const ::jxx::Ptr<TableCellRenderer>& renderer)
    {
        headerRenderer_ = renderer;
    }

    ::jxx::Ptr<TableCellRenderer> TableColumn::getHeaderRenderer() const
    {
        return headerRenderer_;
    }

    void TableColumn::setCellRenderer(
        const ::jxx::Ptr<TableCellRenderer>& renderer)
    {
        cellRenderer_ = renderer;
    }

    ::jxx::Ptr<TableCellRenderer> TableColumn::getCellRenderer() const
    {
        return cellRenderer_;
    }

    void TableColumn::setCellEditor(
        const ::jxx::Ptr<TableCellEditor>& editor)
    {
        cellEditor_ = editor;
    }

    ::jxx::Ptr<TableCellEditor> TableColumn::getCellEditor() const
    {
        return cellEditor_;
    }

    void TableColumn::setWidth(::jxx::lang::jint width)
    {
        width_ = clampWidth(width, minWidth_, maxWidth_);
    }

    ::jxx::lang::jint TableColumn::getWidth() const
    {
        return width_;
    }

    void TableColumn::setPreferredWidth(::jxx::lang::jint width)
    {
        preferredWidth_ = clampWidth(width, minWidth_, maxWidth_);
    }

    ::jxx::lang::jint TableColumn::getPreferredWidth() const
    {
        return preferredWidth_;
    }

    void TableColumn::setMinWidth(::jxx::lang::jint width)
    {
        minWidth_ = std::max<::jxx::lang::jint>(0, width);
        if (maxWidth_ < minWidth_) maxWidth_ = minWidth_;
        width_ = clampWidth(width_, minWidth_, maxWidth_);
        preferredWidth_ = clampWidth(
            preferredWidth_, minWidth_, maxWidth_);
    }

    ::jxx::lang::jint TableColumn::getMinWidth() const
    {
        return minWidth_;
    }

    void TableColumn::setMaxWidth(::jxx::lang::jint width)
    {
        maxWidth_ = std::max<::jxx::lang::jint>(minWidth_, width);
        width_ = clampWidth(width_, minWidth_, maxWidth_);
        preferredWidth_ = clampWidth(
            preferredWidth_, minWidth_, maxWidth_);
    }

    ::jxx::lang::jint TableColumn::getMaxWidth() const
    {
        return maxWidth_;
    }

    void TableColumn::setResizable(::jxx::lang::jbool resizable)
    {
        resizable_ = resizable;
    }

    ::jxx::lang::jbool TableColumn::getResizable() const
    {
        return resizable_;
    }

    void TableColumn::sizeWidthToFit()
    {
        setWidth(preferredWidth_);
    }

    void TableColumn::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&)
    {
    }

    void TableColumn::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>&)
    {
        width_ = clampWidth(width_, minWidth_, maxWidth_);
        preferredWidth_ = clampWidth(
            preferredWidth_, minWidth_, maxWidth_);
    }

    void TableColumn::readObjectNoData()
    {
        modelIndex_ = 0;
        width_ = DEFAULT_WIDTH;
        preferredWidth_ = DEFAULT_WIDTH;
        minWidth_ = 15;
        maxWidth_ = 2147483647;
        resizable_ = true;
        identifier_.reset();
        headerValue_.reset();
        headerRenderer_.reset();
        cellRenderer_.reset();
        cellEditor_.reset();
    }
}
