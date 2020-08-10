#include "stackedwidget.h"
#include "ui_stackedwidget.h"
#include "View/viewmanager.h"
#include <QSizePolicy>


StackedWidget::StackedWidget(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::StackedWidget)
{
    ui->setupUi(this);
	setScaleToOsSettings(); // Added by LEE Jeun jeun@wayne-inc.com
}

StackedWidget::~StackedWidget()
{
    delete ui;
}

// ~ Added by LEE Jeun jeun@wayne-inc.com
void
StackedWidget::setScaleToOsSettings()
{
#ifdef Q_OS_WIN
	setScale(windowsDpiScale());
#endif
}

void
StackedWidget::setScale(float scale)
{
	this->setFixedSize((int)(701*scale), (int)(503*scale));
	foreach(QVBoxLayout* layout, findChildren<QVBoxLayout*>())
	{
		layout->parentWidget()->setFixedSize(
			(int)(scale*layout->parentWidget()->contentsRect().width()),
			(int)(scale*layout->parentWidget()->contentsRect().height()));
	}
}
// Added by LEE Jeun jeun@wayne-inc.com ~