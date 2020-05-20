#ifndef _TYPES_H_
#define _TYPES_H_
#include <stdint.h>
typedef struct line_sensors{

	int16_t left;
	int16_t middle;
	int16_t right;

	struct line_sensors operator=(struct line_sensors o){
		return o;
	};
} line_sensors_t;

typedef struct encoder {
	int32_t right;
	int32_t left;

	struct encoder operator+(struct encoder other){
		other.right += right;
		other.left  += left;
		return other;
	}

	struct encoder operator-(struct encoder other){
		return {right-other.right,left-other.left};
	}
} encoder_t;

#endif
