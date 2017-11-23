/*
 * circular_array.h
 *
 *  Created on: Nov 22, 2017
 *      Author: daniel
 */

#ifndef CIRCULAR_ARRAY_H_
#define CIRCULAR_ARRAY_H_

template<typename T>
class CircularArray{
public:
	explicit CircularArray(size_t size);
	~CircularArray();

	size_t write(const T * src, size_t n);
	size_t read(T * dest, size_t n);

private:
	T * buffer;
	size_t size;
	size_t head;
	size_t tail;
};




#endif /* CIRCULAR_ARRAY_H_ */
