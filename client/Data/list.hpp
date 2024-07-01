#ifndef LIST_HPP
#define LIST_HPP

#include <wobjectdefs.h>

#include "base_data.hpp"
#include "list.hpp"

namespace Data
{
template <typename T>
class list final : public base_data
{    
    W_OBJECT(list)

public:
    list(QObject* parent = nullptr);

    static consteval auto key() { return T::key(); }
    static consteval auto qmlName() { return T::qmlName(); }
    static consteval auto uri() { return T::uri(); }

    QVector<T> items() const;
    T item_at(int index) const;
    T item_at_id(int id) const;

    bool setItemAt(int index, const T& item);

    void preItemsAppended(int number = 1)
    W_SIGNAL(preItemsAppended, number)
    void postItemsAppended()
    W_SIGNAL(postItemsAppended)

    void appendItems(int number = 1);
    W_SLOT(appendItems, (int))

    void preItemsRemoved(int first, int last)
    W_SIGNAL(preItemsRemoved, first, last)
    void postItemsRemoved()
    W_SIGNAL(postItemsRemoved)

    void dataChangedAt(int index)
    W_SIGNAL(dataChangedAt, index)

    void removeItems(int first, int last);
    W_SLOT(removeItems, (int, int))

    void removeItems(int number = 1);
    W_SLOT(removeItems, (int))

    bool setItemAtId(int id, const T& item);
    void set_list(const QVector<T>& list);
    void clear() override;
    W_INVOKABLE(clear)

    void add()
    W_SIGNAL(add)
    void addWith(const QJsonObject& obj)
    W_SIGNAL(addWith, obj)
    void addIn(int parentId)
    W_SIGNAL(addIn, parentId)
    void addInWith(int parentId, const QJsonObject& obj)
    W_SIGNAL(addInWith, parentId, obj)

    void remove(int id)
    W_SIGNAL(remove, id)

    void appendWith(int id);
    void appendWith(const QJsonObject& json);

    void erase(int id);

    void read(const QJsonArray& json);
    void write(QJsonArray& json) const;

    bool is_completed() const;

    qsizetype size() const;

    void complitionChecks() const;
    bool is_empty_completed() const;

private:
    void checkCompleted();
    int index_at_id(int id) const noexcept;

    QVector<T> m_items;
};

}

#include "list.cpp"
#endif // LIST_HPP
