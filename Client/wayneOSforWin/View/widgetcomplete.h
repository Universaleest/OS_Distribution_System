#ifndef WIDGETCOMPLETE_H
#define WIDGETCOMPLETE_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class WidgetComplete;
}

class WidgetComplete : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetComplete(QWidget *parent = 0);
    ~WidgetComplete();

private slots:
    void on_btnNext_clicked();

private:
    Ui::WidgetComplete *ui;
};

#endif // WIDGETCOMPLETE_H
