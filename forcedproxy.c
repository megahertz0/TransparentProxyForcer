#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>

void proxy(struct sockaddr *name) {
	if(name->sa_family == AF_INET) {
		char proxyName[16] ;
		strcpy(proxyName, "127.0.0.1");
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)name;
		if(ntohs(ipv4->sin_port) == 80) {
			ipv4->sin_addr.s_addr = inet_addr(proxyName);
			ipv4->sin_port = htons(8080);
		} else if(ntohs(ipv4->sin_port) == 443) {
			ipv4->sin_addr.s_addr = inet_addr(proxyName);
			ipv4->sin_port = htons(8443);
		}
	} else if(name->sa_family == AF_INET6) {
		char proxyName6[32] ;
		strcpy(proxyName6, "0:0:0:0:0:FFFF:");
		struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)name;
		if(ntohs(ipv6->sin6_port) == 80) {
			struct in6_addr addr;
			ipv6->sin6_addr = addr;
			inet_pton(AF_INET6, proxyName6, &ipv6->sin6_addr);
			ipv6->sin6_port = htons(8080);
		} else if(ntohs(ipv6->sin6_port) == 443) {
			inet_pton(AF_INET6, proxyName6, &ipv6->sin6_addr);
			ipv6->sin6_port = htons(8443);
		}
	}
}

int (*connect_ptr)(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int connect (int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	if (!connect_ptr) {
#ifndef __APPLE__
		void *libc;
		libc = dlopen("libc.so.6", RTLD_LAZY);
		if (!libc) {
			fprintf(stderr, "Unable to open libc!\n");
			exit(-1);
		}
		*(void **) (&connect_ptr) = dlsym(libc, "connect");
#else
		*(void **) (&connect_ptr) = dlsym(RTLD_NEXT, "connect");
#endif
		if (!connect_ptr) {
			fprintf(stderr, "Unable to locate connect function in lib\n");
			exit(-1);
		}
#ifndef __APPLE__
		dlclose(libc);
#endif
	}
	proxy((struct sockaddr *)addr);
	return (int)(*connect_ptr)(sockfd, addr, addrlen);
}
