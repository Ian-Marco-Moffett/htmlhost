#include <server.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


static int sockfd = 0;


static void _on_exit(void) {
  // Close the socket.
  if (sockfd != 0) close(sockfd);
}


static void transmit_html(const char* html_file, int client_sock) {
  FILE* fp = fopen(html_file, "r");

  fseek(fp, 0, SEEK_END);
  size_t fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char http_resp[500];
  snprintf(http_resp, sizeof(http_resp),
      "HTTP/1.1 200 OK\r\n"
      "Content-Length: %d\r\n"
      "Content-Type: text/html\r\n", fsize);
  
  // Allocate memory and read file into buffer.
  char* buf = malloc(sizeof(char) * fsize);
  fread(buf, sizeof(char), fsize, fp);

  // Send the HTTP response.
  send(client_sock, http_resp, strlen(http_resp), 0);
  
  // Send and free the buffer.
  send(client_sock, buf, sizeof(char) * fsize, 0);
  free(buf);
}


void start_server(const char* html_file, uint16_t port) {
  atexit(_on_exit);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

  printf("Press CTRL-C to close server and quit.\n");
  while (1) {
    listen(sockfd, 1);

    struct sockaddr client_addr;
    int client_size = sizeof(client_addr);
    int client_sock = accept(sockfd, (struct sockaddr*)&client_addr, &client_size);
    transmit_html(html_file, client_sock);
    close(client_sock);
  }
}
