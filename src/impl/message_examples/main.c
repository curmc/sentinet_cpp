/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : main
 * @created     : Friday Sep 20, 2019 16:37:58 MDT
 */

#define DEBUG
#include "scpp/data_message.h"

int
main()
{
  // Initialize some data
  uint64_t val = 1;
  uint32_t data = 5;
  char data2[7] = "hello ";

  // Requires destination to serialize into
  // this is optional if you just want
  // the raw pointer to the data, that works too
  uint32_t destination;
  char destination2[7];

  // Create a buffer - does some initialization like endianess etc
  data_buffer buff;
  create_buffer(&buff);

  // Serialize some data - see type_codes for some common types
  serialize_data(&buff, &data, sizeof(uint32_t), UINT32);
  serialize_data(&buff, &data2, 7, STRING);

  to_wire(&buff);
  uint8_t* recieved_data = buff.data;

  printf("======================================\n");
  printf("Heres the message in hex: \n");
  print_message_raw(recieved_data, get_byte_length(recieved_data));
  printf("======================================\n\n\n\n");

  printf("======================================");
  printf("\nHeres the message in a clean form\n");
  // Lets print the header / meta information
  print_message_formatted(recieved_data);
  printf("======================================\n\n");

  // Remember, we don't know any meta information other than the data string
  // itself
  char a = get_data(recieved_data, (char*)&destination, 0).type_code;
  // get data safe has a shit load of error checks, get_data doesn't
  char b = get_data(recieved_data, destination2, 1).type_code;

  // get_data returns the type code
  printf("First index: %d\n", destination);
  printf("Second index: %s\n", destination2);

  printf("\n\n\n======================================\n");
  printf(
    "Now I'll add the same string over and over, and change the message\n");
  printf("I'm just spamming hello\n");
  if (serialize_data(&buff, &data2, 7, STRING) == -1)
    printf("error\n");
  if (serialize_data(&buff, &data2, 7, STRING) == -1)
    printf("error\n");
  if (serialize_data(&buff, &data2, 7, STRING) == -1)
    printf("error\n");
  if (serialize_data(&buff, &data2, 7, STRING) == -1)
    printf("error\n");
  if (serialize_data(&buff, &data2, 7, STRING) == -1)
    printf("error\n");
  if (serialize_data(&buff, &data2, 7, STRING) == -1)
    printf("error\n");
  if (serialize_data(&buff, &data2, 7, STRING) == -1)
    printf("error\n");
  if (serialize_data(&buff, &data2, 7, STRING) == -1)
    printf("error\n");
  if (serialize_data(&buff, &data2, 7, STRING) == -1)
    printf("error\n");
  if (serialize_data(&buff, &data2, 7, STRING) == -1)
    printf("error\n");
  if (serialize_data(&buff, &data2, 7, STRING) == -1)
    printf("error\n");

  to_wire(&buff);
  recieved_data = buff.data;
  printf("======================================\n");
  printf("Heres the message in hex: \n");
  print_message_raw(recieved_data, get_byte_length(recieved_data));
  printf("======================================\n\n\n\n");

  printf("======================================");
  printf("\nHeres the message in a clean form\n");
  // Lets print the header / meta information
  print_message_formatted(recieved_data);
  printf("======================================\n\n\n\n");

  return 1;
}
