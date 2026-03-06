/* 将每个屏幕上的一个页面视为一个场景，场景中可以有很多可绘制对象 */
#pragma once
#ifndef _SY_SCENEMANAGER_H
#define	_SY_SCENEMANAGER_H
#include <set>
#include <easyx.h>
#include "Singleton.h"
#include "DrawAble.h"

namespace sy
{
	class SceneManager : public Singleton<SceneManager>
	{
	private:
		std::set<DrawAble*> m_allObjects;

	public:
		void Add(DrawAble* elem);
		void Remove(DrawAble* elem);
		void Clear();

	public:
		void Update();
		void Show();

	};
}
#endif // !_SY_SCENEMANAGER_H


