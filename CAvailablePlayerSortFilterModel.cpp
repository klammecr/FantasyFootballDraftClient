#include "CAvailablePlayerSortFilterModel.h"

#include "CDraftBoardItemModel.h"
#include "CDraftPlayerItemModel.h"
#include "CDraftParticipant.h"
// Fields in the model a user is able to filter on

// TODO :
// There might be a better way to do this, we might want to construct the dialog based on this vector
// instead of just hardcoding each of these in and giving it a desired input methodolgy
static const std::vector<QString> filterableFields = {"Name:", "Position:", "Team:", "Bye:"};

CAvailablePlayerSortFilterModel::CAvailablePlayerSortFilterModel()
{
    m_FilterParameters = PairedStringList();
}

bool CAvailablePlayerSortFilterModel::lessThan(const QModelIndex& inLeft, const QModelIndex& inRight) const
{
    return QSortFilterProxyModel::lessThan(inLeft, inRight);
}

// Filter out selected players
bool CAvailablePlayerSortFilterModel::filterAcceptsBasePlayer(int sourceRow, const QModelIndex&) const
{
    bool bAccepted = false;
    if (sourceModel())
        bAccepted = !(sourceModel()->data(sourceModel()->index(sourceRow, 0), Qt::UserRole).toBool());
    return bAccepted;
}

bool CAvailablePlayerSortFilterModel::filterAcceptsQuery(int sourceRow, const QModelIndex&) const
{
    bool bAccepted = true;
    CDraftPlayerItemModel* src = qobject_cast<CDraftPlayerItemModel*>(sourceModel());
    for(auto i : m_FilterParameters)
    {
        if (bAccepted)
        {
            // Get the raw pointer
            CDraftParticipant* player = src->GetPlayerPtr(src->index(sourceRow, 0)).get();
            if (i.first == filterableFields[0])
            {
                bAccepted = (player->GetFirst().contains(i.second) ||
                             player->GetLast().contains(i.second));
            }
            else if(i.first == filterableFields[1])
                bAccepted = (player->GetPosition() == i.second);
            else if(i.first == filterableFields[2])
                bAccepted = (player->GetTeam() == i.second);
            else if(i.first == filterableFields[3])
                bAccepted = (player->GetBye() == i.second);
            else
            {
                // TODO : Invalid
            }
        }
    }
    return bAccepted;
}

// Set what to filter on
void CAvailablePlayerSortFilterModel::SetFilterParameters(const PairedStringList& inParameters)
{
    // Acceptable list of parameters
    PairedStringList outParameters;
    // for each parameter pair
    for(auto p : inParameters)
    {
        // boop boop, see if our filter parameter is in the list of accepted fields, remove those who don't comply
        if(std::find(std::begin(filterableFields), std::end(filterableFields), p.first) != std::end(filterableFields))
            outParameters.push_back(p);
    }
    // Move the resources of the parameters set in the dialog to the model
    m_FilterParameters = std::move(outParameters);
    invalidateFilter();

}

bool CAvailablePlayerSortFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex& index) const
{
    return filterAcceptsBasePlayer(sourceRow, index) && filterAcceptsQuery(sourceRow, index);
}


