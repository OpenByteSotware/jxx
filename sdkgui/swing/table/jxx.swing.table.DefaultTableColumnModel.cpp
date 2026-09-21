#include "swing/table/jxx.swing.table.DefaultTableColumnModel.h"

#include <algorithm>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "swing/event/jxx.swing.event.ChangeEvent.h"
#include "swing/event/jxx.swing.event.TableColumnModelEvent.h"
#include "swing/event/jxx.swing.event.TableColumnModelListener.h"
#include "swing/jxx.swing.DefaultListSelectionModel.h"
#include "swing/table/jxx.swing.table.TableColumn.h"

namespace jxx::swing::table
{
    DefaultTableColumnModel::DefaultTableColumnModel()
        : Super(),
          selectionModel_(
              ::jxx::NEW<::jxx::swing::DefaultListSelectionModel>())
    {
    }

    void DefaultTableColumnModel::addColumn(
        const ::jxx::Ptr<TableColumn>& column)
    {
        if (column == nullptr)
            throw ::jxx::lang::IllegalArgumentException("column");
        columns_.push_back(column);
        fireAdded(static_cast<::jxx::lang::jint>(columns_.size() - 1));
    }

    void DefaultTableColumnModel::removeColumn(
        const ::jxx::Ptr<TableColumn>& column)
    {
        const auto iterator = std::find(columns_.begin(), columns_.end(),
            column);
        if (iterator == columns_.end()) return;
        const auto index = static_cast<::jxx::lang::jint>(
            std::distance(columns_.begin(), iterator));
        columns_.erase(iterator);
        fireRemoved(index);
    }

    void DefaultTableColumnModel::moveColumn(
        ::jxx::lang::jint columnIndex,
        ::jxx::lang::jint newIndex)
    {
        const auto count = getColumnCount();
        if (columnIndex < 0 || newIndex < 0
            || columnIndex >= count || newIndex >= count)
            throw ::jxx::lang::IndexOutOfBoundsException("column");
        if (columnIndex == newIndex) return;
        const auto column = columns_[static_cast<std::size_t>(columnIndex)];
        columns_.erase(columns_.begin() + columnIndex);
        columns_.insert(columns_.begin() + newIndex, column);
        fireMoved(columnIndex, newIndex);
    }

    void DefaultTableColumnModel::setColumnMargin(
        ::jxx::lang::jint margin)
    {
        columnMargin_ = margin;
        fireMarginChanged();
    }

    ::jxx::lang::jint DefaultTableColumnModel::getColumnCount() const
    {
        return static_cast<::jxx::lang::jint>(columns_.size());
    }

    ::jxx::Ptr<TableColumn> DefaultTableColumnModel::getColumn(
        ::jxx::lang::jint columnIndex) const
    {
        if (columnIndex < 0 || columnIndex >= getColumnCount())
            throw ::jxx::lang::IndexOutOfBoundsException("column");
        return columns_[static_cast<std::size_t>(columnIndex)];
    }

    ::jxx::lang::jint DefaultTableColumnModel::getColumnIndex(
        const ::jxx::Ptr<::jxx::lang::Object>& identifier) const
    {
        if (identifier == nullptr)
            throw ::jxx::lang::IllegalArgumentException("identifier");
        for (std::size_t index = 0; index < columns_.size(); ++index)
        {
            const auto candidate = columns_[index]->getIdentifier();
            if (candidate != nullptr && candidate->equals(identifier))
                return static_cast<::jxx::lang::jint>(index);
        }
        throw ::jxx::lang::IllegalArgumentException("identifier");
    }

    ::jxx::lang::jint DefaultTableColumnModel::getColumnMargin() const
    {
        return columnMargin_;
    }

