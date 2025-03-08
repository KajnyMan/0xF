/*
 * Dynamiczna tablica a'la std::vector, ktory w starych kompilatorach jest
 * problem.
 *
 */

#ifndef sag_h
#define sag_h

//#include <stddef.h>

template<class T>
class Sag
{
	T* _ptr;
	int _capacity;
	int _next;

	public:
	Sag();
	~Sag();
	void push_back( const T& );
	void drop( const int& );
	int size(){ return _next; }
	int capacity(){ return _capacity; }
	void clear();
	void resize( const int& );
	T& at( int );
	T& operator[]( int i ){ return _ptr[i]; }
};
/*******************************************/
template<class T>
Sag<T>::Sag() : _capacity(0), _next(0), _ptr( 0 )
{
}
/*******/
template<class T>
Sag<T>::~Sag()
{
	delete []_ptr;
}
/******/
template<class T>
void Sag<T>::push_back( const T& obj )
{
	if ( !(_capacity > _next) )
	{
		int temp_capacity = _capacity + 5;
		T* temp = new T[temp_capacity];
		for( int i = 0; i < temp_capacity; i++ )
		{
			if ( i < _capacity ) temp[i] = _ptr[i];
			else i = temp_capacity - 1;
		}
		if ( _ptr ) delete []_ptr;
		_ptr = temp;
		temp = 0;
		_capacity = temp_capacity;
	}
	_ptr[_next] = obj;
	_next++;
}
template<class T>
void Sag<T>::drop( const int& a )
{
	if ( a >= 0 && a < _next )
	{
		for( int i = a ; i < _next - 1; i++ )
		{
			_ptr[i] = _ptr[i + 1];
		}
		_next--;
	}
}
/******/
template<class T>
T& Sag<T>::at( int i )
{
	return _ptr[i];
}
template<class T>
void Sag<T>::clear()
{
	_next = 0;
}
template<class T>
void Sag<T>::resize( const int& s )
{
	if ( _next < s )
	{
		T* temp = new T[s];
		for( int i = 0; i < s; i++ )
		{
			if ( i < _capacity ) temp[i] = _ptr[i];
			else i = s - 1;
		}
		if ( _ptr ) delete []_ptr;
		_ptr = temp;
		temp = 0;
		_capacity = s;
		
	}
}
#endif // sag_h
