#include "frametitlebar.h"

#include "../../HTTP/httpmanager.h"
#include "../../View/viewmanager.h"
#include "../../Info/infomanager.h"
#include "../../Disk/diskmanager.h"
#include "WinDpiScale/WinDpiScale.h"

#include <QMessageBox>


FrameTitleBar::FrameTitleBar()
{
    m_mouse_down = false;
    setFrameShape(Panel);

    // Make this a borderless window which can't
    // be resized or moved via the window system
    setWindowFlags(Qt::FramelessWindowHint);
    setMouseTracking(true);

    m_titleBar = new TitleBar(this);

	QIcon icon("icon.ico");
	m_titleBar->setWindowIcon(icon);


    m_content = new QWidget(this);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(m_titleBar);
    vbox->setMargin(0);
    vbox->setSpacing(0);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_content);
    layout->setMargin(5);
    layout->setSpacing(0);
    vbox->addLayout(layout);

	// ~ Modified by LEE Jeun jeun@wayne-inc.com
    //size Fixed..
	//this->setFixedWidth(705);
	//this->setFixedHeight(540);
	//this->setFixedSize(705, 540);
	// Modified by LEE Jeun jeun@wayne-inc.com ~
}

FrameTitleBar::~FrameTitleBar()
{
	//excetion update file remove.
	if (QFile::exists("updStatus"))
	{
		if (HttpManager::GetInstance()->httpThread.updateFile != NULL && HttpManager::GetInstance()->httpThread.updateFile->isOpen())
		{
			HttpManager::GetInstance()->httpThread.updateFile->close();
		}
		QDir dir("updStatus");
		dir.removeRecursively(); 
	}
    qDebug()<<"MainWindow Delete Managers...";

	CoUninitialize();
    delete ViewManager::GetInstance()->getStackedWidget();
    delete ViewManager::GetInstance();
    delete InfoManager::GetInstance();
	delete DiskManager::GetInstance();
    delete HttpManager::GetInstance();
}



void FrameTitleBar::mousePressEvent(QMouseEvent *e)
{
    m_old_pos = e->pos();
    m_mouse_down = e->button() == Qt::LeftButton;
}

void FrameTitleBar::mouseMoveEvent(QMouseEvent *e)
{
    int x = e->x();
    int y = e->y();

    if (m_mouse_down) {
        int dx = x - m_old_pos.x();
        int dy = y - m_old_pos.y();

        QRect g = geometry();

        if (left)
            g.setLeft(g.left() + dx);
        if (right)
            g.setRight(g.right() + dx);
        if (bottom)
            g.setBottom(g.bottom() + dy);

        setGeometry(g);

        m_old_pos = QPoint(!left ? e->x() : m_old_pos.x(), e->y());
    } else {
        QRect r = rect();
        left = qAbs(x - r.left()) <= 5;
        right = qAbs(x - r.right()) <= 5;
        bottom = qAbs(y - r.bottom()) <= 5;
        bool hor = left | right;

        if (hor && bottom) {
       
            setCursor(Qt::ArrowCursor);
        }
    }
}


void FrameTitleBar::mouseReleaseEvent(QMouseEvent *e)
{
    m_mouse_down = false;
}
