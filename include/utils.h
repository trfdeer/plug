#ifndef Utils_h
#define Utils_h

#include <Arduino.h>

#include <cctype>

#define RELAY1 D4
#define RELAY2 D2
#define RELAY3 D1

inline uint8_t id_to_pin(int id) {
  if (id == 1)
    return RELAY1;
  else if (id == 2)
    return RELAY2;
  else if (id == 3)
    return RELAY3;
  else
    return -1;
}

#endif