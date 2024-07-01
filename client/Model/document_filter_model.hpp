#ifndef DOCUMENT_FILTER_MODEL_H
#define DOCUMENT_FILTER_MODEL_H

#include <QSortFilterProxyModel>
#include <wobjectdefs.h>
#include <Item/document.hpp>

namespace Data {

class document_filter_model : public QSortFilterProxyModel
{
    W_OBJECT(document_filter_model)

public:
    explicit document_filter_model(QObject* parent = nullptr);

    document::categories getCategory() const;
    void setCategory(document::categories newCategory);

    W_PROPERTY(document::categories, category READ getCategory WRITE setCategory)

private:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;

    document::categories category{document::categories::None};
};
}

#endif // DOCUMENT_FILTER_MODEL_H
