#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

#define SIZE_MSG	370000
#define SIZE_BUFFER 400000
#define MAX_FD		2048

typedef struct s_client {
	int		id;
	char	msg[SIZE_MSG];
} t_client;

t_client	clients[MAX_FD];
int			max_fd = 0, next_id = 0, socket_server, i, len;
fd_set		read_set, write_set, current;
char		send_buffer[SIZE_BUFFER], recv_buffer[SIZE_BUFFER];
va_list		args;

void	error(char *msg) {
	write(2, msg ? msg : "Fatal Error", msg ? strlen(msg) : 11);
	write(2, "\n", 1);
	exit(1);
}

void	send_msg(int sender, char *format, ...){
	bzero(send_buffer, sizeof(send_buffer));
	va_start(args, format);
	vsprintf(send_buffer, format, args);
	va_end(args);
	for(int fd = 0; fd <= max_fd; fd++)
	{
		if (FD_ISSET(fd, &write_set) && fd != sender)
			if (send(fd, send_buffer, strlen(send_buffer), 0) == -1)
				error(NULL);
	}
}

int		main(int argc, char **argv) {
	if (argc != 2)
		error("Wrong numbre of arguments");
	
	struct sockaddr_in	server;
	server.sin_family = AF_INET;
	server.sin_port   = htons(atoi(argv[1]));
	server.sin_addr.s_addr = htonl(2130706433);

	socket_server = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_server == -1)
		error(NULL);
	
	FD_ZERO(&read_set);
	FD_ZERO(&write_set);
	FD_ZERO(&current);
	FD_SET(socket_server, &current);
	bzero(clients, sizeof(clients));
	max_fd = socket_server;

	if(bind(socket_server, (struct sockaddr *)&server, sizeof(server)) == -1 || listen(socket_server, 100) == -1)
		error(NULL);
	
	while (1){
		read_set = write_set = current;
		if (select(max_fd + 1, &read_set, &write_set, NULL, NULL) == -1)
			continue;
		for (int fd = 0; fd <= max_fd; fd++){
			if(FD_ISSET(fd, &read_set)){
				if (fd == socket_server){
					socklen_t	addr_len = sizeof(server);
					int			new_client;
					new_client = accept(socket_server, (struct sockaddr *)&server, &addr_len);
					if (new_client != -1) {
						if (new_client >= max_fd) max_fd = new_client;
						clients[new_client].id = next_id++;
						bzero(clients[new_client].msg, sizeof(clients[new_client].msg));
						FD_SET(new_client, &current);
						send_msg(new_client, "server: client %d just arraived\n", clients[new_client].id);
					}
				} else {
					int		num_bytes = recv(fd, recv_buffer, sizeof(recv_buffer), 0);
					if (num_bytes <= 0) {
						send_msg(fd, "server: client %d just left\n", clients[fd].id);
						FD_CLR(fd, &current);
						close(fd);
						bzero(recv_buffer, sizeof(recv_buffer));
						if (fd == max_fd)
							while(max_fd > 0 && !FD_ISSET(max_fd, &current)) max_fd--;
					} else {
						for (i = 0, len = strlen(clients[fd].msg); i < num_bytes && len < SIZE_MSG -1; i++, len++){
							if((clients[fd].msg[len] = recv_buffer[i]) == '\n'){
								clients[fd].msg[len] = '\0';
								send_msg(fd, "client %d: %s\n", clients[fd].id, clients[fd].msg);
								bzero(clients[fd].msg, sizeof(clients[fd].msg));
								len = - 1;
							}
						}
					}
				}
			}

		}

	}
}