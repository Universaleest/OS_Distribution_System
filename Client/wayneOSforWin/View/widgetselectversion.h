#ifndef WIDGETSELECTVERSION_H
#define WIDGETSELECTVERSION_H

#include <QWidget>

namespace Ui {
class WidgetSelectVersion;
}

class WidgetSelectVersion : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetSelectVersion(QWidget *parent = 0);
    ~WidgetSelectVersion();

private slots:
    void on_btnNext_clicked();

    void on_btnPrev_clicked();

private:
    Ui::WidgetSelectVersion *ui;
};

#endif // WIDGETSELECTVERSION_H
