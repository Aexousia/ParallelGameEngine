#pragma once

template<typename T>
class Singleton
{
public:
	static T* getInstance()
	{
		if (!m_instance)
		{
			m_instance = new T();
		}

		return m_instance;
	}
private:
	static T* m_instance;
};

template<typename T>
T* Singleton<T>::m_instance = nullptr;

#define SINGLETON(x) x::getInstance()

//// Usage defined below

//class X : public Singleton<X>
//{
//public: 
//	int member()
//	{
//
//	}
//};
//
//int main()
//{
//	SINGLETON(X)->member();
//}