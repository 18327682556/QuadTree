#include "ScrollBar.h"
#include "Message.h"


void sy::ScrollBar::SetValue(double value)
{
	m_value = value;
	SetBarCirclePos();
}

void sy::ScrollBar::SetBarColor(COLORREF left, COLORREF right)
{
	m_leftColor = left;
	m_rightColor = right;
}

void sy::ScrollBar::SetCircleColor(COLORREF normal, COLORREF select)
{
	m_normalColor = normal;
	m_selectColor = select;
}

sy::ScrollBar::ScrollBar(double m_value)
	:m_click(false)
{
	SetValue(0.5);
	SetBarColor(RGB(0, 0, 255), RGB(50, 50, 50));
	SetCircleColor(RGB(155,0,155),RGB(255,255,0));
	SetBarCirclePos();
}

void sy::ScrollBar::Show()
{
	if (!m_visualAble) return;

	if (!m_isActive)
	{
		float barHeight = (float)r;
		setfillcolor(RGB(GetRValue(m_leftColor) * 0.1, GetGValue(m_leftColor) * 0.1, GetBValue(m_leftColor) * 0.1));
		fillroundrect(m_area.left() + r, int(m_area.top() + (m_area.height - barHeight) / 2), x, int(m_area.bottom() - (m_area.height - barHeight) / 2), (int)barHeight, (int)barHeight);
		setfillcolor(RGB(GetRValue(m_rightColor) * 0.1, GetGValue(m_rightColor) * 0.1, GetBValue(m_rightColor) * 0.1));
		fillroundrect(x, int(m_area.top() + (m_area.height - barHeight) / 2), m_area.right() - r, int(m_area.bottom() - (m_area.height - barHeight) / 2), (int)barHeight, (int)barHeight);
		if (m_click)
		{
			setfillcolor(RGB(GetRValue(m_selectColor) * 0.1, GetGValue(m_selectColor) * 0.1, GetBValue(m_selectColor) * 0.1));
		}
		else
		{
			setfillcolor(RGB(GetRValue(m_normalColor) * 0.1, GetGValue(m_normalColor) * 0.1, GetBValue(m_normalColor) * 0.1));
		}
		fillcircle(x, y, r);
		return;
	}


	float barHeight = (float)r;
	setfillcolor(m_leftColor);
	fillroundrect(m_area.left() + r, int(m_area.top() + (m_area.height - barHeight) / 2), x, int(m_area.bottom() - (m_area.height - barHeight) / 2), (int)barHeight, (int)barHeight);
	setfillcolor(m_rightColor);
	fillroundrect(x, int(m_area.top() + (m_area.height - barHeight) / 2), m_area.right() - r, int(m_area.bottom() - (m_area.height - barHeight) / 2), (int)barHeight, (int)barHeight);
	if (m_click)
	{
		setfillcolor(m_selectColor);
	}
	else
	{
		setfillcolor(m_normalColor);
	}
	fillcircle(x, y, r);
}


void sy::ScrollBar::SetArea(Rect area)
{
	ComponentBase::SetArea(area);
	SetBarCirclePos();
}

void sy::ScrollBar::AddClickInvoke(std::function<void(double)> invoke)
{
	m_invokes[&invoke] = invoke;
}

void sy::ScrollBar::RemoveClickInvoke(std::function<void(double)> invoke)
{
	std::map<void*, std::function<void(double)>>::iterator it = m_invokes.find(&invoke);
	if (it != m_invokes.end())
	{
		m_invokes.erase(it);
	}
}

void sy::ScrollBar::ClearClickInvoke()
{
	m_invokes.clear();
}

void sy::ScrollBar::OnInvoke()//回调
{
	for (auto it : m_invokes)
	{
		it.second(m_value);
	}
}

void sy::ScrollBar::UpdateFromEventOnSelect(Event* event)
{
	if (!m_isActive) return;

	Message* msg = (Message*)event;
	if (!Rect(x - r, y - r, 2 * r, 2 * r).Inside(msg->x, msg->y))
	{
		m_click = false;
		return;
	}

	switch (msg->message)
	{
	case WM_LBUTTONDOWN:
		m_click = true;
		break;
	case WM_LBUTTONUP:
		m_click = false;
		break;
	case WM_MOUSEMOVE:
		if (m_click)
		{
			m_value = Clamp(((double)msg->x - (double)r - (double)m_area.left()) / (double)((double)m_area.width - (double)r - (double)r), 0.0, 1.0);
			SetBarCirclePos();
			OnInvoke();
		}
		break;
	}
}

void sy::ScrollBar::Update()
{

}

void sy::ScrollBar::OnSelected()
{

}

void sy::ScrollBar::OnDisSelected()
{
	m_click = false;
}

double sy::ScrollBar::Clamp(double value, double min, double max)//限定边界值
{
	return (value < min) ? (min) : (value > max ? max : value);
}

void sy::ScrollBar::SetBarCirclePos()
{
	y = m_area.y + (m_area.height >> 1);
	r = min(m_area.width - 6, m_area.height - 6) / 2;
	r = (int)Clamp(r, 3, 25);
	x = int((m_area.x + r) + (m_area.width - r - r) * m_value);
}
