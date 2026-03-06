#pragma once
#ifndef _SY_SUBJECT_H
#define _SY_SUBJECT_H
#include <set>
#include "Event.h"
#include "Observer.h"

using std::set;
namespace sy
{
	class Subject
	{
	private:
		set<Observer*> m_observers;

	public:
		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);
		void NotifyAll(Event* event);
	};
}
#endif // !_SY_SUBJECT_H


