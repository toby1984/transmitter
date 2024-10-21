
/*
 * TODO: Dirty hack to get rid of Clang LSP errors inside Kate editor as it obviously cannot see the variables set by the Makefile ....
 */

#ifndef F_CPU
#warning "F_CPU is undefined, assuming 16 MHz!"
#define F_CPU 16000000
#endif

#ifndef __AVR_ATmega328P__
#warning "MCU type not specified or unknown, assuming ATmega328P"
#define __AVR_ATmega328P__
#endif
