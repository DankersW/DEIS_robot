/*
 * i2c.h
 *
 *  Created on: Nov 22, 2017
 *      Author: daniel
 */

#ifndef I2C_H_
#define I2C_H_

class I2C {
public:
	I2C();
	I2C(void(*sendData)(), void(*receiveData)(int));
	static const int SLAVE_ADDRESS = 0x04;

private:
	void(*sendData)();
	void(*receiveData)(int);
};



#endif /* I2C_H_ */
