#pragma once
#ifndef _SY_DRAWABLE_H_
#define _SY_DRAWABLE_H_

namespace sy
{
	class DrawAble
	{
	public:
		DrawAble() {};
		virtual ~DrawAble() {};

	public:
		virtual void Update() = 0;;
		virtual void Show() = 0;
	};
}

#endif // !_SY_DRAWABLE_H_

