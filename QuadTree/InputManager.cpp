#include "InputManager.h"
#include <iostream>

void sy::InputManager::AddMouseListener(Observer* mouseListener)
{
	m_mouseMessage.AddObserver(mouseListener);
}

void sy::InputManager::AddKeyListener(Observer* keyListener)
{
	m_keyMessage.AddObserver(keyListener);
}

void sy::InputManager::RemoveMouseListener(Observer* mouseListener)
{
	m_mouseMessage.RemoveObserver(mouseListener);
}

void sy::InputManager::RemoveKeyListener(Observer* keyListener)
{
	m_keyMessage.RemoveObserver(keyListener);
}

void sy::InputManager::ProcessMessage()
{
	/* 获取系统消息 */
	if (!peekmessage(&m_exMsg))
	{
		return;
	}
	/* 根据消息类型派发消息 */
	DistributeMessage();
}

void sy::InputManager::DistributeMessage()
{
	ExMessageToMessage(m_exMsg, m_msg);
	switch (m_exMsg.message)
	{
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
		m_mouseMessage.NotifyAll(&m_msg);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		m_keyMessage.NotifyAll(&m_msg);
		break;
	}
}

void sy::InputManager::ExMessageToMessage(const ExMessage& exMsg, Message& msg)
{
	*(ExMessage*)(&msg) = exMsg;
}
