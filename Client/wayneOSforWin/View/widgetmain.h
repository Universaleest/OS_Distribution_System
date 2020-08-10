#ifndef WIDGETMAIN_H
#define WIDGETMAIN_H

#include <QWidget>

namespace Ui {
class WidgetMain;
}

class WidgetMain : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMain(QWidget *parent = 0);
    ~WidgetMain();

private slots:
    void on_btnStart_clicked();

    void on_webView_loadFinished(bool arg1);

private:
    Ui::WidgetMain *ui;
};

#endif // WIDGETMAIN_H
