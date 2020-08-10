#ifndef REQOSFILE_H
#define REQOSFILE_H


#include <QtNetwork/QNetworkAccessManager>

class ReqOsFile
{
public:
	ReqOsFile(QNetworkAccessManager* manager, QString updateFileName, int slideIndex, int slideCount);
    ~ReqOsFile();
    QJsonDocument CreateData();
    int slideIndex;
    int slideCount;
	QString updateFileName;
};

#endif // REQOSFILE_H
