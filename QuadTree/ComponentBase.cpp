#include "ComponentBase.h"
#include "Message.h"

void sy::ComponentBase::Update(Event* event)
{
	Message* msg = (Message*)event;
	// 更新组件被选中状态
	if (m_area.Inside(msg->x, msg->y))
	{
		if (!m_selected)
		{
			m_selected = true;
			OnSelected();
		}
	}
	else
	{
		if (m_selected)
		{
			m_selected = false;
			OnDisSelected();
		}
	}

	// 消息转发给子类
	if (m_selected)
	{
		UpdateFromEventOnSelect(event);
	}
	UpdateFromEvent(event);
}
