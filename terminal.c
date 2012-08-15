
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

#include <stdio.h>
#include <unistd.h>

#include <string.h>		/* for memset */

#include <sys/ioctl.h>	/* for ioctl */

#include <sys/types.h>	/* for open, close, .. */
#include <sys/stat.h>	/* for open, close, .. */
#include <fcntl.h>		/* for open, close, .. */

#include <termios.h>	/* for tc* */

#include "terminal.h"

#ifdef _TERMINAL_DEBUG
#include <errno.h>
#define dbg(...) fprintf(stderr, __VA_ARGS__);
#else
#define dbg(...) 
#endif

static struct termios ttydefaulttio;
static int ttydefaultss;

int term_init(const char * device, unsigned int baud)
{
	int ret;
	int fd;
	
	unsigned int baudrate;
	struct termios newtio;
    int serstat;
	
	ret = open(device, O_RDWR | O_NOCTTY);
	if (ret < 0) {
		dbg("open failed. ret: %d (%d)\n", ret, errno);
		return -1;
	}
	fd = ret;
	
	ret = tcgetattr(fd, &ttydefaulttio);
	if (ret) {
		dbg("tcgetattr failed. ret: %d (%d)\n", ret, errno);
		return -1;
	}
	
	memset(&newtio, 0, sizeof(newtio));
    
	switch(baud){
        case 1200:      baudrate = B1200;    break;
        case 2400:      baudrate = B2400;    break;
        case 4800:      baudrate = B4800;    break;
        case 9600:      baudrate = B9600;    break;
        case 19200:     baudrate = B19200;   break;
        case 38400:     baudrate = B38400;   break;
        case 57600:     baudrate = B57600;   break;
        case 115200:    baudrate = B115200;  break;
        default :       
			dbg("invalid baudrate\n");
			return -1;
    }
	
    newtio.c_cflag = baudrate | CS8 | CLOCAL | CREAD;
	
    newtio.c_iflag = 0;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;
    
    newtio.c_cc[VTIME]    = 2;
	
    ret = tcflush(fd, TCIFLUSH);
    if (ret) {
        dbg("tcflush failed. ret: %d (%u)\n", ret, errno);
        return -1;
    }

    ret = tcsetattr(fd, TCSANOW, &newtio);
    if (ret) {
        dbg("txsetattr failed. ret: %d (%d)\n", ret, errno);
        return -1;
    }
	
    ioctl(fd, TIOCMGET, &serstat);
	ttydefaultss = serstat;
    serstat &= ~(TIOCM_RTS);
    serstat &= ~(TIOCM_DTR);
    ioctl(fd, TIOCMSET, &serstat);
	
	return fd;
}

int term_close(int fd)
{
	int ret;
	
    ioctl(fd, TIOCMSET, &ttydefaultss);
	
    ret = tcflush(fd, TCIFLUSH);
    if (ret) {
        dbg("tcflush failed. ret: %d (%u)\n", ret, errno);
        return -1;
    }

    ret = tcsetattr(fd, TCSANOW, &ttydefaulttio);
    if (ret) {
        dbg("txsetattr failed. ret: %d (%d)\n", ret, errno);
        return -1;
    }
	
	ret = close(fd);
	if (ret < 0) {
		dbg("close failed. ret: %d (%d)\n", ret, errno);
		return -1;
	}
	
	return 0;
}

int term_read(int fd, unsigned char * buf, int blen) 
{
	int ret;
	
	ret = read(fd, buf, blen);
	if (ret <= 0) {
		dbg("read failed. ret: %d (%d)\n", ret, errno);
		return ret;
	}
	
	return ret;
}

int term_write(int fd, const unsigned char * buf, int blen) 
{
	int ret;
	
	ret = write(fd, buf, blen);
	if (ret != blen) {
		dbg("write failed. ret: %d (%d)\n", ret, errno);
		return ret;
	}
	
	return ret;
}

