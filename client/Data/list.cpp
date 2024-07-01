#pragma once

#include <QJsonArray>

#include <wobjectimpl.h>

#include "list.hpp"

namespace Data
{
template <typename T>
list<T>::list(QObject* parent)
    : base_data{parent}
{}

template <typename T>
QVector<T> list<T>::items() const
{
    return m_items;
}

template<typename T>
T list<T>::item_at(int index) const
{
    return m_items[index];
}

template<typename T>
int list<T>::index_at_id(int id) const noexcept
{
    int index{-1};

    for (int i = 0; i < m_items.size(); i++)
        if (m_items[i].id == id)
        {
            index = i;
            break;
        }

    return index;
}

template<typename T>
T list<T>::item_at_id(int id) const
{
    int index{index_at_id(id)};

    if (index != -1)
        return m_items[index];

    return {};
}

template <typename T>
bool list<T>::setItemAt(int index, const T& item)
{
    if (index < 0 || index >= size())
        return false;

    m_items[index] = item;

    emit dataChangedAt(index);

    return true;
}

template <typename T>
void list<T>::appendItems(int number)
{
    emit preItemsAppended(number);

    for (int i = 0; i < number; i++)
        m_items.push_back(T{});

    emit postItemsAppended();
}

template<typename T>
void list<T>::removeItems(int first, int last)
{
    emit preItemsRemoved(first, last);

    m_items.erase(m_items.begin() + first,
                        std::next(m_items.begin() + last));

    emit postItemsRemoved();
}

template<typename T>
void list<T>::removeItems(int number)
{
    const auto count = size();

    removeItems(count - number, count - 1);
}
template<typename T>
void list<T>::complitionChecks() const
{
    base_data::connect(this, &list::postItemsAppended,
                       this, &list::checkCompleted);

    base_data::connect(this, &list::postItemsRemoved,
                       this, &list::checkCompleted);

    base_data::connect(this, &list::dataChangedAt,
                       this, &list::checkCompleted);
}

template <typename T>
void list<T>::read(const QJsonArray& array)
{
    clear();
    emit preItemsAppended(array.size());

    for (const auto& json : array)
    {
        // d'ont insert null values like "empty" contacts
        if (json.isNull()) break;

        T item{};
        item.read(json);
        m_items.push_back(item);
    }

    emit postItemsAppended();
    emit loaded();
}

template <typename T>
qsizetype list<T>::size() const { return m_items.size(); }

template <typename T>
void list<T>::appendWith(const QJsonObject& json)
{
    emit preItemsAppended(1);

    T item{};
    item.read(json);
    m_items.push_back(item);

    emit postItemsAppended();
}

template<typename T>
void list<T>::appendWith(int id)
{
    emit preItemsAppended(1);

    T item{};
    item.id = id;
    m_items.push_back(item);

    emit postItemsAppended();
}

template<typename T>
bool list<T>::setItemAtId(int id, const T& item)
{
    int index{this->index_at_id(id)};

    if (index == -1)
        return false;

    m_items[index] = item;
    emit dataChangedAt(index);
    return true;
}

template<typename T>
void list<T>::set_list(const QVector<T> &list)
{
    clear();
    emit preItemsAppended(size());

    m_items = list;

    emit postItemsAppended();
}

template<typename T>
void list<T>::clear()
{
    emit preItemsRemoved(0, size() - 1);

    m_items.clear();

    emit postItemsRemoved();
}

template<typename T>
bool list<T>::is_completed() const
{
    if (m_items.empty())
        return false;

    for (const auto& item : m_items)
        if (!item.is_completed())
            return false;

    return true;
}

template<typename T>
bool list<T>::is_empty_completed() const
{
    if(m_items.empty())
        return true;

    for(const auto& item : m_items)
        if (!item.is_completed())
            return false;

    return true;
}

template<typename T>
void list<T>::checkCompleted()
{
    this->setCompleted(this->is_completed());
}

template<typename T>
void list<T>::erase(int id)
{
    int index{this->index_at_id(id)};

    if (index == -1)
        return;

    emit preItemsRemoved(index, index);

    m_items.erase(m_items.begin() + index);

    emit postItemsRemoved();
}

} // Data

W_OBJECT_IMPL(Data::list<T>, template <typename T>)
