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
                int ID = -1;
                QString FName = "";
                QString LName = "";
                int Rank = 0;
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
                // Construct a Draft Participant object and add it to the player list
                m_PlayerList.push_back(std::shared_ptr<CDraftParticipant>(new CDraftParticipant(ID, FName, LName, Rank, Team, Position)));
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
