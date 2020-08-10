
#include "widgetlicenseinfo.h"
#include "ui_widgetlicenseinfo.h"

#include "viewmanager.h"
#include "../HTTP/urlstrings.h"


#include <QMessageBox>
#include <QWebFrame>
#include <QWebPage>

#include <QApplication>


WidgetLicenseInfo::WidgetLicenseInfo(QWidget *parent) : QWidget(parent), ui(new Ui::WidgetLicenseInfo)
{
    //ui->setupUi(this);
    ui->setupUi(this);
    ui->btnNext->setEnabled(false);

    UrlStrings urlstrings;
    ui->webView->load(urlstrings.URLLicenseInfo);

    isConnection = false;

}

WidgetLicenseInfo::~WidgetLicenseInfo()
{
    delete ui;
}

void WidgetLicenseInfo::on_btnNext_clicked()
{

    ViewManager::GetInstance()->SetViewByIndex(ViewManager::WIDGET_SELECTVERSION);

}

void WidgetLicenseInfo::on_btnPrev_clicked()
{
    ViewManager::GetInstance()->SetViewByIndex(ViewManager::WIDGET_MAIN);

}

void WidgetLicenseInfo::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked())
    {
        if(isConnection) ui->btnNext->setEnabled(true);

    }
    else if(!ui->checkBox->isChecked())
    {
        ui->btnNext->setEnabled(false);
    }
}


void WidgetLicenseInfo::on_webView_loadFinished(bool arg1)
{
    QString html = ui->webView->page()->mainFrame()->toPlainText();


    if(html == "")
    {
        QMessageBox::critical(NULL, "Connection Error", QString("can not connect to Server.."));
        QApplication::exit();
    }
    else
    {
        isConnection = true;

    }
}
