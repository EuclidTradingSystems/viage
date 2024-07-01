#include <wobjectimpl.h>

#include "document_filter_model.hpp"

namespace Data
{
W_OBJECT_IMPL(document_filter_model);

document_filter_model::document_filter_model(QObject* parent)
    : QSortFilterProxyModel{parent}
{
}

bool document_filter_model::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const auto& cat{sourceModel()->data(
                    sourceModel()->index(sourceRow, 0, sourceParent),
                    document::CategoryRole)};

    return cat.toInt() == category;
}

bool document_filter_model::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
    return true;
}

document::categories document_filter_model::getCategory() const
{
    return category;
}

void document_filter_model::setCategory(document::categories newCategory)
{
    category = newCategory;
}

}
