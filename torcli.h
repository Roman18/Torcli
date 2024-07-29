#ifndef _TORCLI_H
#define _TORCLI_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dlfcn.h>


#define PROXYHOST "127.0.0.1"
#define PROXYPORT 9050


// {{{ Socks4 request definition 

/*
	+----+----+----+----+----+----+----+----+----+----+....+----+
	| VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
	+----+----+----+----+----+----+----+----+----+----+....+----+
	   1    1      2              4           variable       1
*/

typedef struct socks4_req{
	unsigned char vn;
	unsigned char cd;
	unsigned short int dstport;
	unsigned int dstip;
	char userid[8];
}Request;

Request* make_socks4_req(const struct sockaddr_in*);
void free_socks4_rs(void*);

// }}}

// {{{ Socks4 response definition

/*

	+----+----+----+----+----+----+----+----+
	| VN | CD | DSTPORT |      DSTIP        |
	+----+----+----+----+----+----+----+----+
	  1    1      2              4


*/

typedef struct socks4_resp{
	unsigned char vn;
	unsigned char cd;
	unsigned short int _;
	unsigned int __;
}Response;

// }}}


int connect(int, const struct sockaddr*, socklen_t);

#endif
