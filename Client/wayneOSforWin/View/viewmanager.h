#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H


#include "stackedwidget.h"
#include <QTimer>

class ViewManager
{
    private:
        static ViewManager* mThis;
        StackedWidget* stackedWidget;
		ViewManager();
    public:
        const static int WIDGET_MAIN = 0;
        const static int WIDGET_LICENSEINFO = 1;
        const static int WIDGET_SELECTVERSION =2;
        const static int WIDGET_SELECTDISK = 3;
        const static int WIDGET_SELECTTYPE =4;
        const static int WIDGET_SELECTDISKSIZE =5;
        const static int WIDGET_PREPARECOMPLETE =6;
        const static int WIDGET_INSTALL = 7;
        const static int WIDGET_COMPLETE = 8;


     
        ~ViewManager();

        void setStackedWidget(StackedWidget* stackedWidget);
        StackedWidget* getStackedWidget();

        void SetViewByIndex(int index);

        static ViewManager* GetInstance()
        {
            if(mThis == NULL)
            {
                mThis = new ViewManager();
            }
            return mThis;
        }
		// ~ Added by LEE Jeun jeun@wayne-inc.com
		QTimer* timer;
		enum FLAG { DOWNLOAD, FORMAT, EXTRACT, INSTALL, EXIT_EVENT };
		FLAG flag;
		// Added by LEE Jeun jeun@wayne-inc.com ~
};




#endif // VIEWMANAGER_H


