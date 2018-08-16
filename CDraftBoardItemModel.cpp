#include "CDraftBoardItemModel.h"
#include "CDraftParticipant.h"
#include "CDraftDetailsDlg.h"
#include "assert.h"
#include <QModelIndex>
#include <QDebug>
#include <QColor>
#include <unordered_map>

CDraftBoardItemModel::CDraftBoardItemModel()
{
    m_UpcomingPick = 1;
    m_Teams = 0;
    m_Rounds = 0;
    m_bDraftDone = false;
}

CDraftBoardItemModel::~CDraftBoardItemModel()
{}

int CDraftBoardItemModel::rowCount(const QModelIndex& parent) const
{
    return m_Rounds;
}

int CDraftBoardItemModel::columnCount(const QModelIndex& parent) const
{
    return m_Teams;
}

QVariant CDraftBoardItemModel::data(const QModelIndex& inIndex, int inRole) const
{
    QVariant returnVal = QVariant();
    if(inIndex.isValid() && inIndex.internalPointer() && m_SelectedPlayerMap.find(FindPickFromRowColumn(inIndex.row(), inIndex.column())) != m_SelectedPlayerMap.end())
    {
        CDraftParticipant* pParticipant = m_SelectedPlayerMap.at(FindPickFromRowColumn(inIndex.row(), inIndex.column())).get();
        QString participantPosition = pParticipant->GetPosition();
        if (inRole == Qt::DisplayRole)
        {
            // First Last \n Position \n Bye: Number
            QString outData = QString("%1 %2\n%3\nBye: %4").arg(pParticipant->GetFirst())
                    .arg(pParticipant->GetLast())
                    .arg(participantPosition)
                    .arg(pParticipant->GetBye());
            returnVal = outData;
        }
        else if (inRole == Qt::BackgroundColorRole)
        {
            if (participantPosition == "QB")
                returnVal = QColor(Qt::red);
            else if (participantPosition == "RB")
                returnVal = QColor(Qt::blue);
            else if (participantPosition == "WR")
                returnVal = QColor(Qt::darkGreen);
            else if (participantPosition == "TE")
                returnVal = QColor(Qt::magenta);
            else if (participantPosition == "DEF")
                returnVal = QColor(Qt::gray);
            else
                returnVal = QColor(Qt::white);

        }
    }
    return returnVal;
}

QModelIndex CDraftBoardItemModel::index(int row, int col, const QModelIndex& parent) const
{
    QModelIndex returnIdx = QModelIndex();
    if (hasIndex(row, col, parent) && m_SelectedPlayerMap.size())
        returnIdx = createIndex(row, col, m_UpcomingPick);
    return returnIdx;
}


void CDraftBoardItemModel::AddPlayer(std::shared_ptr<CDraftParticipant> inParticipant)
{
    if (inParticipant != nullptr && m_UpcomingPick < (rowCount() * columnCount() + 1) && !inParticipant->GetIsSelected())
    {
        m_SelectedPlayerMap.insert(std::make_pair(m_UpcomingPick, inParticipant));
        // add some logic regarding figuring out row and column
        QModelIndex changedIndex = index(FindRowFromPickNumber(), FindColumnFromPickNumber());
        m_UpcomingPick++;
        if (m_UpcomingPick == rowCount() * columnCount() + 1)
            m_bDraftDone = true;
        emit dataChanged(changedIndex, changedIndex);
    }
}

void CDraftBoardItemModel::StructureTable()
{
    beginInsertRows(QModelIndex(), 0, m_Rounds - 1);
    insertRows(0, m_Rounds, QModelIndex());
    endInsertRows();
    beginInsertColumns(QModelIndex(), 0, m_Teams - 1);
    insertColumns(0, m_Teams, QModelIndex());
    endInsertColumns();
}

void CDraftBoardItemModel::SetTeamHeaders(const QStringList& inList)
{
    for (int i = 0; i < m_Teams; i++)
        setHeaderData(i, Qt::Horizontal, inList.at(i), Qt::EditRole);
    emit headerDataChanged(Qt::Horizontal, 0, m_Teams);
}

bool CDraftBoardItemModel::setHeaderData(int inSection, Qt::Orientation inOrientation, const QVariant& inValue, int inRole)
{
    bool returnVal = false;
    if(inRole == Qt::EditRole && inOrientation == Qt::Orientation::Horizontal)
    {
        auto returnPair= m_HeaderMap.insert_or_assign(inSection, inValue);
        returnVal = returnPair.second;
    }
    else
        returnVal = QAbstractItemModel::setHeaderData(inSection, inOrientation, inValue, inRole);
    return returnVal;
}

QVariant CDraftBoardItemModel::headerData(int inSection, Qt::Orientation inOrientation, int inRole) const
{
    QVariant returnVar = QVariant();
    if(inRole == Qt::DisplayRole && inOrientation == Qt::Orientation::Horizontal && m_HeaderMap.find(inSection) != m_HeaderMap.end())
        returnVar = m_HeaderMap.at(inSection);
    else if(inRole == Qt::DisplayRole && inOrientation == Qt::Orientation::Vertical)
        returnVar = QVariant(QString("Round ") + QAbstractItemModel::headerData(inSection, inOrientation, inRole).toString());
    else
        returnVar = QAbstractItemModel::headerData(inSection, inOrientation, inRole);
    return returnVar;
}

void CDraftBoardItemModel::ResetModel()
{
    beginResetModel();
    m_HeaderMap.clear();
    // reset internal data structure for selected players
    endResetModel();
}

int CDraftBoardItemModel::FindRowFromPickNumber() const
{
    return ((m_UpcomingPick - 1) / m_Teams);
}

int CDraftBoardItemModel::FindColumnFromPickNumber() const
{
    int outColumn = -1;
    int baseColumn = (m_UpcomingPick - 1) % m_Teams;
    if (m_eDraftType == DraftType::eNormal)
    {
        outColumn = baseColumn;
    }
    else if (m_eDraftType == DraftType::eSnake)
    {
        // Even
        if (FindRowFromPickNumber() % 2 == 0)
            outColumn = baseColumn;
        // Odd
        else
            outColumn = m_Teams - 1 - baseColumn;
    }
    else
        assert(false); // need to add a new draft type
    return outColumn;
}

int CDraftBoardItemModel::FindPickFromRowColumn(int inRow, int inColumn) const
{
    int pickNum = -1;
    int  basePick = ((inRow * columnCount()) + inColumn) + 1;
    if (m_eDraftType == DraftType::eNormal)
    {
        // No need for anything special
        pickNum = basePick;
    }
    else if (m_eDraftType == DraftType::eSnake)
    {
        // If we are in an odd row:
        // Column : Number of Teams - base column
        // Row    : Number of teams * base row
        if (inRow % 2 == 1)
        {
            pickNum = (inRow * columnCount()) + (columnCount() - inColumn);
        }
        // Even
        else
            pickNum = basePick;
    }
    else
        // TODO : ADD ERROR HERE, right now we just hard assert,
        // ERROR : ADD DraFT OPTION
        assert(false);
    return pickNum;
}
