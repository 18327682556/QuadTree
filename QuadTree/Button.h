#pragma once
#ifndef _SY_BUTTON_H_
#define _SY_BUTTON_H_
#include <easyx.h>
#include <string>
#include <windows.h>
#include <functional>
#include <map>
#include "ComponentBase.h"

namespace sy
{
	class Button : public ComponentBase
	{
		/* 按钮绘制相关属性 */
	protected:
		UINT			m_textFormat;		// 文字显示类型
		std::wstring	m_textType;			// 文字字体类型
		int				m_textSize;			// 文字大小
		std::wstring	m_text;				// 按钮显示的文字
		COLORREF		m_textColor;		// 按钮文字颜色
		COLORREF		m_textSelectColor;	// 按钮文字选中时颜色
		COLORREF		m_selectColor;		// 选中时的颜色
		COLORREF		m_color;			// 按钮正常颜色
		RECT			m_tmpRECT;			// 绘画使用的
	public:
		Button();
		virtual void Show() override;
		void SetTextColor(COLORREF normal, COLORREF select = RED);
		void SetTextStyle(int textSize = 15, std::wstring type = L"新宋体", UINT format = DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		void SetColor(COLORREF normal, COLORREF select = GREEN);
		void SetText(std::wstring text = L"Button");

		/* 按钮回调相关属性 */
	protected:
		bool m_click;				// 按钮是否被按下
		std::map<void* ,std::function<void(void)>> m_invokes;
	public:
		void AddClickInvoke(std::function<void(void)> invoke);
		void RemoveClickInvoke(std::function<void(void)> invoke);
		void ClearClickInvoke();
		void Click();
	protected:
		void UpdateFromEventOnSelect(Event* event) override;

	public:
		virtual void Update() override;
		virtual void OnSelected() override;
		virtual void OnDisSelected() override;
	};
}

#endif // !_SY_BUTTON_H_