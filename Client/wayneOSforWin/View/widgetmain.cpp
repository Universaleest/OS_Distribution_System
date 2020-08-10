#include "widgetmain.h"
#include "ui_widgetmain.h"

#include "viewmanager.h"

#include "../HTTP/urlstrings.h"

#include <QMessageBox>
#include <QWebFrame>
#include <QWebPage>

#include <QApplication>
#include "../HTTP/httpmanager.h"
#include "widgetselectdisksize.h"


WidgetMain::WidgetMain(QWidget *parent) : QWidget(parent), ui(new Ui::WidgetMain)
{
    ui->setupUi(this);

    UrlStrings urlstrings;
    ui->webView->load(urlstrings.URLMainInfo);

}

WidgetMain::~WidgetMain()
{
    delete ui;
}

void WidgetMain::on_btnStart_clicked()
{
    ViewManager::GetInstance()->SetViewByIndex(ViewManager::WIDGET_LICENSEINFO);
}

void WidgetMain::on_webView_loadFinished(bool arg1)
{

    QString html = ui->webView->page()->mainFrame()->toPlainText();

    if(html =="")
    {
        QMessageBox::critical(NULL, "Connect Error", QString("can not connect to Server.."));
        QApplication::exit();
    }
    else
    {
        ui->btnStart->setEnabled(true);
        //QObject::connect(&(HttpManager::GetInstance()->httpThread),SIGNAL(DonwloadStatus(int,int)),this,SLOT(DonwloadStatus(int,int)));

        WidgetSelectDiskSize* widgetSelectDiskSize =  (WidgetSelectDiskSize*)ViewManager::GetInstance()->getStackedWidget()->widget(ViewManager::WIDGET_SELECTDISKSIZE);

        QObject::connect(&HttpManager::GetInstance()->httpThread,SIGNAL(DownloadDeviceVolume(QStringList)),widgetSelectDiskSize,SLOT(DownloadDeviceVolume(QStringList)));

        HttpManager::GetInstance()->RequestDeviceVolume();




    }




}