    ::jxx::lang::jint DefaultTableColumnModel::getColumnIndexAtX(
        ::jxx::lang::jint xPosition) const
    {
        if (xPosition < 0) return -1;
        ::jxx::lang::jint position = 0;
        for (std::size_t index = 0; index < columns_.size(); ++index)
        {
            position += columns_[index]->getWidth();
            if (xPosition < position)
                return static_cast<::jxx::lang::jint>(index);
        }
        return -1;
    }

    ::jxx::lang::jint DefaultTableColumnModel::getTotalColumnWidth() const
    {
        ::jxx::lang::jint width = 0;
        for (const auto& column : columns_) width += column->getWidth();
        return width;
    }

    void DefaultTableColumnModel::setColumnSelectionAllowed(
        ::jxx::lang::jbool allowed)
    {
        columnSelectionAllowed_ = allowed;
    }

    ::jxx::lang::jbool
    DefaultTableColumnModel::getColumnSelectionAllowed() const
    {
        return columnSelectionAllowed_;
    }

    ::jxx::Ptr<::jxx::swing::ListSelectionModel>
    DefaultTableColumnModel::getSelectionModel() const
    {
        return selectionModel_;
    }

    void DefaultTableColumnModel::setSelectionModel(
        const ::jxx::Ptr<::jxx::swing::ListSelectionModel>& model)
    {
        if (model == nullptr)
            throw ::jxx::lang::IllegalArgumentException("model");
        selectionModel_ = model;
    }

    void DefaultTableColumnModel::addColumnModelListener(
        const ::jxx::Ptr<
            ::jxx::swing::event::TableColumnModelListener>& listener)
    {
        if (listener != nullptr) listeners_.push_back(listener);
    }

    void DefaultTableColumnModel::removeColumnModelListener(
        const ::jxx::Ptr<
            ::jxx::swing::event::TableColumnModelListener>& listener)
    {
        listeners_.erase(std::remove(listeners_.begin(), listeners_.end(),
            listener), listeners_.end());
    }

    void DefaultTableColumnModel::fireAdded(::jxx::lang::jint index)
    {
        auto event = ::jxx::NEW<
            ::jxx::swing::event::TableColumnModelEvent>(
                ::jxx::CAST<TableColumnModel>(thisPtr()), 0, index);
        const auto listeners = listeners_;
        for (const auto& listener : listeners)
            if (listener != nullptr) listener->columnAdded(event);
    }

    void DefaultTableColumnModel::fireRemoved(::jxx::lang::jint index)
    {
        auto event = ::jxx::NEW<
            ::jxx::swing::event::TableColumnModelEvent>(
                ::jxx::CAST<TableColumnModel>(thisPtr()), index, 0);
        const auto listeners = listeners_;
        for (const auto& listener : listeners)
            if (listener != nullptr) listener->columnRemoved(event);
    }

    void DefaultTableColumnModel::fireMoved(
        ::jxx::lang::jint from,
        ::jxx::lang::jint to)
    {
        auto event = ::jxx::NEW<
            ::jxx::swing::event::TableColumnModelEvent>(
                ::jxx::CAST<TableColumnModel>(thisPtr()), from, to);
        const auto listeners = listeners_;
        for (const auto& listener : listeners)
            if (listener != nullptr) listener->columnMoved(event);
    }

    void DefaultTableColumnModel::fireMarginChanged()
    {
        auto event = ::jxx::NEW<::jxx::swing::event::ChangeEvent>(
            ::jxx::CAST<::jxx::lang::Object>(thisPtr()));
        const auto listeners = listeners_;
        for (const auto& listener : listeners)
            if (listener != nullptr) listener->columnMarginChanged(event);
    }

    void DefaultTableColumnModel::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&)
    {
    }

    void DefaultTableColumnModel::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>&)
    {
        listeners_.clear();
    }

    void DefaultTableColumnModel::readObjectNoData()
    {
        columns_.clear();
        listeners_.clear();
        selectionModel_ =
            ::jxx::NEW<::jxx::swing::DefaultListSelectionModel>();
        columnMargin_ = 1;
        columnSelectionAllowed_ = false;
    }
}
