#include "widgetpreparecomplete.h"
#include "ui_widgetpreparecomplete.h"

#include "viewmanager.h"

#include "widgetinstall.h"
#include <QMessageBox>

#include "../HTTP/httpmanager.h"

#include <QDir>


WidgetPrepareComplete::WidgetPrepareComplete(QWidget *parent) :QWidget(parent), ui(new Ui::WidgetPrepareComplete)
{
    ui->setupUi(this);
}

WidgetPrepareComplete::~WidgetPrepareComplete()
{
    delete ui;
}

void WidgetPrepareComplete::on_btnPrev_clicked()
{
    ViewManager::GetInstance()->SetViewByIndex(ViewManager::WIDGET_SELECTDISKSIZE);

}

void WidgetPrepareComplete::on_btnSetup_clicked()
{
	if (QMessageBox::warning(NULL, "Confirm Wayne OS installation", "Writing to a physical device can corrupt the device.\nAll data in your USB will be removed.\nAre you sure you want to continue?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
	{
		return;
	}

	WidgetInstall* widgetInstall = (WidgetInstall*) ViewManager::GetInstance()->getStackedWidget()->widget(ViewManager::WIDGET_INSTALL);
	
	
	ViewManager::GetInstance()->SetViewByIndex(ViewManager::WIDGET_INSTALL);

    widgetInstall->getUI()->labelStatus->setText(QString::fromLocal8Bit(""));
	widgetInstall->RequestServerData();
	//QDir dir;
	//QString path = dir.absoluteFilePath("wayneUpdateFile");
	//QMessageBox::critical(NULL, "Write Error", path);
	//ViewManager::GetInstance()->timer->start(500); // Added by LEE Jeun
}

