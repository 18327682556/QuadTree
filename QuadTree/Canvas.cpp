#include "Canvas.h"
#include "Config.h"

int Canvas::m_xScreen = 800;
int Canvas::m_yScreen = 800;
Canvas* Canvas::m_pInstance = nullptr;

Canvas& Canvas::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new Canvas(m_xScreen, m_yScreen);
	}
	return *m_pInstance;
}

bool Canvas::SetCanvas(int width, int height)
{
	if (m_pInstance == nullptr)
	{
		m_xScreen = width;
		m_yScreen = height;
		return true;
	}
	return false;
}

Canvas::~Canvas()
{
	EndBatchDraw();
	closegraph();
}

void Canvas::Present()
{
	FlushBatchDraw();
}

void Canvas::Clear()
{
	cleardevice();
}

void Canvas::Circle(float x, float y, float r, COLORREF lineColor)
{
	setlinecolor(lineColor);
	circle(int(x + 0.5f), int(y + 0.5f), int(r + 0.5f));
}

void Canvas::SolidCircle(float x, float y, float r, COLORREF color)
{
	setfillcolor(color);
	solidcircle(int(x + 0.5f), int(y + 0.5f), int(r + 0.5f));
}

void Canvas::FillCircle(float x, float y, float r, COLORREF lineColor, COLORREF fillColor)
{
	setlinecolor(lineColor);
	setfillcolor(fillColor);
	fillcircle(int(x + 0.5f), int(y + 0.5f), int(r + 0.5f));
}

void Canvas::Rectangle(const Rect& rect, COLORREF lineColor)
{
	setlinecolor(lineColor);
	rectangle(int(rect.x + 0.5f), int(rect.y + 0.5f), int(rect.x + rect.width + 0.5f), int(rect.y + rect.height + 0.5f));
}

Canvas::Canvas(int xScreen, int yScreen)
{
#ifdef _SY_DEBUG_
	initgraph(xScreen, yScreen, true);
#else
	initgraph(xScreen, yScreen);
#endif

	BeginBatchDraw();
	setbkmode(TRANSPARENT);
}
