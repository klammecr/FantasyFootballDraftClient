#include "CDraftPlayerItemModel.h"
#include "CDraftParticipant.h"
#include <list>
#include <QDebug>
#include <QIcon>
#include <QFont>
//*******************************************************************
// Function: CDraftPlayerItemModel
//
// Description: Default Constructor
//*******************************************************************
CDraftPlayerItemModel::CDraftPlayerItemModel()
{
    QStringList weekFive;
    weekFive << "ATL" << "DEN" << "NO"<< "WAS";
    for(QString m : weekFive)
        m_ByeWeekMap.insert(std::make_pair(m, 5));

    QStringList weekSix;
    weekSix << "BUF" << "CIN" << "DAL" <<"SEA";
    for(QString m : weekSix)
        m_ByeWeekMap.insert(std::make_pair(m, 6));

    QStringList weekSeven;
    weekSeven << "DET" << "HOU";
    for(QString m : weekSeven)
        m_ByeWeekMap.insert(std::make_pair(m, 7));

    QStringList weekEight;
    weekEight << "ARI" << "GB" << "JAX" << "LA" << "NYG" << "TEN";
    for(QString m : weekEight)
        m_ByeWeekMap.insert(std::make_pair(m, 8));

    QStringList weekNine;
    weekNine << "CHI" << "CLE" << "LAC" << "MIN" << "NE" << "PIT";
    for(QString m : weekNine)
        m_ByeWeekMap.insert(std::make_pair(m, 9));

    QStringList weekTen;
    weekTen << "BAL" << "KC" << "OAK" << "PHI";
    for(QString m : weekTen)
        m_ByeWeekMap.insert(std::make_pair(m, 10));

    QStringList weekEleven;
    weekEleven << "CAR" << "IND" << "MIA" << "NYJ" << "SF" << "TB";
    for(QString m : weekEleven)
        m_ByeWeekMap.insert(std::make_pair(m, 11));

    m_HeaderColumnData << "Team" << "Rank" << "Name" << "Position" << "Bye Week" << "Database ID";
}

int CDraftPlayerItemModel::rowCount(const QModelIndex&) const
{
    return m_PlayerMap.size();
}

int CDraftPlayerItemModel::columnCount(const QModelIndex&) const
{
    int columnCount = 0;
    if(m_PlayerMap.size())
        columnCount = (*m_PlayerMap.begin()).second->GetDisplayColumnNumber();
    return columnCount;
}

QVariant CDraftPlayerItemModel::data(const QModelIndex& inIndex, int inRole) const
{
    QVariant returnVal = QVariant();
    int internalID = inIndex.internalId();
    CDraftParticipant* pPlayer = nullptr;
    if (m_PlayerMap.find(internalID) != m_PlayerMap.end())
    {
        pPlayer = m_PlayerMap.at(internalID).get();;
    }
    if (inRole == Qt::UserRole)
    {
        if (pPlayer)
            returnVal = pPlayer->GetIsSelected();
    }
    else if (inRole == Qt::FontRole)
    {
        QFont font;
        if (pPlayer)
        {
            if (pPlayer->GetIsSelected())
                font.setStrikeOut(true);
        }
        returnVal = font;
    }
    else if (inRole == Qt::ForegroundRole)
    {
        QColor color = Qt::black;
        if (pPlayer)
        {
            if (pPlayer->GetIsSelected())
                color = Qt::red;
        }
        return color;
    }
    else if (inRole == Qt::TextAlignmentRole)
        returnVal = Qt::AlignCenter;
    else if (inIndex.isValid())
    {
        int curCol = inIndex.column();
        if (curCol < columnCount())
        {
            QString desiredData = m_HeaderColumnData.at(curCol);
            int internalID = inIndex.internalId();
            if (m_PlayerMap.find(internalID) != m_PlayerMap.end())
            {
                CDraftParticipant* pPlayer = m_PlayerMap.at(internalID).get();
                if (desiredData == "Rank" && inRole == Qt::DisplayRole)
                    returnVal = pPlayer->GetRank();
                else if (desiredData == "Name" && inRole == Qt::DisplayRole)
                {
                    returnVal = pPlayer->GetFirst() +
                            QString(" ") +
                            pPlayer->GetLast();
                }
                else if (desiredData == "Position" && inRole == Qt::DisplayRole)
                    returnVal = pPlayer->GetPosition();
                else if (desiredData == "Team" && inRole == Qt::DecorationRole)
                    returnVal = QPixmap(QString(":/res/%1.png").arg(pPlayer->GetTeam()));
                else if (desiredData == "Bye Week" && inRole == Qt::DisplayRole && m_ByeWeekMap.find(pPlayer->GetTeam()) != m_ByeWeekMap.end())
                {
                    returnVal = m_ByeWeekMap.at(pPlayer->GetTeam());
                }
                else if (desiredData == "Database ID"  && inRole == Qt::DisplayRole)
                    returnVal = pPlayer->GetID();
            }
        }
    }
    return returnVal;
}

