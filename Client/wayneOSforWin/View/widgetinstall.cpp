#define WAYNE_WINAPI 1

#include "widgetinstall.h"
#include "ui_widgetinstall.h"

#include "viewmanager.h"

#include "../Info/infomanager.h"
#include "../View/viewmanager.h"

#include <QMessageBox>
#include <QDir>
#include <QDirIterator>
#include <QStringList>
#include "formatusb.h"


WidgetInstall::WidgetInstall(QWidget *parent) : QWidget(parent),ui(new Ui::WidgetInstall)
{
    ui->setupUi(this);
	// ~ Added by LEE Jeun@wayne-inc.com
	idx = 0, cnt = 0, p = 0;
	//display = "Downloading Files";
	connect(ViewManager::GetInstance()->timer, SIGNAL(timeout()), this, SLOT(setDynamic()));
	//ViewManager::GetInstance()->timer->start(500);
	// Added by LEE Jeun@wayne-inc.com
	status = STATUS_IDLE;
	ui->progressBar->reset();
	hVolume = INVALID_HANDLE_VALUE;
	hFile = INVALID_HANDLE_VALUE;
	hRawDisk = INVALID_HANDLE_VALUE;
	sectorData = NULL;
	sectorsize = 0ul;
}

WidgetInstall::~WidgetInstall()
{
    delete ui;
}
Ui::WidgetInstall* WidgetInstall::getUI()
{
	return ui;
}
void WidgetInstall::on_btnNext_clicked()
{
	ViewManager::GetInstance()->SetViewByIndex(ViewManager::WIDGET_COMPLETE);

}

