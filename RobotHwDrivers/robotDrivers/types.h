#ifndef _TYPES_H_
#define _TYPES_H_

typedef struct line_sensors{
	int left;
	int middle;
	int right;

	struct line_sensors operator=(struct line_sensors o){
		return o;
	};
} line_sensors_t;

typedef struct encoder {
	int16_t right;
	int16_t left;

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
