
#include <cstdio>
#include <cstring>

#include "Serializable.hpp"

unsigned char *Serializable::serialize_int(unsigned char *buf, const unsigned int value) const
{       
        buf[0] = value >> 24;
        buf[1] = value >> 16;
        buf[2] = value >> 8;
        buf[3] = value;
        return buf + 4;
}

unsigned char *Serializable::serialize_char(unsigned char *buf, const char value) const
{       
        buf[0] = value;
        return buf + 1;
}

unsigned char *Serializable::serialize_char_array(unsigned char *buf, const char *value, const int length) const
{       
        memcpy(buf, value, length);
        return buf + length;
}

int Serializable::deserialize_int(const unsigned char *buf) const
{
        int result = 0;
        result |= buf[0] << 24;
        result |= buf[1] << 16;
        result |= buf[2] << 8;
        result |= buf[3];
        return result;
}
