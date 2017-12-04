/*
 * circular_array.h
 *
 *  Created on: Nov 22, 2017
 *      Author: daniel
 */

#ifndef CIRCULAR_ARRAY_H_
#define CIRCULAR_ARRAY_H_
#include <stddef.h>

template<typename T>
class CircularArray{
public:
	explicit CircularArray(size_t size)
		: size(size)
		, head(0)
		, tail(0) {
		buffer = new T[size];
	};
	//explicit CircularArray(size_t size, T init);
	~CircularArray(){
		delete[] buffer;
	};

	size_t write(const T * src, size_t n){
	    //\fn void *memcpy(void *dest, const void *src, size_t len)
		//memcpy()
	};
	size_t read(T * dest, size_t n);

private:
	T * buffer;
	size_t size;
	size_t head;
	size_t tail;
};




#endif /* CIRCULAR_ARRAY_H_ */
