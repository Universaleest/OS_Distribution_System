#define WAYNE_WINAPI 1

#include "widgetselectdisk.h"
#include "ui_widgetselectdisk.h"
#include "viewmanager.h"

#include "../Info/infomanager.h"

#include <QtGui>
#include <QCoreApplication>
#include <QFileInfo>
#include <cstdio>
#include <cstdlib>
#define NOMINMAX
#include <windows.h>
#include <winioctl.h>
#include <dbt.h>

#include "../Disk/diskmanager.h"
#include "../Disk/disk.h"
//#include "md5.h"


WidgetSelectDisk::WidgetSelectDisk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetSelectDisk)
{
    ui->setupUi(this);
#if WAYNE_WINAPI
	getLogicalDrives();
	if(ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt() == 0)
		ui->btnNext->setEnabled(false);
#endif
}

WidgetSelectDisk::~WidgetSelectDisk()
{
    delete ui;
}

#if WAYNE_WINAPI
void WidgetSelectDisk::getLogicalDrives()
{
	// GetLogicalDrives returns 0 on failure, or a bitmask representing
	// the drives available on the system (bit 0 = A:, bit 1 = B:, etc)
	unsigned long driveMask = GetLogicalDrives();

	int i = 0;
	ULONG pID;

	ui->comboBox->clear();

	while (driveMask != 0)
	{
		if (driveMask & 1)
		{
			// the "A" in drivename will get incremented by the # of bits
			// we've shifted
			char drivename[] = "\\\\.\\A:\\";
			drivename[4] += i;
			if (checkDriveType(drivename, &pID))
			{
				ui->comboBox->addItem(QString("[%1:\\]").arg(drivename[4]), (qulonglong)pID);	
			}
		}
		driveMask >>= 1;
		ui->comboBox->setCurrentIndex(0);
		++i;
	}
	//Lee Debug
	//QMessageBox::information(NULL, "ID Info", QString("volume ID : %1, deviceID : %2").arg(volumeID).arg(deviceID));
}
#endif
void WidgetSelectDisk::on_btnPrev_clicked()
{
    ViewManager::GetInstance()->SetViewByIndex(ViewManager::WIDGET_SELECTVERSION);
}

void WidgetSelectDisk::on_btnNext_clicked()
{
#if WAYNE_WINAPI
    int combo_index = ui->comboBox->currentIndex();
	//selected combo device id
	InfoManager::GetInstance()->mDeviceId =ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt();
	getLogicalDrives();
	
	ui->comboBox->setCurrentIndex(combo_index);
	
	//QMessageBox::information(NULL, "current Index", QString("current index  = %1").arg(ui->comboBox->currentIndex()));
	if(ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt() != InfoManager::GetInstance()->mDeviceId
		|| ui->comboBox->currentIndex()==-1)
	{
		QMessageBox::critical(NULL, "USB or SSD Unconnected", "Connect USB or SSD!!");
		ui->comboBox->setCurrentIndex(0);
		//ui->btnNext->setEnabled(false);
	}
	else
	{
		InfoManager::GetInstance()->mDriveInstallPath = ui->comboBox->currentText();
		//QString tmpStr = ui->comboBox->currentText(); // for debugging
		InfoManager::GetInstance()->mVolumeId = ui->comboBox->currentText().at(1).toLatin1() - 'A';
		//int tmpVolumeID = ui->comboBox->currentText().at(1).toLatin1() - 'A'; // for debugging
		InfoManager::GetInstance()->mDeviceId =ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt();
		//int tmpDeviceID = ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt(); // for debugging
		
		//Get SerialNumber, ProductId, VendorId
		getProductInfo(InfoManager::GetInstance()->mDeviceId);
		getSerialNumber(InfoManager::GetInstance()->mVolumeId);
		
		
		//QMessageBox::information(NULL, "Hello", DiskManager::GetInstance()->mSerialNumber);


		//QMessageBox::information(NULL, "mDeviceId", QString("current mDeviceId  = %1").arg(InfoManager::GetInstance()->mDeviceId));

		/*
		QString serialNumber = DiskManager::GetInstance()->mSerialNumber;
		QString productID = DiskManager::GetInstance()->mProductId;
		QString venderID = DiskManager::GetInstance()->mVendorId;
		QString productRevision = DiskManager::GetInstance()->mProductRevision;
		
		QMessageBox::information(NULL, "Serialnumber", serialNumber);
		QMessageBox::information(NULL, "mProductId", productID);
		QMessageBox::information(NULL, "mVendorId", venderID);
		QMessageBox::information(NULL, "mProductRevision", productRevision);
		*/

		ViewManager::GetInstance()->SetViewByIndex(ViewManager::WIDGET_SELECTTYPE);
		


		
	}
#else
    ViewManager::GetInstance()->SetViewByIndex(ViewManager::WIDGET_SELECTTYPE);
#endif
}

void WidgetSelectDisk::on_btnRefresh_clicked()
{
    #if WAYNE_WINAPI
	InfoManager::GetInstance()->mDeviceId =ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt();

    getLogicalDrives();

    if( ui->comboBox->currentIndex()==-1)
		ui->btnNext->setEnabled(false);
	else 
		ui->btnNext->setEnabled(true);

    #endif
}
