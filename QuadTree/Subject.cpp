#include "Subject.h"

void sy::Subject::AddObserver(Observer* observer)
{
	m_observers.insert(observer);
}

void sy::Subject::RemoveObserver(Observer* observer)
{
	auto it = m_observers.find(observer);
	if (it != m_observers.end())
	{
		m_observers.erase(it);
	}
}

void sy::Subject::NotifyAll(Event* event)
{
	for (auto it : m_observers)
	{
		it->Update(event);
	}
}
