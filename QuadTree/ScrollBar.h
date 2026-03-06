#pragma once
#ifndef _SY_SCROLLBAR_H_
#define _SY_SCROLLBAR_H_
#include <easyx.h>
#include <string>
#include <windows.h>
#include <functional>
#include <map>
#include "ComponentBase.h"

namespace sy
{
	class ScrollBar : public ComponentBase
	{
		/* 绘制 ScrollBar 相关属性 */
	protected:
		double m_value;			// ScrollBar的值，[0.0 - 1.0]
		int x, y, r;			// 拖动的圆形的坐标以及
		COLORREF m_leftColor;
		COLORREF m_rightColor;
		COLORREF m_normalColor;
		COLORREF m_selectColor;

	public:
		void SetValue(double value = 0.5);
		void SetBarColor(COLORREF left = BLUE, COLORREF right = RGB(150,150,150));
		void SetCircleColor(COLORREF normal = WHITE, COLORREF select = GREEN);
		ScrollBar(double m_value = 0.5);
		virtual void Show() override;
		virtual void SetArea(Rect area) override;

		/* 设置按钮的值改变时的回调相关属性 */
	protected:
		bool m_click;				//是否被按下
		std::map<void*, std::function<void(double)>> m_invokes;
	public:
		void AddClickInvoke(std::function<void(double)> invoke);
		void RemoveClickInvoke(std::function<void(double)> invoke);
		void ClearClickInvoke();
		void OnInvoke();

	protected:
		void UpdateFromEventOnSelect(Event* event) override;

	public:
		virtual void Update() override;
		virtual void OnSelected() override;
		virtual void OnDisSelected() override;

	private:
		double Clamp(double value, double min, double max);
		void SetBarCirclePos();
	};
}

#endif // !_SY_SCROLLBAR_H_
