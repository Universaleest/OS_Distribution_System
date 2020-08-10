#include "widgetselecttype.h"
#include "ui_widgetselecttype.h"

#include "viewmanager.h"

#include "../Info/infomanager.h"

#include <QLabel>
WidgetSelectType::WidgetSelectType(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetSelectType)
{
    ui->setupUi(this);
	ui->radioUSB->click();
}

WidgetSelectType::~WidgetSelectType()
{
    delete ui;
}

void WidgetSelectType::on_btnPrev_clicked()
{
    ViewManager::GetInstance()->SetViewByIndex(ViewManager::WIDGET_SELECTDISK);
}

void WidgetSelectType::on_btnNext_clicked()
{

    /*if(ui->radioUSB->isChecked())
    {
        InfoManager::GetInstance()->mDeviceType = InfoManager::USB;
    }
    else if(ui->radioSSD->isChecked())
    {
        InfoManager::GetInstance()->mDeviceType = InfoManager::SSD;

    }*/
    InfoManager::GetInstance()->mDeviceType = InfoManager::USB;
    ViewManager::GetInstance()->SetViewByIndex(ViewManager::WIDGET_SELECTDISKSIZE);
}
