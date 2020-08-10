#include "widgetcomplete.h"
#include "ui_widgetcomplete.h"

#include "HTTP/httpmanager.h"
#include <QApplication>
#include <QPixmap>
#include <QBitmap>

WidgetComplete::WidgetComplete(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetComplete)
{
	ui->setupUi(this);
	// ~ Added by LEE jeun jeun@wayne-inc.com
	QPixmap pic("company-logo.jpg");
	ui->label->setPixmap(pic);
	ui->label->setMask(pic.mask());
	ui->label->setScaledContents(true);
	// Added by LEE jeun jeun@wayne-inc.com ~
}

WidgetComplete::~WidgetComplete()
{
    delete ui;
}

void WidgetComplete::on_btnNext_clicked()
{
    QApplication::exit();

}

