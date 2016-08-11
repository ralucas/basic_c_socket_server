#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <sys/errno.h>


#define SOCKET_ERROR  -1

#define MAX_PENDING_CONNECTIONS   10
#define LOCALHOST                 "127.0.0.1"
#define PORT                      8080
#define BUFSIZE                   1024 // Using 1KB messages here

void serve() {
  int server_socket_fd;
  if ((server_socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) <= SOCKET_ERROR) {
    perror("Error creating the server socket");
  }
  fprintf(stdout, "Server (%i) socket created\n", server_socket_fd);

  int socket_opts;
  int sockopt = 1;
  if ((socket_opts = setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(int))) <= SOCKET_ERROR) {
    perror("Error setting the socket options");
  }
  fprintf(stdout, "Socket options (%i) set\n", socket_opts);

  // Create ip address
  in_addr_t ip_address;
  ip_address = inet_addr(LOCALHOST);

  struct sockaddr_in socket_address;
  // Allocate some memory for the socket_address
  memset(&socket_address, 0, sizeof(socket_address));
  socket_address.sin_family = AF_INET;
  socket_address.sin_addr.s_addr = ip_address;
  socket_address.sin_port = htons(PORT);

  int server;
  if ((server = bind(server_socket_fd, (struct sockaddr *) &socket_address, sizeof(socket_address))) <= SOCKET_ERROR) {
    perror("Error binding the server");
  }
  fprintf(stdout, "Server (%i) bound\n", server);

  int connection_listener;
  if ((connection_listener = listen(server_socket_fd, MAX_PENDING_CONNECTIONS)) <= SOCKET_ERROR) {
    perror("Error connecting the listener");
  }
  fprintf(stdout, "Server listening to connection (%i)\n", connection_listener);

  for (;;) {
    int client;
    struct sockaddr_in client_address;
    socklen_t client_len;
    char buffer[BUFSIZE];

    client_len = sizeof(client_address);

    // Accept incoming connections from the client
    if ((client = accept(server_socket_fd, (struct sockaddr *) &client_address, &client_len)) < SOCKET_ERROR) {
      perror("Error accepting on the server socket");
    }

    // Receive the message
    int receiver;
    if ((receiver = recv(client, buffer, BUFSIZE
    -1, 0)) < SOCKET_ERROR ) {
      perror("Error receiving message from the client");
    }
    fprintf(stdout, "Server recieved message (%i)\n", receiver);

    // Respond
    ssize_t sender;
    char response[BUFSIZE];
    size_t len;

    strcpy(response, "This is a response string!");
    len = strlen(response);

    if ((sender = send(client, response, len, 0)) < SOCKET_ERROR) {
      perror("Error sending the response");
    }
    fprintf(stdout, "Successfully sent message (%zu)", sender);
  }
}

int main(int argc, char **argv) {
  serve();
}
