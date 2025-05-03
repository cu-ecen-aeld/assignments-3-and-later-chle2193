#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    /* Opens a stream socket bound to port 9000, failing and returning -1 if any of the socket connection steps fail. 
    On success, a file descriptor for the new socket is returned.  On
       error, -1 is returned, and errno is set to indicate the error.
*/

    openlog("aesdsocket", LOG_PID | LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "Starting socket server assignment 5-1");

    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if (fd == -1) return -1;

    /*Opens a stream socket bound to port 9000, failing and returning -1 if any of the socket connection steps fail.
      int bind(int sockfd, const struct sockaddr *addr,                socklen_t addrlen);
      On success, zero is returned.  On error, -1 is returned, and errno
       is set to indicate the error.
      */

    /* int getaddrinfo(const char *restrict node,
                       const char *restrict service,
                       const struct addrinfo *restrict hints,
                       struct addrinfo **restrict res);*/

    
    struct addrinfo hints; /*https://man7.org/linux/man-pages/man3/getaddrinfo.3.html*/
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int addrInfoRet = getaddrinfo(NULL, "9000", &hints, &result);
    if (addrInfoRet != 0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(addrInfoRet));
        exit(EXIT_FAILURE);
    }

    if (bind(fd, rp->ai_addr, rp->ai_addrlen) != 0)
    {
        close(fd);
        return -1;
    }   
    freeaddrinfo(result);   
    
    listen(fd, 1); /*   int listen(int sockfd, int backlog); On success, zero is returned.  On error, -1 is returned, */
    syslog(LOG_INFO, "Listening on port 9000");

    /*      int accept(int sockfd, struct sockaddr *_Nullable restrict addr,    socklen_t *_Nullable restrict addrlen);*/
    /*On success, these system calls return a file descriptor for the
       accepted socket (a nonnegative integer).  On error, -1 */
    int acceptFd;
    struct sockaddr_storage client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    acceptFd = accept(fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if (acceptFd == -1) 
    {
        return -1;
    }

    char clientIP[INET_ADDRSTRLEN];
    struct sockaddr_in *s = (struct sockaddr_in *)&client_addr;
    inet_ntop(AF_INET, &s->sin_addr, clientIP, sizeof(clientIP));
    syslog(LOG_INFO, "Accepted connection from %s", clientIP);


}