/*
 * i2c.h
 *
 *  Created on: Jul 19, 2022
 *      Author: chlsk
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

void I2C1_init(void);
void I2C1_byteRead(char saddr, char maddr, char* data);
void I2C1_burstRead(char saddr, char maddr, int n, char* data);
void I2C1_burstWrite(char saddr, char maddr, int n, char* data);



#endif /* INC_I2C_H_ */
