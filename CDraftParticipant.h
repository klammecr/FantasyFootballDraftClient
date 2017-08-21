#ifndef CDRAFTPARTICIPANT_H
#define CDRAFTPARTICIPANT_H

#include <QString>
class QUrl;

class CDraftParticipant
{
public:
    CDraftParticipant(int, const QString&, const QString&, int, const QString&, const QString&);
    CDraftParticipant(const CDraftParticipant&);
    virtual ~CDraftParticipant();
    inline int GetID() const { return m_ID; }
    inline QString GetFirst() const { return m_FName; }
    inline QString GetLast() const { return m_LName; }
    inline int GetRank() const { return m_Rank; }
    inline QString GetTeam() const { return m_Team; }
    inline QString GetPosition() const { return m_Position; }
    inline int GetDisplayColumnNumber() const { return 6; }
    inline const bool GetIsSelected() const { return m_bSelected; }
    inline void SetSelected(const bool inSelection) { m_bSelected = inSelection; }
protected:
    int m_ID;
    QString m_FName;
    QString m_LName;
    int m_Rank;
    QString m_Team;
    QString m_Position;
    QUrl* m_pPicture;
    bool m_bSelected;
private:
};

#endif // CDRAFTPARTICIPANT_H
