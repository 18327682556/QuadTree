#pragma once
#ifndef _SY_SINGLETON_H_
#define _SY_SINGLETON_H_

namespace sy
{
	template<class T>
	class Singleton
	{
	protected:
		static T* instance;
		Singleton() {};
		Singleton(const Singleton&) {};
		Singleton& operator=(const Singleton&) {};
	public:
		static T& GetInstance();
	};

	template<class T>
	T* Singleton<T>::instance = nullptr;


	template<class T>
	inline T& Singleton<T>::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new T;
		}
		return *instance;
	}

}

#endif // !_SY_SINGLETON_H_
