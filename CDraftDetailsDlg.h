#ifndef DRAFTDETAILSDLG_H
#define DRAFTDETAILSDLG_H

#include "ui_draftdetailsdlg.h"
enum DraftType
{
    eNormal = 0,
    eSnake,
    eAuction
};

class QWidget;
class QString;
class CDraftDetailsDlg : public QDialog
{
    Q_OBJECT
public:
    CDraftDetailsDlg(QWidget* parent = 0);
    inline int GetDraftRounds() const { return ui.m_pNumberRounds->value(); }
    inline int GetNumTeams() const { return ui.m_pLeagueSize->value(); }
    inline DraftType GetDraftType() const {return static_cast<DraftType>(ui.radSnakeDraft->isEnabled());}
    inline QString GetLeagueName() const { return ui.m_pLeagueNameEdit->text(); }
private:
    Ui::DraftDetailsDlg ui;
};

#endif // DRAFTDETAILSDLG_H
