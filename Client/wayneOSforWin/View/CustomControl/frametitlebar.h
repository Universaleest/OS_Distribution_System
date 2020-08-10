#ifndef FRAMETITLEBAR_H
#define FRAMETITLEBAR_H

#include "titlebar.h"
#include <QFrame>
#include "../stackedwidget.h"

class FrameTitleBar : public QFrame
{
public:
    FrameTitleBar();
    ~FrameTitleBar();


    QWidget *contentWidget() const { return m_content; }

    TitleBar *titleBar() const { return m_titleBar; }

    void mousePressEvent(QMouseEvent *e);

    void mouseMoveEvent(QMouseEvent *e);


    void mouseReleaseEvent(QMouseEvent *e);

private:

    TitleBar *m_titleBar;
    QWidget *m_content;
    QPoint m_old_pos;
    bool m_mouse_down;
    bool left, right, bottom;
    StackedWidget* stackedWidget;

};

#endif // FRAMETITLEBAR_H
