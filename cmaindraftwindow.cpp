#include "CMainDraftWindow.h"
#include "ui_cmaindraftwindow.h"
#include "CDraftDetailsDlg.h"
#include "CPlayerImportHelper.h"
#include "CDraftParticipantList.h"
#include <QAction>
#include "CDraftBoardItemModel.h"
#include "CDraftPlayerItemModel.h"
#include "CAvailablePlayerSortFilterModel.h"
#include <memory>
#include <QMessageBox>

CMainDraftWindow::CMainDraftWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainDraftWindow)
{
    ui->setupUi(this);
    m_pImportHelper.reset(new CPlayerImportHelper());
    SetupMenus();

    // UI Fluff
    setWindowIcon(QIcon(":/res/NFL_Draft.png"));
    ui->buttonDraftPlayer->setStyleSheet("background-color: rgb(252, 232, 81);");
    ui->buttonDraftPlayer->setEnabled(false);
    // Selection Settings
    ui->m_pAllPlayersTblView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->m_pAllPlayersTblView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->m_pAvailPlayerTblView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->m_pAvailPlayerTblView->setSelectionMode(QAbstractItemView::SingleSelection);
    std::unique_ptr<CDraftBoardItemModel> pItemModel(new CDraftBoardItemModel);

    // Doing pointer resets and model setting
    m_pSourceModel.reset(new CDraftPlayerItemModel());
    pItemModel->SetByeMap(m_pSourceModel->GetByeMap());
    std::unique_ptr<CAvailablePlayerSortFilterModel> pAvailablePlayerProxy(new CAvailablePlayerSortFilterModel);
    ui->m_pDraftBoardTblView->setModel(pItemModel.release());
    ui->m_pAllPlayersTblView->setModel(m_pSourceModel.get());

    pAvailablePlayerProxy->setDynamicSortFilter(true);
    pAvailablePlayerProxy->setSourceModel(m_pSourceModel.get());
    ui->m_pAvailPlayerTblView->setModel(pAvailablePlayerProxy.release());
    connect(ui->buttonDraftPlayer, SIGNAL(clicked(bool)), this, SLOT(HandleDraftPlayer()));
    connect(m_pImportHelper.get(), SIGNAL(dataParsed()), this, SLOT(HandleModelDataTransfer()));
}

CMainDraftWindow::~CMainDraftWindow()
{
    delete ui;
}

void CMainDraftWindow::SetupMenus()
{
    QAction* pImportPlayers = new QAction("Import Players from NFL.com", this);
    QAction* pNewDraft = new QAction("Start a new Draft", this);
    ui->m_pFileMenu->addAction(pImportPlayers);
    ui->m_pFileMenu->addAction(pNewDraft);
    connect(pImportPlayers, SIGNAL(triggered()), this, SLOT(HandlePlayerImport()));
    connect(pNewDraft, SIGNAL(triggered()), this, SLOT(HandleNewDraft()));
}

const bool CMainDraftWindow::HandlePlayerImport()
{
    // Contact the nfl.com Fantasy API and get all players
    // Parse out the JSON response if it is valid
    const int playerCount = 1000;
    for(unsigned int i = 0; i < playerCount/50; i++)
       m_pImportHelper->SetPlayerData(50*i); // i is the offset, by 50
    // If not valid, we need to display the error to the client
    // Eventually this needs to pass the error from the import helper to the Client
    return true;
}

void CMainDraftWindow::HandleModelDataTransfer()
{
    // We have players to select
    ui->buttonDraftPlayer->setEnabled(true);

    if(m_pSourceModel)
    {
        std::list<std::shared_ptr<CDraftParticipant>>& participantList = m_pImportHelper->GetPlayerList();
        qDebug() << m_pImportHelper->GetPlayerSize();
        if (participantList.size() > 0)
        {
            // Sorting and good sizes for Tbl View
            m_pSourceModel->SetPlayerMap(participantList);
            m_pSourceModel->SetHeaders();
            ui->m_pAllPlayersTblView->resizeColumnsToContents();
            ui->m_pAllPlayersTblView->resizeRowsToContents();

            // Sorting and good sizes for Tbl View
            ui->m_pAvailPlayerTblView->resizeColumnsToContents();
            ui->m_pAvailPlayerTblView->resizeRowsToContents();
            ui->m_pAvailPlayerTblView->setSortingEnabled(true);
        }
        else
        {// ERROR: SHIT NO PARTICIPANTS WERE LOADED
        }
    }
    else
    {
        // Invalid Cast Error Message
    }
}

