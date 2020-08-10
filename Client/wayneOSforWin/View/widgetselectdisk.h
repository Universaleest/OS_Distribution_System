#ifndef WIDGETSELECTDISK_H
#define WIDGETSELECTDISK_H

#include <QWidget>
#ifndef WINVER
#define WINVER 0x0500
#endif
#include <QtGui>
#include <cstdio>
#include <cstdlib>
#define NOMINMAX
#include <windows.h>
#include <winioctl.h>
#include <QtWidgets\qmainwindow.h>

#include <QtWidgets\qfiledialog.h>

#include "../Disk/disk.h"
#include "ui_widgetselectdisk.h"

namespace Ui {
class WidgetSelectDisk;
}

class WidgetSelectDisk : public QWidget//, public Ui::WidgetSelectDisk
{
    Q_OBJECT

public:
    explicit WidgetSelectDisk(QWidget *parent = 0);
    ~WidgetSelectDisk();

private slots:
    void on_btnPrev_clicked();

    void on_btnNext_clicked();

	void on_btnRefresh_clicked();
private:

    Ui::WidgetSelectDisk *ui;

	void getLogicalDrives();
};

#endif // WIDGETSELECTDISK_H
