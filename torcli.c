#include "torcli.h"




Request* make_socks4_req(const struct sockaddr_in *sock){
	Request *req = malloc(sizeof(Request));
	if(req == NULL){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	req->vn = 4;
	req->cd = 1;
	req->dstport = sock->sin_port;
	req->dstip = sock->sin_addr.s_addr;
	memset(req->userid, 0, 8);
	memcpy(req->userid, "torcli)", 7);
	return req;
}

void free_socks4_rs(void *rs){
	if(rs != NULL)
		free(rs);
}

int connect(int orig_fd, const struct sockaddr *addr,
                   socklen_t addrlen){


	int (*orig_connect)(int, const struct sockaddr*, socklen_t);
	orig_connect = dlsym(RTLD_NEXT, "connect");
	if(orig_connect == NULL){
		fprintf(stderr, "Could not find address of the connect()\n");
		exit(EXIT_FAILURE);
	}

	struct timeval tm;
	memset(&tm, 0, sizeof(tm));
    	tm.tv_sec = 10;
    	tm.tv_usec = 0;

	int proxy_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(proxy_fd < 0){
		fprintf(stderr, "Error: %s\n", strerror(errno));
	}
	 if (setsockopt(proxy_fd, SOL_SOCKET, SO_RCVTIMEO, &tm,
                sizeof(tm)) < 0)
        	perror("setsockopt failed\n");
	 if (setsockopt(proxy_fd, SOL_SOCKET, SO_SNDTIMEO, &tm,
                sizeof(tm)) < 0)
        	perror("setsockopt failed\n");
	struct sockaddr_in sock;
	sock.sin_family = AF_INET;
	sock.sin_port = htons(PROXYPORT);
	sock.sin_addr.s_addr = inet_addr(PROXYHOST);
	if(orig_connect(proxy_fd, (struct sockaddr*) &sock, sizeof(sock)) < 0){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("[+] Connected to proxy\n");
	Request *req = make_socks4_req((struct sockaddr_in*) addr);
	if(write(proxy_fd, req, sizeof(Request)) < 0){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("[+] Socks4 requests was sent\n");
	char buf[sizeof(Response)];
	Response *resp;
	if(read(proxy_fd, buf, sizeof(Response)) < 0){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	resp = (Response*) buf;
	if(resp->cd == 90){
		printf("[+] Request granted\n");
	}else{
		fprintf(stderr, "Request is not granted. Code: %d\n", resp->cd);
		exit(EXIT_FAILURE);
	}

	if(dup2(proxy_fd, orig_fd) == -1){
		fprintf(stderr, "Error: %d\n", strerror(errno));
		exit(EXIT_FAILURE);
	} 
	free_socks4_rs(req);
	close(proxy_fd);

	return EXIT_SUCCESS;
}
