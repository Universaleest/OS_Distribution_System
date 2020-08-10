
#ifndef WIDGETLICENSEINFO_H
#define WIDGETLICENSEINFO_H

#include <QWidget>
#include "ui_widgetlicenseinfo.h"


namespace Ui {
class WidgetLicenseInfo;
}

class WidgetLicenseInfo : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetLicenseInfo(QWidget *parent = 0);
    ~WidgetLicenseInfo();

private slots:
    void on_btnNext_clicked();

    void on_btnPrev_clicked();

	void on_checkBox_clicked();
    void on_webView_loadFinished(bool arg1);

private:
    Ui::WidgetLicenseInfo *ui;
    bool isConnection;
};

#endif // WIDGETLICENSEINFO_H
