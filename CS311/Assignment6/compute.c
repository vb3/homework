#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<sys/time.h>	/* timeval{} for select() */
#include	<time.h>		/* timespec{} for pselect() */
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */
#include	<errno.h>
#include	<fcntl.h>		/* for nonblocking */
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include    <strings.h>     /* for bzero */
#include	<sys/stat.h>	/* for S_xxx file mode constants */
#include	<sys/uio.h>		/* for iovec{} and readv/writev */
#include	<unistd.h>
#include	<sys/wait.h>

#define	LISTENQ		1	/* 2nd argument to listen() */

/* Miscellaneous constants */
#define	MAXLINE		4096	/* max text line length */
#define	MAXSOCKADDR  128	/* max socket address structure size */
#define	BUFFSIZE	8192	/* buffer size for reads and writes */

/* Define some port number that can be used for client-servers */
#define	SERV_PORT		 4659			/* TCP and UDP client-servers */
#define	SERV_PORT_STR	"4659"			/* TCP and UDP client-servers */

/* Functions */
int perfect_num(int n);
void check_perfect_num(int rangelow, int rangehigh);

/* Globals */
int sockfd;

int main(int argc, char **argv)
{
    struct sockaddr_in servaddr;
    
    char sendline[MAXLINE];
    char recvline[MAXLINE];
    double time_spent=0;
    clock_t begin, end;
    char temp[12] = {};
    int result, count=0, mod_max, low, high;
    char *rangetemp;
    
    if (argc != 2){
		perror("usage: compute <IPaddress>");
		exit(-1);
	}
    
    /* Setup socket and connect to server */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (read(sockfd, recvline, MAXLINE) == 0){
        perror("Connecting error");
        exit(-1);
    }
    printf("%s\n", recvline);
    strcpy(recvline, "");
    
    /* Timing Loop */
    while(time_spent < 3){
        result = count % 7;
        count++;
        end = clock();
        time_spent = (double)(end-begin) / CLOCKS_PER_SEC;
    }
    mod_max = count;
    
    while (1) {
        
        /* Send server mod max */
        sprintf(temp, "!%d", mod_max);
        write(sockfd, temp, strlen(temp));

        /* Get range */
        if (read(sockfd, recvline, MAXLINE) == 0){
            perror("Server didn't send a range. Exiting.");
            exit(-1);
        }
        
        /* Parse Range */
        rangetemp = strtok(recvline, ",");
        low = atoi(rangetemp);
        rangetemp = strtok(NULL, ",");
        high = atoi(rangetemp);
        printf("Range recieved: %d to %d\n", low, high);
        strcpy(recvline, "");
        
        check_perfect_num(low,high);
    }
    
}

void check_perfect_num(int rangelow, int rangehigh)
{
    int i;
    int perf_num;
    int write_size;
    char str[512];
    int str_len=0;
    
    for(i = rangelow; i <= rangehigh; i++) {
        if (perfect_num(i)) {
            printf("found: %d\n", i);
			write_size = sprintf(&str[str_len], "%d,", i);
			str_len += write_size;
        }
    }
    
    write_size = write(sockfd, str, str_len);
}

int perfect_num(int n)
{
    int sum = 1, i;
    
    if (n % 2 == 0) {
        for (i = 2; i < n; i++)
            if (n % i == 0)
                sum += i;
        if (n == sum)
            return 1;
    }
    
    return 0;
}