bool CDraftPlayerItemModel::setHeaderData(int inSection, Qt::Orientation inOrientation, const QVariant& inValue, int inRole)
{
    return QAbstractItemModel::setHeaderData(inSection, inOrientation, inValue, inRole);
}

QVariant CDraftPlayerItemModel::headerData(int inSection, Qt::Orientation inOrientation, int inRole) const
{
    QVariant returnVal = QVariant();
    if (inRole == Qt::DisplayRole && inOrientation == Qt::Orientation::Horizontal && columnCount())
        returnVal = m_HeaderColumnData.at(inSection);
    else if (inOrientation == Qt::Orientation::Vertical)
        ; // We just don't want to call the base class
    else
        returnVal = QAbstractItemModel::headerData(inSection, inOrientation, inRole);
    return returnVal;
}

void CDraftPlayerItemModel::SetHeaders()
{
    for (int i = 0; i < columnCount(); i++)
        setHeaderData(i, Qt::Orientation::Horizontal, m_HeaderColumnData.at(i), Qt::EditRole);
    emit headerDataChanged(Qt::Orientation::Horizontal, 0, columnCount() - 1);
    emit headerDataChanged(Qt::Orientation::Vertical, 0, rowCount() - 1);
}

void CDraftPlayerItemModel::SetPlayerMap(std::list<std::shared_ptr<CDraftParticipant>>& inList)
{
    ResetModel();
    if (inList.size())
    {
        beginInsertRows(QModelIndex(), 0, inList.size() - 1);
        insertRows(0, rowCount(), QModelIndex());
        endInsertRows();
        beginInsertColumns(QModelIndex(), 0, inList.front()->GetDisplayColumnNumber()-1);
        insertColumns(0, columnCount(), QModelIndex());
        endInsertColumns();

        for (auto& player : inList)
        {
            // Store Index based on row column and ID (Rank)
            m_PlayerMap.insert(std::make_pair(player->GetRank(), player));
            index( player->GetRank() - 1, 0);
        }
        emit dataChanged(index(0, 0), index(rowCount()-1, columnCount()-1));
    }
}

QModelIndex CDraftPlayerItemModel::index(int row, int col, const QModelIndex& parent) const
{
    QModelIndex returnIdx = QModelIndex();
    if (hasIndex(row, col, parent))
        returnIdx = createIndex(row, col, row + 1);
    return returnIdx;
}

Qt::ItemFlags CDraftPlayerItemModel::flags(const QModelIndex& inIndex) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void CDraftPlayerItemModel::ResetModel()
{
    beginResetModel();
    m_PlayerMap.clear(); // reset internal data structure for selected players
    endResetModel();
}

void CDraftPlayerItemModel::MarkPlayerSelected(const QModelIndex& inIndex)
{
    if (m_PlayerMap.find(inIndex.internalId()) != m_PlayerMap.end() && !m_PlayerMap.at(inIndex.internalId())->GetIsSelected())
    {
        m_PlayerMap.at(inIndex.internalId())->SetSelected(true);
        emit dataChanged(inIndex, inIndex);
    }
}

std::shared_ptr<CDraftParticipant>& CDraftPlayerItemModel::GetPlayerRef(const QModelIndex& inIndex)
{
    if (inIndex.isValid())
    {
        if (m_PlayerMap.find(inIndex.internalId()) != m_PlayerMap.end())
        {
            return m_PlayerMap.at(inIndex.internalId());
        }
    }
    std::shared_ptr<CDraftParticipant> returnPtr;
    return returnPtr;
}
