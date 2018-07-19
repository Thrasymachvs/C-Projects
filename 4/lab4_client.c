#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	char buffer[256];
	int sockfd, portno, n;

	struct sockaddr_in serv_addr;
	struct hostent *server;

	if (argc < 3) {
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
		error("ERROR opening socket");
	server = gethostbyname(argv[1]);

	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}

	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR: Unable to connect.");

	////////////////////////////////////////////////////
	// To test: 
	// 1. A message greater than 255 

	while (1)
	{
		int message_len; 
		printf("%s ", "> ");
		fgets(buffer, 255, stdin);

		//Sending the length of the message to the server so that it knows how many bytes to read 
		message_len = strlen(buffer);
		if (message_len > 255) { 
			error("ERROR: message is too long");
			return 1;
		}
		n = write(sockfd, (char*)&message_len, sizeof(message_len));
		if (n < 0)error("ERROR: unable to write to socket.");
		
		//-------------------------------------------------------------------------------------//


		//This is where I WANTED to store the message length at the beginning of the buffer 
		/*char m_len[] = {strlen(buffer)};
		memcpy(buffer + 1 * sizeof(char), buffer, strlen(buffer) * sizeof(char));
		memcpy(buffer, m_len, sizeof(m_len));*/
		//------------------------------------------------------------------------------------//

		n = write(sockfd, buffer, strlen(buffer));
		if (n < 0)
			error("ERROR: unable to write to socket.");

		bzero(buffer, 256); //Clearing the buffer 
		n = read(sockfd, buffer, 255); //Reading the name from the server 
		if (strcmp("+++", buffer) == 0){ 
			close(sockfd);
			exit(0); 
		}
		else {
			printf("%s\n", buffer);
		}
	}
	return 0;
}