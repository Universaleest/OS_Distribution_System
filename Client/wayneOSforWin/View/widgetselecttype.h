#ifndef WIDGETSELECTTYPE_H
#define WIDGETSELECTTYPE_H

#include <QWidget>

namespace Ui {
class WidgetSelectType;
}

class WidgetSelectType : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetSelectType(QWidget *parent = 0);
    ~WidgetSelectType();

private slots:
    void on_btnPrev_clicked();

    void on_btnNext_clicked();

private:
    Ui::WidgetSelectType *ui;
};

#endif // WIDGETSELECTTYPE_H
