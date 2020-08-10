// this is added by LEE jeun jeun@wayne-inc.com
#include "WinDpiScale.h"

float windowsDpiScale()
{
	HDC screen = GetDC(0);
	FLOAT dpiX = static_cast<FLOAT>(GetDeviceCaps(screen, LOGPIXELSX));
	ReleaseDC(0, screen);
	return dpiX / DEFAULT_DPI;
}