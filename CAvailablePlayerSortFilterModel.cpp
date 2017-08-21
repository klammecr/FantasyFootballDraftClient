#include "CAvailablePlayerSortFilterModel.h"
#include "CDraftBoardItemModel.h"
CAvailablePlayerSortFilterModel::CAvailablePlayerSortFilterModel()
{}

bool CAvailablePlayerSortFilterModel::lessThan(const QModelIndex& inLeft, const QModelIndex& inRight) const
{
    bool bLessThan = false;
    QVariant leftData = sourceModel()->data(inLeft);
    QVariant rightData = sourceModel()->data(inRight);
    if (leftData.type() == QVariant::String)
        bLessThan = QString::localeAwareCompare(leftData.toString(), rightData.toString()) < 0;
    else if (leftData.type() == QVariant::Int)
        bLessThan = leftData < rightData;
    return bLessThan;
}

bool CAvailablePlayerSortFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex&) const
{
    bool bAccepted = false;
    if (sourceModel())
        bAccepted = !(sourceModel()->data(sourceModel()->index(sourceRow, 0), Qt::UserRole).toBool());
    return bAccepted;
}

