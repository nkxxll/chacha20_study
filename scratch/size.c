#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE(x) (sizeof(x) / sizeof(x[0]))

// thought that would not work only solution is a macro or the size as a
// parameter
size_t size(uint32_t array[16]) { return sizeof(array) / sizeof(array[0]); }

int main(int argc, char *argv[]) {
  uint32_t array[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  int size_int = size(array);
  int size_array = sizeof(array) / sizeof(array[0]);
  int macro_size = SIZE(array);
  printf("%d %d %d\n", size_int, size_array, macro_size);
  return EXIT_SUCCESS;
}
