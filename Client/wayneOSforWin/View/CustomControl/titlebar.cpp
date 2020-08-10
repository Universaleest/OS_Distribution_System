#include "titlebar.h"

TitleBar::TitleBar(QWidget *parent)
{
    // Don't let this widget inherit the parent's backround color
    setAutoFillBackground(true);
    // Use a brush with a Highlight color role to render the background
    setBackgroundRole(QPalette::Highlight);

    imageLabel = new QLabel(this);
    minimize = new QToolButton(this);
    close= new QToolButton(this);

    // Use the style to set the button pixmaps
    QPixmap pix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    close->setIcon(pix);


    pix = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    minimize->setIcon(pix);

    restorePix = style()->standardPixmap(QStyle::SP_TitleBarNormalButton);

    minimize->setMinimumHeight(20);
    close->setMinimumHeight(20);

    //imageLabel = new QLabel(this);

	// ~ Modified by LEE jeun jeun@wayne-inc.com
    QPixmap pixmap("icon.png");
	imageLabel->setPixmap(pixmap);
	// Modified by LEE jeun jeun@wayne-inc.com ~

    imageLabel->setMask(pixmap.mask());
    imageLabel->setScaledContents(true);

    QLabel* label = new QLabel(this);
	// ~ Modified by LEE jeun jeun@wayne-inc.com
    label->setText("  Wayne OS Installer 1.4.2");
    parent->setWindowTitle("  Wayne OS Installer 1.4.2");
	// Modified by LEE jeun jeun@wayne-inc.com ~

    QHBoxLayout* hbox = new QHBoxLayout(this);
    hbox->addWidget(imageLabel);
    hbox->addWidget(label);
    hbox->addWidget(minimize);
    hbox->addWidget(close);

    hbox->insertStretch(2, 500);
    hbox->setSpacing(0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    maxNormal = false;

    //connect(close, SIGNAL( clicked() ), parent, SLOT(close() ) );
	// Added by LEE Jeun@wayne-inc.com
	connect(close, SIGNAL(clicked()), this, SLOT(queryExit()));
    connect(minimize, SIGNAL( clicked() ), this, SLOT(showSmall() ) );

}
TitleBar::~TitleBar()
{

    delete minimize;
    delete close;
    delete imageLabel;
}

void TitleBar::showSmall()
{
    parentWidget()->showMinimized();
}

void TitleBar::showMaxRestore()
{
    if (maxNormal) {
        parentWidget()->showNormal();
        maxNormal = !maxNormal;

    } else {
        parentWidget()->showMaximized();
        maxNormal = !maxNormal;

    }
}

// exit when clicking exit button, Added by LEE Jeun@wayne-inc.com
void TitleBar::queryExit()
{
	QMessageBox msgBox;
	if (msgBox.warning(parentWidget(), "Wayne OS Installer", "The installer will be closed.\nDo you want to stop Wayne OS installation?",
		QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
	{
		ViewManager::GetInstance()->flag = ViewManager::GetInstance()->EXIT_EVENT;

		parentWidget()->close();
	}
	/*msgBox.setText("The installer will be closed.\nDo you want to stop Wayne OS installation?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	msgBox.setWindowTitle("Wayne Inc. OS Installer");
	msgBox.setIcon(QMessageBox::Warning);
	msgBox.setStyleSheet("QLabel{width:200px; font-size: 13px;} QPushButton{ width:10px; height:10px; font-size: 11px; }");

	int ret = msgBox.exec();
	
	if (ret == QMessageBox::Yes)
	{
		ViewManager::GetInstance()->flag = ViewManager::GetInstance()->EXIT_EVENT;
		
		parentWidget()->close();
	}*/
}

void TitleBar::mousePressEvent(QMouseEvent *me)
{
    startPos = me->globalPos();
    clickPos = mapToParent(me->pos());
}
void TitleBar::mouseMoveEvent(QMouseEvent *me)
{
    if (maxNormal)
        return;
    parentWidget()->move(me->globalPos() - clickPos);
}
