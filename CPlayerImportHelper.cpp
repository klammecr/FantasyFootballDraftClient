#include "CPlayerImportHelper.h"
#include "CDraftParticipant.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <rapidjson/reader.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <memory>
#include <iostream>

static const char* kTypeNames[] =
    { "Null", "False", "True", "Object", "Array", "String", "Number" };
//QNetworkReply* networkManger->get(QNetworkRequest(QUrl("http://api.fantasy.nfl.com/v1/players/editordraftranks")));
// http://api.fantasy.nfl.com/v1/players/editordraftranks?offset= i*50
CPlayerImportHelper::CPlayerImportHelper()
{
}

CPlayerImportHelper::~CPlayerImportHelper()
{}

void CPlayerImportHelper::SetPlayerData(const int& offset)
{
    // CK HACK: Each time we start a new import just empty old list
    if(!offset)
        m_PlayerList.clear();
    get(QNetworkRequest(QUrl(QString("http://api.fantasy.nfl.com/v1/players/editordraftranks?format=json&offset=%1").arg(offset))));
    connect(this, SIGNAL(finished(QNetworkReply*)), this, SLOT(SlotReadReply(QNetworkReply*)));
}

void CPlayerImportHelper::SlotReadReply(QNetworkReply* pInResponse)
{
    QString jsonStr = pInResponse->readAll();
    qDebug() << jsonStr;
    rapidjson::Document document;
    document.Parse(jsonStr.toStdString().c_str());
    if(!document.HasParseError())
    {
        const rapidjson::Value& playerArray = document["players"];
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
                m_PlayerList.push_back(std::move(std::shared_ptr<CDraftParticipant>(new CDraftParticipant(ID, FName, LName, Rank, Team, Position))));
        }
        qDebug() << m_PlayerList.size();
        emit dataParsed();
    }
    else
    {
       qDebug() << rapidjson::GetParseError_En(document.GetParseError());
    }

}
