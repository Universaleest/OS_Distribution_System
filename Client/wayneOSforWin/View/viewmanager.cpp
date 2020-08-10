#include "viewmanager.h"
#include <QtWidgets>
#include <QtGui>

ViewManager* ViewManager::mThis;


ViewManager::ViewManager()
{
	// ~ Added by LEE Jeun@wayne-inc.com
	timer = new QTimer();
	flag = DOWNLOAD;
	// Added by LEE Jeun@wayne-inc.com
}

ViewManager::~ViewManager()
{
	if (timer != NULL) delete timer; // Added by LEE Jeun@wayne-inc.com
}

void ViewManager::SetViewByIndex(int index)
{
    if(index == WIDGET_MAIN)
    {
        stackedWidget->setCurrentIndex(WIDGET_MAIN);
    }
    else if(index == WIDGET_LICENSEINFO)
    {
        stackedWidget->setCurrentIndex(WIDGET_LICENSEINFO);
    }
    else if(index == WIDGET_SELECTVERSION)
    {
        stackedWidget->setCurrentIndex(WIDGET_SELECTVERSION);
    }

    else if(index == WIDGET_SELECTDISK)
    {
        stackedWidget->setCurrentIndex(WIDGET_SELECTDISK);
    }
    else if(index == WIDGET_SELECTTYPE)
    {
        stackedWidget->setCurrentIndex(WIDGET_SELECTTYPE);
    }
    else if(index == WIDGET_SELECTDISKSIZE)
    {
        stackedWidget->setCurrentIndex(WIDGET_SELECTDISKSIZE);
    }
    else if(index == WIDGET_PREPARECOMPLETE)
    {
        stackedWidget->setCurrentIndex(WIDGET_PREPARECOMPLETE);
    }
    else if(index == WIDGET_INSTALL)
    {
        stackedWidget->setCurrentIndex(WIDGET_INSTALL);
    }
    else if(index == WIDGET_COMPLETE)
    {
        stackedWidget->setCurrentIndex(WIDGET_COMPLETE);
    }
}

void ViewManager::setStackedWidget(StackedWidget* stackedWidget)
{
    this->stackedWidget = stackedWidget;
}

StackedWidget* ViewManager::getStackedWidget()
{
    return this->stackedWidget;
}
