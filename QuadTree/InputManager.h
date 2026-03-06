/*
* InPutManager 旨在管理系统中用户的输入输出，这里我们通过 ProcessMessage() 获取系统的输入输出消息
*/
#pragma once
#ifndef _SY_INPUTMANAGER_H
#define	_SY_INPUTMANAGER_H
#include <easyx.h>
#include "KeyMessage.h"
#include "MouseMessage.h"
#include "Message.h"
#include "Singleton.h"

namespace sy 
{
	class InputManager: public Singleton<InputManager>
	{
	private:
		KeyMessage		m_keyMessage;
		MouseMessage	m_mouseMessage;

		ExMessage		m_exMsg;	
		Message			m_msg;

	public:
		void AddMouseListener(Observer* mouseListener);
		void AddKeyListener(Observer* keyListener);
		void RemoveMouseListener(Observer* mouseListener);
		void RemoveKeyListener(Observer* keyListener);

	public:
		void ProcessMessage();

	private:
		void DistributeMessage();
		void ExMessageToMessage(const ExMessage& exMsg, Message& msg);
	};
}
#endif // !_SY_INPUTMANAGER_H