void CMainDraftWindow::HandleNewDraft()
{
    CDraftDetailsDlg* pNewDraftDlg = new CDraftDetailsDlg(this);
    connect(pNewDraftDlg, SIGNAL(accepted()), this, SLOT(HandleNewDraftFinished()));
    pNewDraftDlg->show();
}

void CMainDraftWindow::HandleNewDraftFinished()
{
    CDraftDetailsDlg* senderDlg = qobject_cast<CDraftDetailsDlg*>(sender());
    CDraftBoardItemModel* draftBoardModel = dynamic_cast<CDraftBoardItemModel*>(ui->m_pDraftBoardTblView->model());
    draftBoardModel->ResetModel();
    draftBoardModel->SetRounds(senderDlg->GetDraftRounds());
    draftBoardModel->SetNumTeams(senderDlg->GetNumTeams());
    draftBoardModel->SetLeagueName(senderDlg->GetLeagueName());
    draftBoardModel->SetDraftType(senderDlg->GetDraftType());
    m_DraftType = senderDlg->GetDraftType();
    draftBoardModel->StructureTable();
    QDialog* playerNameDialog = new QDialog(this);
    QVBoxLayout* dlgLayout = new QVBoxLayout();
    for(int i = 0; i < senderDlg->GetNumTeams(); i++)
    {
        QHBoxLayout* layout = new QHBoxLayout();
        layout->addWidget(new QLabel(QString("Player: %1").arg(i+1)));
        layout->addWidget(new QLineEdit());
        dlgLayout->addLayout(layout);
    }
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    dlgLayout->addWidget(buttonBox);
    connect(buttonBox, SIGNAL(accepted()), playerNameDialog, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), playerNameDialog, SLOT(reject()));
    playerNameDialog->setLayout(dlgLayout);
    if(playerNameDialog->exec() == QDialog::Accepted)
    {
        QStringList teamNames;
        for(QLineEdit* lineEditObject : playerNameDialog->findChildren<QLineEdit*>())
            teamNames.append(lineEditObject->text());
        draftBoardModel->SetTeamHeaders(teamNames);
    }
}

void CMainDraftWindow::HandleDraftPlayer()
{
    CDraftBoardItemModel* pDraftBoardModel = qobject_cast<CDraftBoardItemModel*>(ui->m_pDraftBoardTblView->model());
    if (pDraftBoardModel->columnCount() > 0 && pDraftBoardModel->rowCount() > 0)
    {
        bool bAreUsingAProxy = false;
        QItemSelectionModel* curSelectionModel;
        int currentTab = ui->tabWidget->currentIndex();
        if (currentTab == 2)
            curSelectionModel = ui->m_pAllPlayersTblView->selectionModel();
        else if (currentTab == 1)
        {
            curSelectionModel = ui->m_pAvailPlayerTblView->selectionModel();
            bAreUsingAProxy = true;
        }
        else
            return;
        if (curSelectionModel->hasSelection())
        {
            QModelIndexList indexList = curSelectionModel->selection().indexes();
            CAvailablePlayerSortFilterModel* proxyModel = qobject_cast<CAvailablePlayerSortFilterModel*>(ui->m_pAvailPlayerTblView->model());
            if (proxyModel)
            {
                QModelIndex indexToChange = bAreUsingAProxy ? proxyModel->mapToSource(indexList.at(0)) : indexList.at(0);
                if (pDraftBoardModel && !pDraftBoardModel->GetDraftDone())
                {
                    pDraftBoardModel->AddPlayer(m_pSourceModel->GetPlayerRef(indexToChange));
                    m_pSourceModel->MarkPlayerSelected(indexToChange);
                    ui->m_pDraftBoardTblView->resizeRowsToContents();
                    ui->m_pDraftBoardTblView->resizeColumnsToContents();
                }
            }

        }
    }
    else
    {
        QMessageBox* msg = new QMessageBox(QMessageBox::Icon::Warning, "Invalid Draft Format", "Invalid Draft Format, need at least 1 Team and 1 Round");
        msg->setParent(this);
        msg->show();
    }
}
