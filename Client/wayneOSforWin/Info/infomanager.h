#ifndef INFOMANAGER_H
#define INFOMANAGER_H

#include <QString>


class InfoManager
{
private:
    InfoManager();

    static InfoManager* mThis;

public:

    const static int PAID = 1;
    const static int FREE = 2;


    const static int USB = 100;
    //const static int SSD = 200;

   


    QString mFileName;

    int mVersionType;
    int mDeviceType;

    int mSize;

    int mDeviceId;
    int mVolumeId;

    QString mDriveInstallPath;



    static InfoManager* GetInstance()
    {
        if(mThis == NULL)
        {
            mThis = new InfoManager();
        }
        return mThis;

    }

    ~InfoManager();


};

#endif // INFOMANAGER_H
