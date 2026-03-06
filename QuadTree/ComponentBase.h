#pragma once
#ifndef _SY_COMPONENTBASE_H_
#define _SY_COMPONENTBASE_H_
#include "Component.h"
#include "Observer.h"
namespace sy
{
	class ComponentBase :virtual public Component, virtual public Observer
	{
	protected:
		Rect m_area;		// 每个组件在屏幕上的区域
		bool m_isActive;	// 每个组件是否被激活
		bool m_visualAble;	// 组件可见性
		bool m_selected;	// 鼠标是否在组件区域中选中

		ComponentBase(Rect area = { 0,0,0,0 }, bool isActive = true, bool visualAble = true)
			:m_area(area), m_isActive(isActive), m_visualAble(visualAble), m_selected(false) {}
	public:
		virtual void SetArea(Rect area) { m_area = area; }			
		virtual void SetActive(bool active) { m_isActive = active; }
		virtual void SetVisualAble(bool visualAble) { m_visualAble = visualAble; }
		virtual const Rect& GetArea() { return m_area; }
		virtual bool GetActive() { return m_isActive; }
		virtual bool GetVisualAble() { return m_visualAble; }

	private:
		virtual void Update(Event* event) override;					// 阻止子类进行重写，在该方法中我们派发下面两种消息给子类

	protected:
		virtual void UpdateFromEventOnSelect(Event* event) {};		// 鼠标处于该组件的区域中后，每次有系统消息（EM_CHAR, EM_KEY, EM_MOUSE等）都会被调用
		virtual void UpdateFromEvent(Event* event) {};				// 每次有系统消息（EM_CHAR, EM_KEY, EM_MOUSE等）都会被调用

	public:
		virtual void Update() {};			// 每帧调用一次，用于更新组件, 子类不是必须实现此接口，接口来自于 DrawAble
		virtual void Show() = 0;			// 每帧调用一次，用于绘制组件, 子类必须实现 ComponentBase 不能被实例化

	public:
		virtual void OnSelected() {};		// 组件从未被选中更换为被选中时调用一次
		virtual void OnDisSelected() {};	// 组件从被选中更换为未被选中时调用一次
	};
}
#endif // !_SY_COMPONENTBASE_H_