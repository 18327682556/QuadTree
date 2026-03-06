#pragma once
#include "DrawAble.h"
#ifndef _SY_LABEL_H_
#define _SY_LABEL_H_
#include <easyx.h>
#include <string>
#include <windows.h>
#include <functional>
#include <map>
#include "ComponentBase.h"

namespace sy
{
	class Label : public ComponentBase
	{
		/* Label 绘制相关属性 */
	protected:
		UINT			m_textFormat;		// 文字显示类型
		std::wstring	m_textType;			// 文字字体类型
		int				m_textSize;			// 文字大小
		std::wstring	m_text;				// 按钮显示的文字
		COLORREF		m_textColor;		// 按钮文字颜色
		RECT			m_tmpRECT;			// 绘画使用的
	public:
		Label(std::wstring text = L"Label");
		virtual void Show() override;
		void SetTextColor(COLORREF normal);
		void SetTextStyle(int textSize = 15, std::wstring type = L"新宋体", UINT format = DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		void SetText(std::wstring text = L"Label");

	public:
		virtual void Update() override;
		virtual void OnSelected() override;
		virtual void OnDisSelected() override;
	};
}

#endif // !_SY_LABEL_H_