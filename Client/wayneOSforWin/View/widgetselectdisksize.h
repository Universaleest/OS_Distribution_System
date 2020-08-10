#ifndef WIDGETSELECTDISKSIZE_H
#define WIDGETSELECTDISKSIZE_H

#include <QWidget>


namespace Ui {
class WidgetSelectDiskSize;
}

class WidgetSelectDiskSize : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetSelectDiskSize(QWidget *parent = 0);
    void SetWidgetPrepare();
    ~WidgetSelectDiskSize();

private slots:
    void on_btnPrev_clicked();

    void on_btnNext_clicked();

public slots:
    void DownloadDeviceVolume(QStringList list);

private:
    Ui::WidgetSelectDiskSize *ui;
};

#endif // WIDGETSELECTDISKSIZE_H
