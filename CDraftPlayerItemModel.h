#ifndef CDRAFTPLAYERITEMMODEL_H
#define CDRAFTPLAYERITEMMODEL_H

#include <QAbstractTableModel>
#include <unordered_map>
#include <memory>

class CDraftParticipant;
class CDraftPlayerItemModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CDraftPlayerItemModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex&, int) const override;
    void SetPlayerMap(std::list<std::shared_ptr<CDraftParticipant>>&);
    void SetHeaders();
    QModelIndex index(int, int, const QModelIndex& parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex&) const override;
    void MarkPlayerSelected (const QModelIndex&);
    std::shared_ptr<CDraftParticipant> GetPlayerPtr(const QModelIndex&);
protected:
    bool setHeaderData(int, Qt::Orientation, const QVariant&, int) override;
    QVariant headerData(int, Qt::Orientation, int) const override;
    void ResetModel();
    bool SetDataForNewIndex(const QModelIndex&, const QVariant&, int role = Qt::EditRole);
private:
    // Internal data structure for all players stored
    std::map<int, std::shared_ptr<CDraftParticipant>> m_PlayerMap;
    QStringList m_HeaderColumnData;
};
#endif // CDRAFTPLAYERITEMMODEL_H
