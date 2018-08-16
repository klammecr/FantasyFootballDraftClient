#include "CPlayerImportHelper.h"
#include "CDraftParticipant.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <rapidjson/reader.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <memory>
#include <iostream>

CPlayerImportHelper::CPlayerImportHelper()
{
    // For now this will do... will need signals and slots if we make an api request for it for future
    SetByeWeekData();
}

CPlayerImportHelper::~CPlayerImportHelper()
{}

void CPlayerImportHelper::SetPlayerData(const int offset)
{
    // CK: Each time we start a new import just empty old list
    if(!offset)
        m_PlayerList.clear();
    // Ping the server address and get the data for the players
    get(QNetworkRequest(QUrl(QString("http://api.fantasy.nfl.com/v1/players/editordraftranks?format=json&offset=%1").arg(offset))));
    // When the network reply is received run SlotReadReply
    connect(this, SIGNAL(finished(QNetworkReply*)), this, SLOT(SlotReadReply(QNetworkReply*)));
}

// A hard coded stringlist of various bye weeks
// If this ever goes to enterprise then hit an API for this info but I don't want to pay for it
void CPlayerImportHelper::SetByeWeekData()
{
    QStringList weekFour;
    weekFour << "CAR" << "WAS";
    for(QString m : weekFour)
        m_ByeWeekMap.insert(std::make_pair(m, 4));

    QStringList weekFive;
    weekFive << "CHI" << "TB";
    for(QString m : weekFive)
        m_ByeWeekMap.insert(std::make_pair(m, 5));

    QStringList weekSix;
    weekSix << "DET" << "NO";
    for(QString m : weekSix)
        m_ByeWeekMap.insert(std::make_pair(m, 6));

    QStringList weekSeven;
    weekSeven << "GB" << "OAK" << "PIT" << "SEA";
    for(QString m : weekSeven)
        m_ByeWeekMap.insert(std::make_pair(m, 7));

    QStringList weekEight;
    weekEight << "ATL" << "DAL" << "LAC" << "TEN";
    for(QString m : weekEight)
        m_ByeWeekMap.insert(std::make_pair(m, 8));

    QStringList weekNine;
    weekNine << "ARI" << "CIN" << "IND" << "JAX" << "NYG" << "PHI";
    for(QString m : weekNine)
        m_ByeWeekMap.insert(std::make_pair(m, 9));

    QStringList weekTen;
    weekTen << "BAL" << "DEN" << "MIN" << "HOU";
    for(QString m : weekTen)
        m_ByeWeekMap.insert(std::make_pair(m, 10));

    QStringList weekEleven;
    weekEleven << "BUF" << "CLE" << "MIA" << "NYJ" << "SF" << "NE";
    for(QString m : weekEleven)
        m_ByeWeekMap.insert(std::make_pair(m, 11));

    QStringList weekTwelve;
    weekTwelve << "KC" << "LA";
    for(QString m : weekTwelve)
        m_ByeWeekMap.insert(std::make_pair(m, 12));
}

void CPlayerImportHelper::SlotReadReply(QNetworkReply* pInResponse)
{
    // Read JSON response
    QString jsonStr = pInResponse->readAll();
    qDebug() << jsonStr;
    rapidjson::Document document;
    // Parse the JSON for validity
    document.Parse(jsonStr.toStdString().c_str());
    if(!document.HasParseError())
    {
        const rapidjson::Value& playerArray = document["players"];
        // TODO : Add an error here, is assert
        assert(playerArray.IsArray());
        for (rapidjson::SizeType i = 0; i < playerArray.Size(); i++) // Uses SizeType instead of size_t
        {
                const rapidjson::Value& playerAttributes = playerArray[i];
                unsigned int ID = 0;
                QString FName = "";
                QString LName = "";
                unsigned int Rank = 0;
                QString Team = "";
                QString Position = "";
                // Run an iterator starting at the first player attribute
                for (rapidjson::Value::ConstMemberIterator itr = playerAttributes.MemberBegin();
                    itr != playerAttributes.MemberEnd(); ++itr)
                {
                    if(itr->value.IsString())
                    {
                       std::string member = itr->value.GetString();
                       if(itr->name == "id")
                          ID = std::stoi(member);
                       else if(itr->name == "firstName")
                          FName = member.c_str();
                       else if(itr->name == "lastName")
                          LName = member.c_str();
                       else if(itr->name == "rank")
                          Rank = std::stoi(member);
                       else if(itr->name == "teamAbbr")
                          Team = member.c_str();
                       else if(itr->name == "position")
                           Position = member.c_str();
                    }
                }
                // Set bye week based on mapping of team to bye week
                unsigned int Bye = 0;
                if (m_ByeWeekMap.find(Team) != m_ByeWeekMap.end())
                    Bye = m_ByeWeekMap[Team];
                // Construct a Draft Participant object and add it to the player list
                m_PlayerList.push_back(std::shared_ptr<CDraftParticipant>(new CDraftParticipant(ID, FName, LName, Team, Bye, Position, Rank)));
        }
        // Let everyone else know we are finished loading the data
        emit dataParsed();
    }
    else
    {
       // TODO : Add an error dialogue here
       qDebug() << rapidjson::GetParseError_En(document.GetParseError());
    }

}
