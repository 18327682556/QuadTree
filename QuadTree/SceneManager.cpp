#include "SceneManager.h"

void sy::SceneManager::Add(DrawAble* elem)
{
	m_allObjects.insert(elem);
}

void sy::SceneManager::Remove(DrawAble* elem)
{
	auto it = m_allObjects.find(elem);
	if (it != m_allObjects.end())
		m_allObjects.erase(it);
}

void sy::SceneManager::Clear()
{
	m_allObjects.clear();
}

void sy::SceneManager::Update()
{
	for (auto it : m_allObjects)
	{
		if(it != nullptr)
			it->Update();
	}
}

void sy::SceneManager::Show()
{
	for (auto it : m_allObjects)
	{
		if (it != nullptr)
			it->Show();
	}
}
