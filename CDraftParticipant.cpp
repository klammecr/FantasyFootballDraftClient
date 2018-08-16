#include "CDraftParticipant.h"
#include <QString>

CDraftParticipant::CDraftParticipant(unsigned int ID, const QString& FName, const QString& LName, const QString& Team, unsigned int Bye, const QString& Position, unsigned int Rank) :
    m_ID(ID), m_FName(FName), m_LName(LName), m_Rank(Rank), m_Team(Team), m_Position(Position), m_pPicture(nullptr), m_bSelected(false), m_Bye(Bye)
{}

CDraftParticipant::~CDraftParticipant()
{}

CDraftParticipant::CDraftParticipant(const CDraftParticipant& inParticipant)
{
    m_ID = inParticipant.GetID();
    m_FName = inParticipant.GetFirst();
    m_LName = inParticipant.GetLast();
    m_Position = inParticipant.GetPosition();
    m_Rank = inParticipant.GetRank();
    m_Team = inParticipant.GetTeam();
    m_Bye = inParticipant.GetBye();
}
