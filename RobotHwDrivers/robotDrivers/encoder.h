/*
 * encoder.h
 *
 *  Created on: Nov 8, 2017
 *      Author: daniel
 */

#ifndef ENCODER_H_
#define ENCODER_H_
#include <stdint.h>

class Encoder{
public:
	Encoder(int32_t low, int32_t high);
	int32_t getDelta();
	void update(int32_t new_count);
protected:
	uint32_t range;
	int32_t threshold_high;
	int32_t threshold_low;
	int32_t last;
	int32_t delta;
};


#endif /* ENCODER_H_ */
