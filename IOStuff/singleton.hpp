#ifndef SINGLETON_H_
#define SINGLETON_H_

template < class T >
class Singleton
{
protected:
	Singleton(){}
	Singleton(const Singleton&) = delete;
	Singleton & operator= (const Singleton&) = delete;
	
	static T * instance;
public:
	static T& get();
	static void release();
};

 template <class T>
 T* Singleton<T>::instance = nullptr;
 
 template < class T>
  T& Singleton<T>::get()
  {
  	if(!instance) instance = new T();
  	return *instance;
  }
  
  template < class T >
  void Singleton<T>::release()
  {
  	delete instance;
  }
  
  #endif