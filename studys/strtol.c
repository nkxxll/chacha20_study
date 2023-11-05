#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  char buf[20];
  fgets(buf, 20, stdin);
  unsigned int input_int = strtoul(buf, NULL, 16);
  printf("%s %ud", buf, input_int);
  return EXIT_SUCCESS;
}
