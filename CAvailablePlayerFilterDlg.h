#ifndef CAVAILABLEPLAYERFILTERDLG_H
#define CAVAILABLEPLAYERFILTERDLG_H

#include "stdafx.h"
#include "ui_cavailableplayerfilterdlg.h"

class QWidget;
class CAvailablePlayerFilterDlg : public QDialog
{
    Q_OBJECT
public:
    CAvailablePlayerFilterDlg(QWidget* parent = 0);
    const PairedStringList GetParameterList();
private:
    Ui::CAvailablePlayerFilterDlg ui;
};

#endif // CAVAILABLEPLAYERFILTERDLG_H
