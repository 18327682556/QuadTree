#pragma once
#ifndef _SY_MESSAGE_H
#define _SY_MESSAGE_H
#include <windows.h>
#include "Event.h"

namespace sy
{
	struct Message: public Event		// 这里引用了 ExMessage 结构的设计，只是更改了我自己的继承关系
	{
		USHORT message;					// 消息标识
		union
		{
			// 鼠标消息的数据
			struct
			{
				bool ctrl : 1;			// Ctrl 键是否按下
				bool shift : 1;			// Shift 键是否按下
				bool lbutton : 1;		// 鼠标左键是否按下
				bool mbutton : 1;		// 鼠标中键是否按下
				bool rbutton : 1;		// 鼠标右键
				short x;				// 鼠标的 x 坐标
				short y;				// 鼠标的 y 坐标
				short wheel;			// 鼠标滚轮滚动值，为 120 的倍数
			};

			// 按键消息的数据
			struct
			{
				BYTE vkcode;			// 按键的虚拟键码
				BYTE scancode;			// 按键的扫描码（依赖于 OEM）
				bool extended : 1;		// 按键是否是扩展键
				bool prevdown : 1;		// 按键的前一个状态是否按下
			};

			// 字符消息的数据
			TCHAR ch;

			// 窗口消息的数据
			struct
			{
				WPARAM wParam;
				LPARAM lParam;
			};
		};
	};
}
#endif // !_SY_MESSAGE_H
