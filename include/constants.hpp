#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>

// BYTE = 8 bits
typedef unsigned char BYTE;
typedef char SIGNED_BYTE;

// WORD = 16 bits
typedef unsigned short WORD;
typedef signed short SIGNED_WORD;

struct HexCharStruct
{
  BYTE c;
  HexCharStruct(BYTE _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
  return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(BYTE _c)
{
  return HexCharStruct(_c);
}

const int REFRESHRATE = 60; // gameboy refreshes screen 60 times per sec
const int CLOCKSPEED = 4194304; // gameboy can execute 4194304 clock cycles each second
const int MAXCYCLES = 69905; // max cpu cycles per frame

#endif