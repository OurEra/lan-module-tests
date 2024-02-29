/*
 * BMUL_CNT.h
 *
 *  Created on: 2011-3-6
 *      Author: root
 */

#ifndef BMUL_CNT_H_
#define BMUL_CNT_H_

#include <stdio.h>
#include <string.h>

#define DATA_SIZE 	(128+1)
#define BUF_SIZE 		(DATA_SIZE*2)

enum ERR
{
	OK = 0,
	ERR_NUM_128,
	ERR_NUM_0_9,
	ERR_OVERFLOW
};

/*  return : 	OK
 * 				ERR_NUM_128
 * 				ERR_NUM_0_9
 * 				ERR_OVERFLOW
 *	  MyMul(Num_1, Num_2,  result);
  */
int MyMul(char*, char* , char*);

#endif /* BMUL_CNT_H_ */
