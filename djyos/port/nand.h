/*
 * nand.h
 *
 *  Created on: Mar 5, 2013
 *      Author: John
 */

#ifndef _NAND_H_
#define _NAND_H_
#ifdef __cplusplus
extern "C" {
#endif

int nand_read(unsigned int nand_start, unsigned char* buf, unsigned int len);
int nand_write(unsigned int nand_start, unsigned char* buf, unsigned int len);
void nand_erase(unsigned int addr);

#ifdef __cplusplus
}
#endif

#endif /* NAND_H_ */
