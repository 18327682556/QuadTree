#pragma once
#ifndef _SY_OBSERVER_H
#define _SY_OBSERVER_H
#include "Event.h"
namespace sy
{
	class Observer
	{
	public:
		virtual void Update(Event *event) = 0;
	};
}

#endif // !_SY_OBSERVER_H




