#include "widgetselectversion.h"
#include "ui_widgetselectversion.h"

#include "viewmanager.h"

#include "../Info/infomanager.h"
#include "WinDpiScale/WinDpiScale.h"

WidgetSelectVersion::WidgetSelectVersion(QWidget *parent) :QWidget(parent), ui(new Ui::WidgetSelectVersion)
{
    ui->setupUi(this);
    ui->radioFree->click(); // modified by LEE jeun jeun@wayne-inc.com
}



WidgetSelectVersion::~WidgetSelectVersion()
{
    delete ui;
}

void WidgetSelectVersion::on_btnNext_clicked()
{
    if(ui->radioFree->isChecked())
    {
        InfoManager::GetInstance()->mVersionType = InfoManager::FREE;

    }
    else if(ui->radioPaid->isChecked())
    {
        InfoManager::GetInstance()->mVersionType= InfoManager::PAID;
    }


    ViewManager::GetInstance()->SetViewByIndex(ViewManager::WIDGET_SELECTDISK);
}

void WidgetSelectVersion::on_btnPrev_clicked()
{
    ViewManager::GetInstance()->SetViewByIndex(ViewManager::WIDGET_LICENSEINFO);
}
