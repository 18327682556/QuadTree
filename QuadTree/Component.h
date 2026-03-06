#pragma once
#ifndef _SY_COMPONENT_H_
#define _SY_COMPONENT_H_
#include "DrawAble.h"
namespace sy
{
	class Component : virtual public DrawAble
	{
	public:
		struct Rect
		{
			int x, y, width, height;
			int x1()	{ return x; }
			int x2()	{ return x + width; }
			int y1()	{ return y; }
			int y2()	{ return y + height; }
			int left()	{ return x1(); }
			int right() { return x2(); }
			int top()	{ return y1(); }
			int bottom(){ return y2(); }
			Rect(int _x = 0, int _y = 0, int _width = 0, int _height = 0) :x(_x), y(_y), width(_width), height(_height) {}
			bool Inside(int x, int y) { return x >= x1() && x <= x2() && y >= y1() && y <= y2(); }
		};

	public:
		Component() {}
		virtual ~Component() {}

	public:
		virtual void Update() = 0;			// 每帧调用一次，用于更新组件
		virtual void Show() = 0;			// 每帧调用一次，用于绘制组件
	};
}

#endif // !_SY_COMPONENT_H_s
