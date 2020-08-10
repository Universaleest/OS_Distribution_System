// this is added by LEE jeun jeun@wayne-inc.com
#ifndef WINDPISCALE_H
#define WINDPISCALE_H

#include <QtGlobal>

#ifdef Q_OS_WIN

#include <windows.h>

const float DEFAULT_DPI = 96.0;

float windowsDpiScale();

#endif

#endif