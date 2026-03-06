#pragma once
#ifndef _CANVAS_H_
#define _CANVAS_H_
#include <graphics.h>
#include "QuadTree.h"

// 对 easyx 的绘图函数进行封装，坐标系被映射到笛卡尔坐标系了。（单例模式）
class Canvas
{
private :
	static Canvas* m_pInstance;
	static int m_xScreen;			// 屏幕宽度
	static int m_yScreen;			// 屏幕高度
public:
	static Canvas& GetInstance();
	static bool SetCanvas(int width, int height);		// 设置画布长宽
	static int GetWidth() { return m_xScreen; }			// 设置宽度 (在 GetInstance 调用之前调用)
	static int GetHeight() { return m_yScreen; }		// 设置长度 (在 GetInstance 调用之前调用)
	~Canvas();

public:
	void Present();					// 将当前画布上的东西呈现出来
	void Clear();					// 清除画布当前所有内容

	void Circle(float x, float y, float r, COLORREF lineColor = WHITE);
	void SolidCircle(float x, float y, float r, COLORREF color = WHITE);
	void FillCircle(float x, float y, float r, COLORREF lineColor = WHITE, COLORREF fillColor = BLUE);
	
	void Rectangle(const Rect& rect, COLORREF lineColor);

private:
	Canvas(int xScreen, int yScreen);
	
};
#endif // !_CANVAS_H_