// Merging extract function into installation function. 
// extract and read a partial file, write it on a hard disk, read this partial file and write it on USB, and delete a partial file from a hard disk. this is one cycle of this function.
// ~ Modified by LEE Jeun jeun@wayne-inc.com
void WidgetInstall::startInstall()
{
	DWORD size;
	BOOL bResult = FALSE;
	char driveName[4];
	char volumeName[MAX_PATH];
	status = STATUS_WRITING;
	// ~ Added by LEE Jeun@wayne-inc.com
	ViewManager::GetInstance()->timer->stop();
	ViewManager::GetInstance()->flag = ViewManager::GetInstance()->INSTALL;
	ViewManager::GetInstance()->timer->start(500);
	display = "Installing";
	ui->labelStatus->setText(display);

    #if WAYNE_WINAPI
	double mbpersec;
	unsigned long long i, lasti, availablesectors, numsectors;

	//int volumeID = InfoManager::GetInstance()->mVolumeId;
	int deviceID = InfoManager::GetInstance()->mDeviceId; 

	hRawDisk = getHandleOnDevice(deviceID, GENERIC_WRITE | GENERIC_READ);

	if (hRawDisk == INVALID_HANDLE_VALUE)
	{
		qDebug("Could not get handle from device!");
		status = STATUS_EXIT;
		return;
	}

	bResult = DeviceIoControl(hRawDisk, IOCTL_DISK_UPDATE_PROPERTIES, NULL, 0, NULL, 0, &size, NULL);
		
	volume_name = getLogicalName(deviceID, 0, FALSE);

	if (volume_name.empty())
	{
		volume_name = AltGetLogicalName(deviceID, 0, FALSE);
		qDebug("Use dos volume name: %s", volume_name.c_str());
	}

	else
	{
		qDebug("Use volume GUID: %s", volume_name.c_str());
	}

	if (volume_name.empty())
	{
		qDebug("Could not find volume name!");
		CloseHandle(hRawDisk);
		hRawDisk = INVALID_HANDLE_VALUE;
		status = STATUS_EXIT;
		return;
	}

	memset(volumeName, 0, sizeof(volumeName));
	strncpy(volumeName, volume_name.c_str(), volume_name.size());
	qDebug("volume name: %s", volumeName);
	
	hVolume = CreateFileA(volumeName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, 0, NULL);

	if (hVolume == INVALID_HANDLE_VALUE)
	{
		//status = STATUS_IDLE;
		status = STATUS_EXIT;
		//btnCancel->setEnabled(false);
		//setReadWriteButtonState();
		CloseHandle(hRawDisk);
		hRawDisk = INVALID_HANDLE_VALUE;
		return;
	}
	if (!getLockOnVolume(hVolume))
	{
		CloseHandle(hRawDisk);
		CloseHandle(hVolume);
		//status = STATUS_IDLE;
		status = STATUS_EXIT;
		hRawDisk = INVALID_HANDLE_VALUE;
		hVolume = INVALID_HANDLE_VALUE;
		//btnCancel->setEnabled(false);
		//setReadWriteButtonState();
		return;
	}
	if (!unmountVolume(hVolume))
	{
		removeLockOnVolume(hVolume);
		CloseHandle(hRawDisk);
		CloseHandle(hVolume);
		//status = STATUS_IDLE;
		status = STATUS_EXIT;
		hRawDisk = INVALID_HANDLE_VALUE;
		hVolume = INVALID_HANDLE_VALUE;
		//btnCancel->setEnabled(false);
		//setReadWriteButtonState();
		return;
	}
	
	i = 0ul, lasti = 0ul;
	availablesectors = getNumberOfSectors(hRawDisk, &sectorsize);
	ui->progressBar->reset();

	unsigned long long totalSz = 0;

	QDir dir;
	QString path = dir.absoluteFilePath("updStatus/" + HttpManager::GetInstance()->httpThread.getFileName());
	QByteArray charPath = path.toLocal8Bit();

	unzFile uf = unzOpen(charPath.toStdString().c_str());

	if (uf == NULL)
	{
		qDebug() << "failed to open .zip file!";

		removeLockOnVolume(hVolume);
		CloseHandle(hRawDisk);
		CloseHandle(hVolume);
		hRawDisk = INVALID_HANDLE_VALUE;
		hVolume = INVALID_HANDLE_VALUE;

		status = STATUS_EXIT;

		return;
	}

	if (unzGoToFirstFile(uf) != UNZ_OK)
	{
		qDebug() << "error occured!";

		removeLockOnVolume(hVolume);
		CloseHandle(hRawDisk);
		CloseHandle(hVolume);
		unzClose(uf);
		hRawDisk = INVALID_HANDLE_VALUE;
		hVolume = INVALID_HANDLE_VALUE;

		status = STATUS_EXIT;

		return;
	}

	unsigned long long curTot = 0;
	
	unz_file_info64 info;
	unzGetCurrentFileInfo64(uf, &info, NULL, 0, NULL, 0, NULL, 0);

	totalSz = info.uncompressed_size;

	qDebug() << "OS file name : " << HttpManager::GetInstance()->httpThread.getFileName();
	qDebug() << "compressed size : " << info.compressed_size << " bytes";
	qDebug() << "uncompressed size : " << info.uncompressed_size << " bytes";

	unsigned long long totalNumsectors = (unsigned long long)info.uncompressed_size / sectorsize;

	if (totalNumsectors > availablesectors)
	{
		QMessageBox::critical(NULL, "Write Error", QString::fromLocal8Bit("Not enough space on disk: Size: %1 sectors  Available: %2 sectors  Sector size: %3").arg(totalNumsectors).arg(availablesectors).arg(sectorsize));
		status = STATUS_EXIT;
		removeLockOnVolume(hVolume);
		CloseHandle(hRawDisk);
		CloseHandle(hVolume);
		unzClose(uf);
		hRawDisk = INVALID_HANDLE_VALUE;
		hVolume = INVALID_HANDLE_VALUE;
		return;
	}

	ui->progressBar->setRange(0, totalNumsectors);
	
	timer.start();

	unsigned long long partNumber = 0;

	if (unzOpenCurrentFile(uf) != UNZ_OK)
	{
		qDebug() << "error occured!";

		unzClose(uf);
		removeLockOnVolume(hVolume);
		CloseHandle(hRawDisk);
		CloseHandle(hVolume);
		hRawDisk = INVALID_HANDLE_VALUE;
		hVolume = INVALID_HANDLE_VALUE;

		status = STATUS_EXIT;
		
		return;
	}

	const int BUFSIZE = 524288;
	Bytef in[BUFSIZE];
	unsigned long long readSz = 0;

	while (readSz = unzReadCurrentFile(uf, in, BUFSIZE))
	{
		QCoreApplication::processEvents();

		if (ViewManager::GetInstance()->flag == ViewManager::GetInstance()->EXIT_EVENT)
		{
			qDebug() << "detect clicking exit button!";

			break;
		}

		if (readSz < 0)
		{
			qDebug() << "error occured!";

			unzCloseCurrentFile(uf);
			unzClose(uf);
			removeLockOnVolume(hVolume);
			CloseHandle(hRawDisk);
			CloseHandle(hVolume);
			hRawDisk = INVALID_HANDLE_VALUE;
			hVolume = INVALID_HANDLE_VALUE;

			status = STATUS_EXIT;

			return;
		}

		curTot += readSz;

		partNumber = (curTot * 100 / totalSz);

		qDebug() << curTot << "/" << totalSz << " completed...";

		QString uzFilename = "WayneOSfile_part";

		QFile uzf("updStatus/" + uzFilename + QString::number(partNumber));
		uzf.open(QIODevice::WriteOnly);
		
		uzf.write((const char*)in, readSz);

		uzf.close();

		path = dir.absoluteFilePath(QString("updStatus/"));

		QDirIterator it(path, QDirIterator::Subdirectories);

		while (it.hasNext())
		{
			qDebug() << it.next();
			if (it.fileName().toStdString().find("zip") == std::string::npos && it.fileName().toStdString().find(uzFilename.toStdString()) != std::string::npos)
			{
				qDebug() << "OS File : " << it.fileName();
				path = dir.absoluteFilePath(it.filePath());
				break;
			}
		}

		charPath = path.toLocal8Bit();

		hFile = getHandleOnFile(charPath.data(), GENERIC_READ);
		//hFile = getHandleOnFile("wayneUpdateFile", GENERIC_READ);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			unzCloseCurrentFile(uf);
			unzClose(uf);
			removeLockOnVolume(hVolume);
			CloseHandle(hRawDisk);
			CloseHandle(hVolume);
			//status = STATUS_IDLE;
			status = STATUS_EXIT;
			hRawDisk = INVALID_HANDLE_VALUE;
			hVolume = INVALID_HANDLE_VALUE;
			//btnCancel->setEnabled(false);
			//setReadWriteButtonState();
			return;
		}

		numsectors = getFileSizeInSectors(hFile, sectorsize);

		sectorData = readSectorDataFromHandle(hFile, 0, (totalNumsectors - i >= numsectors) ? numsectors : (totalNumsectors - i), sectorsize);
		
		if (sectorData == NULL)
		{
			delete sectorData;
			unzCloseCurrentFile(uf);
			unzClose(uf);
			removeLockOnVolume(hVolume);
			CloseHandle(hRawDisk);
			CloseHandle(hFile);
			CloseHandle(hVolume);
			//status = STATUS_IDLE;
			status = STATUS_EXIT;
			sectorData = NULL;
			hRawDisk = INVALID_HANDLE_VALUE;
			hFile = INVALID_HANDLE_VALUE;
			hVolume = INVALID_HANDLE_VALUE;
			//btnCancel->setEnabled(false);
			//setReadWriteButtonState();
			return;
		}
		
		if (!writeSectorDataToHandle(hRawDisk, sectorData, i, (totalNumsectors - i >= numsectors) ? numsectors : (totalNumsectors - i), sectorsize))
		{
			delete sectorData;
			unzCloseCurrentFile(uf);
			unzClose(uf);
			removeLockOnVolume(hVolume);
			CloseHandle(hRawDisk);
			CloseHandle(hFile);
			CloseHandle(hVolume);
			//status = STATUS_IDLE;
			status = STATUS_EXIT;
			sectorData = NULL;
			hRawDisk = INVALID_HANDLE_VALUE;
			hFile = INVALID_HANDLE_VALUE;
			hVolume = INVALID_HANDLE_VALUE;
			//btnCancel->setEnabled(false);
			//setReadWriteButtonState();
			return;
		}

		delete sectorData;
		sectorData = NULL;
		
		if (timer.elapsed() >= 1000)
		{
			mbpersec = (((double)sectorsize * (i - lasti)) * (1000.0 / timer.elapsed())) / 1024.0 / 1024.0;
			//after make
			//statusbar->showMessage(QString("%1MB/s").arg(mbpersec));
			timer.start();
			lasti = i;
		}
		ui->progressBar->setValue(i);
		CloseHandle(hFile);
		sectorData = NULL;
		hFile = INVALID_HANDLE_VALUE;
		uzf.remove();
		i += numsectors;
	}

	unzCloseCurrentFile(uf);
	unzClose(uf);
	//ui->progressBar->reset();
    #endif
	
	qDebug("Refresh drive layout");
	bResult = DeviceIoControl(hRawDisk, IOCTL_DISK_UPDATE_PROPERTIES, NULL, 0, NULL, 0, &size, NULL);

	removeLockOnVolume(hVolume);
	CloseHandle(hVolume);
	CloseHandle(hRawDisk);
	hVolume = INVALID_HANDLE_VALUE;
	hRawDisk = INVALID_HANDLE_VALUE;
	Sleep(200);
	//WaitForLogical(deviceID, 0);

	if (ViewManager::GetInstance()->flag == ViewManager::GetInstance()->EXIT_EVENT)
	{
		this->CompleteUpdateFileDelete();
		QCoreApplication::exit();
		return;
	}

	while (!WaitForLogical(deviceID, 0))
	{
		if (ViewManager::GetInstance()->flag == ViewManager::GetInstance()->EXIT_EVENT)
		{
			this->CompleteUpdateFileDelete();
			QCoreApplication::exit();
			return;
		}

		QMessageBox msgBox;
		msgBox.setText("Plug out USB from your PC and plug in USB again.\nClick Ok after you do this.");
		msgBox.setWindowTitle("Notice");
		msgBox.setIcon(QMessageBox::Information);
		int ret = msgBox.exec();
		
		if (ret == QMessageBox::Ok && !getLogicalName(deviceID, 0, TRUE).empty())
		{
			bResult = scanForHwChanges();
			break;
		}
	}
	
	volume_name = getLogicalName(deviceID, 0, TRUE);
	memset(volumeName, 0, sizeof(volumeName));
	strncpy(volumeName, volume_name.c_str(), volume_name.size());
	memset(driveName, 0, sizeof(driveName));
	strncpy(driveName, drive_name.c_str(), drive_name.size());
	qDebug("volume name: %s, drive name: %s", volumeName, driveName);
	bResult = MountVolume(driveName, volumeName);

	if (bResult)
	{
		qDebug("volume mounted as %s", driveName);
	}

	ViewManager::GetInstance()->timer->stop();
	ui->progressBar->hide();
	ui->label_3->hide();
	ui->label_4->hide();
	ui->label_5->hide();
	ui->labelStatus->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	ui->labelStatus->setFont(QFont("Aharoni", 13, QFont::Bold, false));
	ui->labelStatus->setText(QString::fromLocal8Bit("Installation Complete!!"));
	this->CompleteUpdateFileDelete();
	ui->btnNext->setEnabled(true);
	
	status = STATUS_IDLE;
}
// Modified by LEE Jeun jeun@wayne-inc.com ~

