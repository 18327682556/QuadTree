#include "Label.h"

sy::Label::Label(std::wstring text)
{
	SetTextColor(WHITE);
	SetTextStyle();
	SetText(text);
}

void sy::Label::Show()
{
	if (!m_visualAble)
		return;

	if (!m_isActive)
	{
		settextcolor(RGB(GetRValue(m_textColor) * 0.1, GetGValue(m_textColor) * 0.1, GetBValue(m_textColor) * 0.1));
	}
	else
	{
		settextcolor(m_textColor);
	}
	m_tmpRECT = { m_area.left(), m_area.top(), m_area.right(), m_area.bottom() };
	
	LOGFONT f;
	gettextstyle(&f);							// 获取当前字体设置
	f.lfHeight = m_textSize;					// 设置字体高度为 48
	wcscpy_s(f.lfFaceName, m_textType.c_str());	// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	f.lfQuality = ANTIALIASED_QUALITY;			// 设置输出效果为抗锯齿  
	settextstyle(&f);							// 设置字体样式
	drawtext(m_text.c_str(), &m_tmpRECT, m_textFormat);
}

void sy::Label::SetTextColor(COLORREF normal)
{
	m_textColor = normal;
}

void sy::Label::SetTextStyle(int textSize, std::wstring type, UINT format)
{
	m_textSize = textSize;
	m_textType = type;
	m_textFormat = format;
}

void sy::Label::SetText(std::wstring text)
{
	m_text = text;
}

void sy::Label::Update()
{
}

void sy::Label::OnSelected()
{
}

void sy::Label::OnDisSelected()
{
}
