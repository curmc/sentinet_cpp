/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : main
 * @created     : Friday Sep 20, 2019 16:37:58 MDT
 */

#define DEBUG
#include "scpp/data_message.h"
#include <time.h>

int
main()
{
  // Create a buffer - does some initialization like endianess etc
  data_buffer buff;

  clock_t begin = clock();

  for (int j = 0; j < 1000000; ++j) {
    create_buffer(&buff);
    serialize_data(&buff, &data2, 6, STRING);
  }

  clock_t end = clock();

  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("Clocks: %.6f\n", time_spent);
  return 1;
}
