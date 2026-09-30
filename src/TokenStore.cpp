#include "TokenStore.h"

#include <Arduino.h>
#include <EEPROM.h>

namespace {

constexpr uint32_t MAGIC = 0x544F4B32;
constexpr size_t OFFSET = 128;
constexpr size_t EEPROM_SIZE = 256;

struct Tokens {
  uint32_t magic;
  char gitlab[96];
};

Tokens tokens = {};

bool read(Tokens& out) {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(OFFSET, out);
  EEPROM.end();
  if (out.magic != MAGIC) return false;
  out.gitlab[sizeof(out.gitlab) - 1] = '\0';
  return true;
}

}

namespace TokenStore {

void save(const char* gitlab) {
  Tokens next = {};
  next.magic = MAGIC;
  strncpy(next.gitlab, gitlab, sizeof(next.gitlab) - 1);

  Tokens current;
  if (read(current) && memcmp(&current, &next, sizeof(Tokens)) == 0) return;

  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(OFFSET, next);
  EEPROM.commit();
  EEPROM.end();
}

void load() {
  if (!read(tokens)) tokens = {};
}

const char* gitlab() {
  return tokens.gitlab;
}

}
