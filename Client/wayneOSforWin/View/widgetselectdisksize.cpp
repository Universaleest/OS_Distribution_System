#include "widgetselectdisksize.h"
#include "ui_widgetselectdisksize.h"

#include "viewmanager.h"

#include "../Info/infomanager.h"

#include <QLineEdit>

#include <QDebug>

WidgetSelectDiskSize::WidgetSelectDiskSize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetSelectDiskSize)
{
    ui->setupUi(this); 


}

WidgetSelectDiskSize::~WidgetSelectDiskSize()
{
    delete ui;
}

void WidgetSelectDiskSize::on_btnPrev_clicked()
{
    ViewManager::GetInstance()->SetViewByIndex(ViewManager::WIDGET_SELECTTYPE);
}

void WidgetSelectDiskSize::on_btnNext_clicked()
{
	QString strSize = ui->comboBox->currentText();

	InfoManager::GetInstance()->mSize = (strSize.mid(0, strSize.length() - 2).toInt());
	SetWidgetPrepare();

	ViewManager::GetInstance()->SetViewByIndex(ViewManager::WIDGET_PREPARECOMPLETE);
}


void  WidgetSelectDiskSize::SetWidgetPrepare()
{
    QWidget* widgetPrepare =  ViewManager::GetInstance()->getStackedWidget()->widget(ViewManager::WIDGET_PREPARECOMPLETE);
    
	//Version Type Check
	QLineEdit* editVersion =  widgetPrepare->findChild<QLineEdit*>("editVersion");
    if((InfoManager::GetInstance()->mVersionType) == InfoManager::FREE)
    {
        editVersion->setText(QString::fromLocal8Bit("Wayne OS Free Version"));

    }
    else if((InfoManager::GetInstance()->mVersionType) == InfoManager::PAID)
    {
        editVersion->setText(QString::fromLocal8Bit("Wayne OS Paid Version"));
    }

	//Path Check
	QLineEdit* editPath =  widgetPrepare->findChild<QLineEdit*>("editPath");
    editPath->setText(InfoManager::GetInstance()->mDriveInstallPath);

	//Device Type Check
	QLineEdit* editType =  widgetPrepare->findChild<QLineEdit*>("editType");
    editType->setText(QString::fromLocal8Bit("USB Flash Drive"));
    /*if((InfoManager::GetInstance()->mDeviceType) == InfoManager::USB)
    {
        editType->setText(QString::fromLocal8Bit("USB Flash Drive"));

    }
    else if((InfoManager::GetInstance()->mDeviceType) == InfoManager::SSD)
    {
        editType->setText(QString::fromLocal8Bit("SSD"));
    }*/
	//Device Size Check
	QLineEdit* editSize = widgetPrepare->findChild<QLineEdit*>("editSize");
	editSize->setText(QString::number(InfoManager::GetInstance()->mSize) + "GB");

}


void WidgetSelectDiskSize::DownloadDeviceVolume( QStringList list)
{
	qDebug() << "list Size : " << list.size();
	for (int i = 0; i<list.size(); i++)
	{
		ui->comboBox->addItem(list.at(i) + "GB");
		ui->comboBox->setCurrentIndex(0);

	}

}

