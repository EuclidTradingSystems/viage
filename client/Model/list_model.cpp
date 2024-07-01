#pragma once
#include <wobjectimpl.h>

#include "list_model.hpp"
#include <list.hpp>

namespace Data
{
W_OBJECT_IMPL(list_model<T>, template <typename T>)

template <typename T>
list_model<T>::list_model(QObject* parent)
    : QAbstractListModel{parent}
{
}

template <typename T>
int list_model<T>::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_list)
        return 0;

    return m_list->size();
}

template <typename T>
QVariant list_model<T>::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_list)
        return QVariant{};

    const T item = m_list->items().at(index.row());
    return item.data(role);
}

template <typename T>
bool list_model<T>::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!m_list)
        return false;

    T item = m_list->items().at(index.row());
    item.setData(value, role);

    if (m_list->setItemAt(index.row(), item))
    {
        emit this->dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

template <typename T>
Qt::ItemFlags list_model<T>::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

template <typename T>
QHash<int, QByteArray> list_model<T>::roleNames() const
{
    return T::roleNames();
}

template <typename T>
void list_model<T>::setList(list<T>* newList)
{
    beginResetModel();

    if (m_list)
        m_list->disconnect(this);

    m_list = newList;

    if (m_list)
    {
        connect(m_list, &list<T>::preItemsAppended,
                this, [=](int number)
        {
            const int index = m_list->size();
            beginInsertRows(QModelIndex(), index, index + number - 1);
        });

        connect(m_list, &list<T>::postItemsAppended,
                this, [=]()
        { endInsertRows(); });

        connect(m_list, &list<T>::preItemsRemoved,
                this, [=](int first, int last)
        { beginRemoveRows(QModelIndex(), first, last); });

        connect(m_list, &list<T>::postItemsRemoved,
                this, [=]()
        { endRemoveRows(); });

        connect(m_list, &list<T>::dataChangedAt,
                this, [this](int row)
        { emit dataChanged(index(row), index(row)); });
    }

    endResetModel();
}

template<typename T>
list<T>* list_model<T>::getList() const
{
    return m_list;
}

}
