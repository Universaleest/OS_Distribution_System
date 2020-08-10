#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include <QString>


class DiskManager
{
private:
    DiskManager();

    static DiskManager* mThis;

public:

    QString mSerialNumber;// = 0; 
    QString mProductId;// = 0; 
	QString mVendorId; 
	QString mProductRevision; 

    static DiskManager* GetInstance()
    {
        if(mThis == NULL)
        {
            mThis = new DiskManager();
        }
        return mThis;

    }

    ~DiskManager();

};

#endif // DISKMANAGER_H
