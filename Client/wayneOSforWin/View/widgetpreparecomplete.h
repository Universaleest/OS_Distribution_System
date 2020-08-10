#ifndef WIDGETPREPARECOMPLETE_H
#define WIDGETPREPARECOMPLETE_H

#include <QWidget>

namespace Ui {
class WidgetPrepareComplete;
}

class WidgetPrepareComplete : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetPrepareComplete(QWidget *parent = 0);
    ~WidgetPrepareComplete();

private slots:
    void on_btnPrev_clicked();

    void on_btnSetup_clicked();

private:
    Ui::WidgetPrepareComplete *ui;
};

#endif // WIDGETPREPARECOMPLETE_H
