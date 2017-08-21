#ifndef CAVAILABLEPLAYERSORTFILTERMODEL_H
#define CAVAILABLEPLAYERSORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class QModelIndex;
class CAvailablePlayerSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    CAvailablePlayerSortFilterModel();
    inline void FilterChanged() { invalidateFilter(); }
protected:
    bool lessThan(const QModelIndex&, const QModelIndex&) const override;
    bool filterAcceptsRow(int, const QModelIndex& parent = QModelIndex()) const override;
};

#endif // CAVAILABLEPLAYERSORTFILTERMODEL_H
