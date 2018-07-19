/* A simple server in the internet domain using TCP
The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

const char *database[] = { "harris.ics.uci.edu\n", "Ian G. Harris", "joe@cnn.com", "Joe Smith", "jane@slashdot.org", "Jane Smith" };
char * temp[256];

void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno, clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}


	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
		error("ERROR opening socket");

	bzero((char *)&serv_addr, sizeof(serv_addr));

	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0)
		error("ERROR on binding");

	
	/////////////////////////////////////////////////////////////////

	//bzero(buffer, 256);
	/*n = read(newsockfd, buffer, 255);
	if (n < 0)
	{
		error("ERROR reading from socket");
	}*/

	int new_connec_flag;
	int read_size;
	while (1) {
		listen(sockfd, 5);

		clilen = sizeof(cli_addr);

		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		printf("%s\n", "Address started");
		new_connec_flag = 1;
		if (newsockfd < 0)
			error("ERROR on accept");
		while (new_connec_flag = 1) {

			int buflen;
			n = read(newsockfd, (char*)&buflen, sizeof(buflen));
			if (n < 0) error("ERROR reading from socket");
			//printf("test %d\n", buflen);

			//bzero(buffer, 256);
			/*int buflen;
			n = read(newsockfd, (char*)&buflen, sizeof(buflen));
			if (n < 0) error("ERROR reading from socket");
			buflen = ntohl(buflen);

			n = read(newsockfd, buffer, buflen);
			if (n < 0) error("ERROR reading from socket");
			//else printf("%s\n",buffer);
			*/
			bzero(buffer, 256);
			n = read(newsockfd, buffer, buflen);
			if (n < 0) error("ERROR reading from socket");
			if (strcmp("+++\n", buffer) != 0) {
				printf("%s", buffer); //This is where the server prints the email address 
			}
			if (strcmp("harris@ics.uci.edu\n", buffer) == 0) {
				n = write(newsockfd, "Ian G. Harris", 13);
				if (n < 0) error("ERROR writing to socket");
			}
			else if (strcmp("joe@cnn.com\n", buffer) == 0) {
				n = write(newsockfd, "Joe Smith", 9);
				if (n < 0) error("ERROR writing to socket");
			}
			else if (strcmp("jane@slashdot.org\n", buffer) == 0) {
				n = write(newsockfd, "Jane Smith", 10);
				if (n < 0) error("ERROR writing to socket");
			}
			else if (strcmp("+++\n", buffer) == 0) {
				n = write(newsockfd, "+++", 3);
				if (n < 0) error("ERROR writing to socket");
				close(newsockfd);
				new_connec_flag = 0;
				break;
			}
			else {
				n = write(newsockfd, "Input not recognized", 20);
				if (n < 0) error("ERROR writing to socket");
			}

			//printf("%s\n", buffer);
			//memset(buffer, '\0', 256);


			/*if (strcmp("harris@ics.uci.edu\n", buffer) == 0) {
				printf("%s\n","in the correct loop");
				memset(buffer, '\0', 256);
				n = write(newsockfd, "success", 7);
			}*/
			//memset(buffer, '\0', 256);
		}
	}
	//n = write(newsockfd, "I got your message", 18);
	//if (n < 0) error("ERROR writing to socket");
	
	return 0;
}