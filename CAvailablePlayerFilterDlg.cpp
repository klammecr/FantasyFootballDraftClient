#include "CAvailablePlayerFilterDlg.h"


CAvailablePlayerFilterDlg::CAvailablePlayerFilterDlg(QWidget* parent) : QDialog(parent)
{
    ui.setupUi(this);
}

// https://en.wikipedia.org/wiki/Copy_elision
const PairedStringList CAvailablePlayerFilterDlg::GetParameterList()
{
    // Push back the current data inside the textboxes into the outputted list
    PairedStringList outList;
    QString nameTxt = ui.m_NameEdit->text();
    if (nameTxt != "")
        outList.push_back(std::pair<QString, QString>(ui.m_NameLabel->text(),nameTxt));
    QString teamTxt = ui.m_cmbTeam->currentText();
    if (teamTxt != "")
        outList.push_back(std::pair<QString, QString>(ui.m_TeamLabel->text(), teamTxt));
    QString byeTxt =  ui.m_ByeEdit->text();
    if(byeTxt != "")
        outList.push_back(std::pair<QString, QString>(ui.m_ByeLabel->text(), byeTxt));
    QString posTxt = ui.m_cmbPosition->currentText();
    if(posTxt !="")
        outList.push_back(std::pair<QString, QString>(ui.m_PositionLabel->text(), posTxt));
    return outList;
}