void WidgetInstall::RequestServerData()
{
	ui->btnNext->setEnabled(false);
	// ~ Added by LEE Jeun jeun@wayne-inc.com
	p = 0;
	ViewManager::GetInstance()->timer->start(500);
	// Added by LEE JEun jeun@wayne-inc.com ~
	QObject::connect(&(HttpManager::GetInstance()->httpThread), SIGNAL(DownloadStatus(int, int)), this, SLOT(DownloadStatus(int, int)));
	HttpManager::GetInstance()->RequestOSData();
}

// ~ Modified by LEE Jeun@wayne-inc.com
//from Http Thread
void WidgetInstall::DownloadStatus(int index, int count)
{
	if (index < count)
	{
		idx = index;
		cnt = count;
		//QString status = "Downloading Files... " + QString::number(index) + " / " + QString::number(count);
		display = "Downloading Files " + QString::number(idx) + " / " + QString::number(cnt);
		switch (p)
		{
		case 1:
			display += " . ";
			break;
		case 2:
			display += " . . ";
			break;
		case 3:
			display += " . . . ";
			break;
		}
		ui->progressBar->setValue(index * 100 / count);
		//ui->labelStatus->setText(status);
		ui->labelStatus->setText(display);
	}
	else if (index == count)
	{
		qDebug() << "FileDownload End";

		int deviceId = InfoManager::GetInstance()->mDeviceId;

		display = "Clean USB...";
		ui->labelStatus->setText(display);
		ViewManager::GetInstance()->flag = ViewManager::GetInstance()->FORMAT;
		ui->progressBar->reset();

		if (getVds(deviceId, drive_name) == -1)
		{
			QMessageBox::critical(NULL, "error", "Could not clean USB!");
			status = STATUS_EXIT;
		}

		else
		{
			try
			{
				this->startInstall();
			}
			catch (std::exception & e)
			{
				qDebug() << "install Fail!";
				this->CompleteUpdateFileDelete();
				status = STATUS_EXIT;
			}
		}
	}

	if (status == STATUS_EXIT)
	{
		QMessageBox::critical(NULL, "error", "error occured while installing!");
		QCoreApplication::exit(-1);
	}
}
// Modified by LEE Jeun jeun@wayne-inc.com ~

