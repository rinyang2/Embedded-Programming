/*
 * file.h
 *
 *  Created on: 26/12/2011
 *      Author: Morten
 */

#ifndef FILE_H_
#define FILE_H_

#include "emp_type.h"

typedef INT8U* FILE;

#define COM1  0      // UART
#define COM2  1      // LCD
#define COM3  2      // Keyboard

BOOLEAN put_file( FILE, INT8U );
BOOLEAN get_file( FILE, INT8U* );
extern INT8U init_files();

#endif /* FILE_H_ */
