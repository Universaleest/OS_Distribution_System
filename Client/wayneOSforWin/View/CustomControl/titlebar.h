#include <QtGui>

#include <QToolButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QStyle>
#include <QCoreApplication>
#include "View/viewmanager.h"
#include <QMessageBox>


class TitleBar : public QWidget
{
    Q_OBJECT
public:
    TitleBar(QWidget *parent);
    ~TitleBar();
public slots:
    void showSmall();
    void showMaxRestore();
	// exit when clicking exit button, Added by LEE Jeun@wayne-inc.com
	void queryExit();

protected:
    void mousePressEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);

private:
    QLabel* imageLabel;

    QToolButton *minimize;
    QToolButton *close;
    QPixmap restorePix, maxPix;
    bool maxNormal;
    QPoint startPos;
    QPoint clickPos;
};


