#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

#include <QStackedWidget>
#include "WinDpiScale/WinDpiScale.h"

namespace Ui {
class StackedWidget;
}

class StackedWidget : public QStackedWidget
{
    Q_OBJECT

public:
    explicit StackedWidget(QWidget *parent = 0);
    ~StackedWidget();
	// ~ Added by LEE Jeun jeun@wayne-inc.com
	void setScaleToOsSettings();
	void setScale(float scale);
	// Added by LEE Jeun jeun@wayne-inc.com ~

private:
    Ui::StackedWidget *ui;
};

#endif // STACKEDWIDGET_H
