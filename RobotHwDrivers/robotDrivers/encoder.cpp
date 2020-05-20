/*
 * encoder.cpp
 *
 *  Created on: Nov 8, 2017
 *      Author: daniel
 */

#include "encoder.h"

Encoder::Encoder(int32_t low, int32_t high)
	: range(high-low+1)
	// 30 percent hysteris
	, threshold_low(low + range*0.3)
	, threshold_high(low + range*0.7)
	, last(0)
	, delta(0){
}

void Encoder::update(int32_t new_count){
	int32_t delta;
	if((last > threshold_high) && (new_count < threshold_low)){
		delta = new_count + range - last;
	}
	else if((last < threshold_low) && (new_count > threshold_high)){
		delta = new_count - range - last;
	}
	else{
		delta = new_count - last;
	}
	this->delta += delta;
	this->last = new_count;
}


int32_t Encoder::getDelta(){
	int32_t delta = this->delta;
	this->delta = 0;
	//if (isReversed())
	//	return -delta;
	//else
	return delta;
}
