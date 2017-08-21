#ifndef CPLAYERIMPORTHELPER_H
#define CPLAYERIMPORTHELPER_H

#include<memory>
#include <list>
#include <QNetworkAccessManager>

class CDraftParticipant;
class CPlayerImportHelper : public QNetworkAccessManager
{
Q_OBJECT
public:
    CPlayerImportHelper();
    virtual ~CPlayerImportHelper();
    void SetPlayerData(const int&);
    inline std::list<std::shared_ptr<CDraftParticipant>>& GetPlayerList() { return m_PlayerList; }
    inline int GetPlayerSize() {return m_PlayerList.size(); }
protected:
private:
   std::list<std::shared_ptr<CDraftParticipant>> m_PlayerList;
signals:
   // This signal is due to an issue with the request response time
   void dataParsed();
public slots:
    void SlotReadReply(QNetworkReply*);
};

#endif // CPLAYERIMPORTHELPER_H