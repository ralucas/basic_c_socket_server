Building a socket server in C is a great introduction to the language as well as IPC mechanisms such as sockets and it can show you how simple it is to set up a server.  For this installment, we're going to do a single-threaded version.

So, let's get started...

### The Server

First I want to define some constants that we'll use throughout. For some of these you may want to pass these in dynamically, but for this case I'm just hardcoding them here as constants:

```
#define SOCKET_ERROR  -1

#define MAX_PENDING_CONNECTIONS   10
#define LOCALHOST   "127.0.0.1"
#define PORT        8080
#define BUFSIZE     1024
```

We'll put the server in a void function that we'll call serve: `void serve()`, so
```
void serve() {
  // Put all subsequent code here
```

Create the server socket file descriptor, usually created with with `PF_INET`, `SOCK_STREAM`, and with tcp `IPPROTO_TCP`.

```
int server_socket_fd;
if ( (server_socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) <= SOCKET_ERROR ) {
  perror("Error creating the server socket");
}
```

Set the socket options, passing in the newly created socket file descriptor.  Additionally,

```
int sockopt;
int socket_opts;
if ( (socket_opts = setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(int))) <= SOCKET_ERROR ) {
  perror("Error setting the socket options");
}
```

Now we need to set up the socket address:

```
// Create ip address
in_addr_t ip_address;
ip_address = inet_addr();

struct sockaddr_in socket_address;
// Allocate some memory for the socket_address
memset(&socket_address, 0, sizeof(socket_address));
socket_address.sin_family = AF_INET;
socket_address.sin_addr.s_addr = ip_address;
socket_address.sin_port = htons(PORT);
```

Bind the server

```
int server;
if ( (server = bind(server_socket_fd, (struct sockaddr *) &socket_address, sizeof(socket_address))) <= SOCKET_ERROR ) {
  perror("Error binding the server");
}
```

Listen for incoming connections

```
int connection_listener;
if ( (connection_listener = listen(server_socket_fd, MAX_PENDING_CONNECTIONS)) <= SOCKET_ERROR ) {
  perror("Error connecting the listener");
}
```

Now let's run the server by running an infinite loop

```
for (;;) {
  int client;
  struct sockaddr_in client_address;
  socklen_t client_len;

  client_len = sizeof(client_address);

  // Accept incoming connections from the client
  if ( (client = accept(server_socket_fd, (struct sockaddr *) &client_address, &client_len)) <= SOCKET_ERROR ) {
    perror("Error accepting on the server socket");
  }

  // Receive the message
  int receiver;
  if ( (receiver = recv(client, buffer, BUFSIZE - 1, 0)) <= SOCKET_ERROR ) {
    perror("Error receiving message from the client);
  }

  // Respond
  ssize_t sender;
  char response[BUFSIZE];
  size_t len;

  strcpy(response, "This is a response string!");
  len = strlen(response);

  if ( (sender = send(client, response, len, 0)) <= SOCKET_ERROR ) {
    perror("Error sending the response");
  }
  fprintf(stdout, "Successfully sent (%i) message", sender);
}
```

Go ahead, build it, and fire it up and send a curl request.  In a future post, I'll build an HTTP server.

The source code for this can be found here: <https://github.com/ralucas/basic_c_socket_server>

I also suggest taking a look at [Beej's Guide to Networking](http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html)

