#ifndef CAVAILABLEPLAYERSORTFILTERMODEL_H
#define CAVAILABLEPLAYERSORTFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <list>
#include <map>
#include <string>
#include "stdafx.h"

class QModelIndex;
class CAvailablePlayerSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    CAvailablePlayerSortFilterModel();
    inline void FilterChanged() { invalidateFilter(); }
    void SetFilterParameters(const PairedStringList& inParameters);
    inline const PairedStringList& GetFilterParameters() {return m_FilterParameters;}
protected:
    bool lessThan(const QModelIndex&, const QModelIndex&) const override;
    bool filterAcceptsRow(int, const QModelIndex& parent = QModelIndex()) const override;
    bool filterAcceptsBasePlayer(int, const QModelIndex& parent = QModelIndex()) const;
    bool filterAcceptsQuery(int sourceRow, const QModelIndex& parent = QModelIndex()) const;
    // Member Variables

    // Variable whose purpose is to store combinations of fields to filter on
    // STRUCTURE OF PAIR : FIELD, STRING
    PairedStringList m_FilterParameters;
};

#endif // CAVAILABLEPLAYERSORTFILTERMODEL_H