void WidgetInstall::CompleteUpdateFileDelete()
{
	if (QFile::exists("updStatus"))
	{
		qDebug() << "file exists..and delete";
		QDir dir("updStatus");
		dir.removeRecursively();

	}
}

// ~ Added by LEE Jeun jeun@wayne-inc.com
void
WidgetInstall::setDynamic()
{
	if (p < 3)
	{
		display += " . ";
		++p;
		ui->labelStatus->setText(display);
	}

	else
	{
		if (ViewManager::GetInstance()->flag == ViewManager::GetInstance()->DOWNLOAD)
		{
			display = "Downloading Files " 
				+ QString::number(idx) + " / " + QString::number(cnt);
		}

		else if (ViewManager::GetInstance()->flag == ViewManager::GetInstance()->INSTALL)
		{
			display = "Installing";
		}

		else if(ViewManager::GetInstance()->flag == ViewManager::GetInstance()->EXTRACT)
		{
			display = "Extracting";
		}

		p = 0;
		ui->labelStatus->setText(display);
	}
}

/*int WidgetInstall::extract(const QString& filename) // this is for extracting .zip files.
{
	ViewManager::GetInstance()->flag = ViewManager::GetInstance()->EXTRACT;

	p = 0;

	ui->progressBar->reset();
	ui->progressBar->setRange(0, 100);

	unsigned long long totalSz = 0;

	QDir dir;
	QString path = dir.absoluteFilePath("updStatus/" + filename);
	QByteArray charpath = path.toLocal8Bit();

	unzFile uf = unzOpen(charpath.toStdString().c_str());

	if (uf == NULL)
	{
		qDebug() << "failed to open .zip file!";
		unzClose(uf);

		return -1;
	}

	if (unzGoToFirstFile(uf) != UNZ_OK)
	{
		qDebug() << "error occured!";
		unzClose(uf);

		return -1;
	}

	const int MAX_FILENAME = 256;
	//const int MAX_COMMENT = 256;

	unsigned long long curTot = 0;

	char oriFileName[MAX_FILENAME];
	//char comment[MAX_COMMENT];

	unz_file_info64 info = { 0, };
	unzGetCurrentFileInfo64(uf, &info, oriFileName, MAX_FILENAME, NULL, 0, NULL, 0);

	totalSz = info.uncompressed_size;

	qDebug() << "filename : " << oriFileName;
	//qDebug() << "comment : " << comment;
	qDebug() << "compressed size : " << info.compressed_size << " bytes";
	qDebug() << "uncompressed size : " << info.uncompressed_size << " bytes";

	QString uzFilename = oriFileName;

	QFile uzf("updStatus/" + uzFilename);
	uzf.open(QIODevice::WriteOnly);

	if (unzOpenCurrentFile(uf) != UNZ_OK)
	{
		qDebug() << "error occured!";

		uzf.close();
		unzClose(uf);

		return -1;
	}

	const int BUFSIZE = 204800;
	Bytef in[BUFSIZE];
	unsigned long long readSz = 0;

	while (readSz = unzReadCurrentFile(uf, in, BUFSIZE))
	{
		QCoreApplication::processEvents();

		if (ViewManager::GetInstance()->flag == ViewManager::GetInstance()->EXIT_EVENT)
		{
			qDebug() << "detect clicking exit button!";

			unzCloseCurrentFile(uf);
			uzf.close();
			unzClose(uf);

			return 1;
		}

		if (readSz < 0)
		{
			qDebug() << "error occured!";

			unzCloseCurrentFile(uf);
			uzf.close();
			unzClose(uf);

			return -1;
		}

		uzf.write((const char*)in, readSz);

		curTot += readSz;

		qDebug() << curTot << "/" << totalSz << " completed...";

		ui->progressBar->setValue((100 * curTot) / totalSz);
	}

	qDebug() << "decompression completed : " << filename << " - > " << uzFilename;

	unzCloseCurrentFile(uf);
	uzf.close();
	unzClose(uf);

	return 0;
}*/
// Added by LEE Jeun jeun@wayne-inc.com ~
