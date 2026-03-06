#include <iostream>
#include "Button.h"
#include "Message.h"

sy::Button::Button()
	: m_click(false)
{
	SetTextColor(RGB(255,255,0), RGB(0,0,0));
	SetTextStyle(20);
	SetColor(RGB(100,100,100),RGB(255,255,0));
	SetText();
}

void sy::Button::Show()
{
	if (!m_visualAble) return;	// 不可见

	if (!m_isActive)		// 没有处于激活状态
	{
		settextcolor(RGB( GetRValue(m_textColor) * 0.1, GetGValue(m_textColor) * 0.1, GetBValue(m_textColor) * 0.1));
		LOGFONT f;
		gettextstyle(&f);							// 获取当前字体设置
		f.lfHeight = m_textSize;					// 设置字体高度为 48
		wcscpy_s(f.lfFaceName, m_textType.c_str());	// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
		f.lfQuality = ANTIALIASED_QUALITY;			// 设置输出效果为抗锯齿  
		settextstyle(&f);							// 设置字体样式
		setfillcolor(RGB(GetRValue(m_color) * 0.1, GetGValue(m_color) * 0.1, GetBValue(m_color) * 0.1));
		fillroundrect(m_area.left(), m_area.top(), m_area.right(), m_area.bottom(), 20, 20);
		m_tmpRECT = { m_area.left(), m_area.top(), m_area.right(), m_area.bottom() };
		drawtext(m_text.c_str(), &m_tmpRECT, m_textFormat);
		return;
	}

	if (m_selected)		// 按钮被选中
	{
		settextcolor(m_textSelectColor);
		LOGFONT f;
		gettextstyle(&f);							// 获取当前字体设置
		f.lfHeight = m_textSize * 1.3;				// 设置字体高度为 48
		wcscpy_s(f.lfFaceName, m_textType.c_str());	// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
		f.lfQuality = ANTIALIASED_QUALITY;			// 设置输出效果为抗锯齿  
		settextstyle(&f);							// 设置字体样式
		setfillcolor(m_selectColor);
		fillroundrect(m_area.left(), m_area.top(), m_area.right(), m_area.bottom(), 20, 20);
		m_tmpRECT = { m_area.left(), m_area.top(), m_area.right(), m_area.bottom()};
		drawtext(m_text.c_str(), &m_tmpRECT, m_textFormat);
	}
	else
	{
		settextcolor(m_textColor);
		LOGFONT f;
		gettextstyle(&f);							// 获取当前字体设置
		f.lfHeight = m_textSize;					// 设置字体高度为 48
		wcscpy_s(f.lfFaceName, m_textType.c_str());	// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
		f.lfQuality = ANTIALIASED_QUALITY;			// 设置输出效果为抗锯齿  
		settextstyle(&f);							// 设置字体样式
		setfillcolor(m_color);
		fillroundrect(m_area.left(), m_area.top(), m_area.right(), m_area.bottom(), 20, 20);
		m_tmpRECT = { m_area.left(), m_area.top(), m_area.right(), m_area.bottom()};
		drawtext(m_text.c_str(), &m_tmpRECT, m_textFormat);
	}
}

void sy::Button::SetTextColor(COLORREF normal, COLORREF select)
{
	m_textColor = normal;
	m_textSelectColor = select;
}

void sy::Button::SetTextStyle(int textSize, std::wstring type, UINT format)
{
	m_textSize = textSize;
	m_textType = type;
	m_textFormat = format;
}

void sy::Button::SetColor(COLORREF normal, COLORREF select)
{
	m_color = normal;
	m_selectColor = select;
}

void sy::Button::SetText(std::wstring text)
{
	m_text = text;
}

void sy::Button::AddClickInvoke(std::function<void(void)> invoke)
{
	m_invokes[&invoke] = invoke;
}

void sy::Button::RemoveClickInvoke(std::function<void(void)> invoke)
{
	std::map<void*, std::function<void(void)>>::iterator it = m_invokes.find(&invoke);
	if (it != m_invokes.end())
	{
		m_invokes.erase(it);
	}
}

void sy::Button::ClearClickInvoke()
{
	m_invokes.clear();
}

void sy::Button::Click()
{
	for (auto it : m_invokes)
	{
		it.second();
	}
}

void sy::Button::UpdateFromEventOnSelect(Event* event)
{
	if (!m_isActive) return;

	Message* msg = (Message*)event;
	if (msg->message == WM_LBUTTONDOWN)
	{
		m_click = true;
		Click();
	}
	if (msg->message == WM_LBUTTONUP)
	{
		m_click = false;
	}
}

void sy::Button::Update()
{
}

void sy::Button::OnSelected()
{

}

void sy::Button::OnDisSelected()
{
	m_click = false;
}
