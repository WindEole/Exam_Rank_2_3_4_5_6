#include <stdio.h> // NULL / sprintf
#include <stdlib.h> // NULL / exit / atoi
#include <unistd.h> // close / write / exit
#include <string.h> // strlen / bzero
#include <sys/socket.h> // socket / send / listen / bind / accept
#include <sys/select.h> // select / fd_set type / FD_ISSET / FD_SET / FD_CLR / FD_ZERO
#include <netinet/in.h> // sockaddr_in / sockaddr / htonl / htons

typedef struct	s_client
{
	int			id;
	char		msg[100000];
}				t_client;

t_client		all_clients[1024];

int				max_fd = 0, next_id = 0;
fd_set			sock_list, cpyRead, cpyWrite;
char			bufRead[110000], bufWrite[110000];

void	fatal()
{
	write(2, "Fatal error\n", strlen("Fatal error\n"));
	exit(1);
}

void	send_all(int fd) // WRITE partout partout !
{
	for (int i = 0; i <= max_fd; i++)
		if (FD_ISSET(i, &cpyWrite) && i != fd) // after calling select(), FD_ISSET tests if a fd is still present in a set of fd.
			send(i, bufWrite, strlen(bufWrite), 0);
}

int		main(int ac, char **av)
{
	//1) Traitement des arguments
	if (ac != 2)
	{
		write(2, "Wrong number of arguments\n", strlen("Wrong number of arguments\n"));
		exit(1);
	}
	int port = atoi(av[1]);

	bzero(&all_clients, sizeof(all_clients));
	FD_ZERO(&sock_list); // FD_ZERO removes all fd from the list of fd socket. First step of initializing a fd list !

	//2) SOCKET create socket server !
	int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		fatal();
	
	max_fd = serv_sock; // serv_sock sera tjs > 2, on set le max_fd a serv_sock
	FD_SET(serv_sock, &sock_list); // FD_SET adds serv_sock to the list of fd sock_list

	//3) SOCKADDR_IN assign IP and port -> main !
	struct sockaddr_in	serv_addr;
	socklen_t			addr_len = sizeof(serv_addr);
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	serv_addr.sin_port = htons(port);

	//4) BIND and LISTEN
	if ((bind(serv_sock, (const struct sockaddr *)&serv_addr, addr_len)) < 0)
		fatal();
	if (listen(serv_sock, 128) < 0)
		fatal();

	//5) la grande boucle WHILE(1)
	while(1)
	{
		cpyRead = cpyWrite = sock_list;
		//5.a) SELECT : allows a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O operation
		if (select(max_fd + 1, &cpyRead, &cpyWrite, NULL, NULL) < 0)
			continue;

		//5.b) la grande boucle FOR sur max_fd
		for (int s = 0; s <= max_fd; s++)
		{
			//5.b.1) ACCEPT : Add new user to all_clients
			if (FD_ISSET(s, &cpyRead) && s == serv_sock)
			{
				int cli_sock = accept(serv_sock, (struct sockaddr *)&serv_addr, &addr_len);
				if (cli_sock < 0)
					continue;
				max_fd = (cli_sock > max_fd) ? cli_sock : max_fd;
				all_clients[cli_sock].id = next_id++;
				FD_SET(cli_sock, &sock_list);
				sprintf(bufWrite, "server: client %d just arrived\n", all_clients[cli_sock].id);
				send_all(cli_sock);
				break;
			}

			//5.b.2) RECV : recoit un message du Client
			if (FD_ISSET(s, &cpyRead) && s != serv_sock)
			{
				int res = recv(s, bufRead, 110000, 0);
				//5.b.2.a) le client quitte le serveur !
				if (res <= 0)
				{
					sprintf(bufWrite, "server: client %d just left\n", all_clients[s].id);
					send_all(s);
					FD_CLR(s, &sock_list);
					close(s);
					break;
				}
				//5.b.2.b) le client envoie un message normal
				else
				{
					for (int i = 0, j = strlen(all_clients[s].msg); i < res; i++, j++)
					{
						all_clients[s].msg[j] = bufRead[i];
						if (all_clients[s].msg[j] == '\n')
						{
							all_clients[s].msg[j] = '\0';
							sprintf(bufWrite, "client %d; %s\n", all_clients[s].id, all_clients[s].msg);
							send_all(s);
							bzero(&all_clients[s].msg, strlen(all_clients[s].msg));
							j = -1; 
						}
					}
					break;
				}
			}
		}
	}
}