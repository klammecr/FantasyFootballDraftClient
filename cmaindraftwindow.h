#ifndef CMAINDRAFTWINDOW_H
#define CMAINDRAFTWINDOW_H

#include "CDraftParticipant.h"
#include "CDraftDetailsDlg.h"
#include <QMainWindow>
#include <memory>

class CDraftPlayerItemModel;
namespace Ui{class CMainDraftWindow;}

class CPlayerImportHelper;

class CMainDraftWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainDraftWindow(QWidget *parent = 0);
    virtual ~CMainDraftWindow();
protected:
    void SetupMenus();
    std::unique_ptr<CPlayerImportHelper> m_pImportHelper;
private:
    DraftType m_DraftType;
    Ui::CMainDraftWindow* ui;
    std::list<std::unique_ptr<CDraftParticipant>> m_DraftList;
    std::shared_ptr<CDraftPlayerItemModel> m_pSourceModel;
// Definition for slots
public slots:
    void HandleTabSelectionChange(int);
    bool HandlePlayerImport();
    void HandleNewDraft();
    void HandleNewDraftFinished();
    void HandleModelDataTransfer();
    void HandleDraftPlayer();
    void HandleFilterPlayers();
    void HandleSendFilterParameters();
};

#endif // CMAINDRAFTWINDOW_H
