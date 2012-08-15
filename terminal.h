
/*
 * Serial terminal interface for Unix/Linux systems
 *
 * Author: Andras Takacs <andras.takacs@emsol.hu>
 *
 * Version 0.2
 *
 * Copyright (c) 2009-2012 Emsol Mernok Iroda
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the Emsol Mernok Iroda.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _TERMINAL_H
#define _TERMINAL_H

/*!
 * \mainpage Serial terminal interface for Unix/Linux systems
 *
 * Basically it has been designed for FTDI serial interfaces,
 * but it should work with others as well.
 *
 * To enable debugging, define _TERMINAL_DEBUG
 */

//#define _TERMINAL_DEBUG

/*!
 * \brief Initialize terminal interface
 *
 * It configures the serial terminal for read-write usage.
 * It is not thread safe! It stores the default terminal 
 * configuration in static variables!
 *
 * After usage, the term_close function restores the default 
 * settings.
 *
 * \param device Device path, i.e. /dev/ttyUSB0
 * \param baud Terminal baud rate, i.e. 115200
 *
 * \return File dscriptor of the terminal if OK, otherwise negative
 */
int term_init(const char * device, unsigned int baud);

/*!
 * \brief Restore default terminal settings
 *
 * Restore the default settings of the previously initialized 
 * terminal.
 *
 * \param fd Opened file descriptor of the terminal
 *
 * \return Zero if OK, otherwise negative
 */
int term_close(int fd);

/*!
 * \brief Read data form the terminal
 *
 * It reads up to blen bytes form the previously initialized terminal
 *
 * \param fd Opened file descriptor of the terminal
 * \param buf Buffer, where it puts the data
 * \param blen Size of buf buffer
 *
 * \return Amount of the read data if OK, otherwise zero (closed) or negative (error)
 */
int term_read(int fd, unsigned char * buf, int blen);

/*!
 * \brief Write data to the terminal
 *
 * It writes blen bytes to the previously initialized terminal
 *
 * \param fd Opened file descriptor of the terminal
 * \param buf Buffer, which contains the data
 * \param blen Size of buf buffer
 *
 * \return Amount of the written data if OK, otherwise zero (closed) or negative (error)
 */
int term_write(int fd, const unsigned char * buf, int blen);

#endif
