#ifndef CDRAFTBOARDITEMMODEL_H
#define CDRAFTBOARDITEMMODEL_H

#include <QAbstractTableModel>
#include "CDraftDetailsDlg.h"
#include <memory>

class CDraftParticipant;
class QModelIndex;
class QVariant;
class CDraftBoardItemModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CDraftBoardItemModel();
    virtual ~CDraftBoardItemModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex&, int) const override;
    void AddPlayer(std::shared_ptr<CDraftParticipant>);
    inline void SetLeagueName(const QString& inName){m_LeagueName = inName;}
    inline void SetNumTeams(const int inTeams){m_Teams = inTeams;}
    inline void SetRounds(const int inRounds) {m_Rounds = inRounds;}
    inline void SetDraftType(const DraftType inDraftType) { m_eDraftType = inDraftType; }
    void StructureTable();
    void SetTeamHeaders(const QStringList&);
    void ResetModel();
    QModelIndex index(int, int, const QModelIndex& = QModelIndex()) const override;
    const bool GetDraftDone() { return m_bDraftDone; }
    inline void SetByeMap (std::map<QString, int>& inMap) { m_ByeMap = inMap; }
protected:
    bool setHeaderData(int, Qt::Orientation, const QVariant&, int) override;
    QVariant headerData(int, Qt::Orientation, int) const override;
    int FindRowFromPickNumber() const;
    int FindColumnFromPickNumber() const;
    int FindPickFromRowColumn(int, int) const;
private:
    std::map<int, std::shared_ptr<CDraftParticipant>> m_SelectedPlayerMap;
    int m_Teams;
    int m_Rounds;
    QString m_LeagueName;
    DraftType m_eDraftType;
    int m_UpcomingPick;
    std::map<int, QVariant> m_HeaderMap;
    std::map<QString, int> m_ByeMap;
    bool m_bDraftDone;
};

#endif // CDRAFTBOARDITEMMODEL_H
