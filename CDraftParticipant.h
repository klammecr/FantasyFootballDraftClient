#ifndef CDRAFTPARTICIPANT_H
#define CDRAFTPARTICIPANT_H

#include <QString>
class QUrl;

class CDraftParticipant
{
public:
    CDraftParticipant(unsigned int ID, const QString& FName, const QString& LName, const QString& Team, unsigned int Bye, const QString& Position, unsigned int Rank);
    CDraftParticipant(const CDraftParticipant&);
    virtual ~CDraftParticipant();
    inline unsigned int GetID() const { return m_ID; }
    inline QString GetFirst() const { return m_FName; }
    inline QString GetLast() const { return m_LName; }
    inline unsigned int GetRank() const { return m_Rank; }
    inline QString GetTeam() const { return m_Team; }
    inline QString GetPosition() const { return m_Position; }
    inline int GetDisplayColumnNumber() const { return 6; }
    inline bool GetIsSelected() const { return m_bSelected; }
    inline void SetSelected(const bool inSelection) { m_bSelected = inSelection; }
    inline unsigned int GetBye() const { return m_Bye; }
protected:
    unsigned int m_ID;
    QString m_FName;
    QString m_LName;
    unsigned int m_Rank;
    QString m_Team;
    QString m_Position;
    QUrl* m_pPicture;
    bool m_bSelected;
    unsigned int m_Bye;
private:
};

#endif // CDRAFTPARTICIPANT_H
