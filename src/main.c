#include <stdio.h>
#include <argp.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <server.h>

static const char* input_html_file = NULL;
static uint16_t portno = 0;


static struct argp_option options[] = {
  {"in", 'i', "INPUT", 0, "Input HTML file"},
  {"port", 'p', "PORT", 0, "Port number"},
  {0}
};


static error_t parse_opt(int key, char* arg, struct argp_state* state) {
  switch (key) {
    case 'i':
      input_html_file = arg;
      break;
    case 'p':
      portno = atoi(arg);
      if (portno == 0) {
        printf("Error: Invalid port number!\n");
        return ARGP_ERR_UNKNOWN;
      }

      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp argp = { options, parse_opt, 0, 0, 0, 0, 0 };


int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Error: Too few arguments! Please use --help\n");
    return 1;
  }

  argp_parse(&argp, argc, argv, 0, 0, 0);

  if (input_html_file == NULL) {
    printf("Error: Requires HTML filepath. Please use --help\n");
    return 1;
  }

  if (access(input_html_file, F_OK) != 0) {
    printf("HTML file \"%s\" does not exist!\n");
    return 1;
  }

  start_server(input_html_file, portno);

  return 0;
}
