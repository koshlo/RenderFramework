#ifndef __FILE_H__
#define __FILE_H__

#include <cstdio>
#include "String.h"

class File
{
public:
    File(const char* fileName);
    ~File();

    bool isValid() const { return _file != NULL; }
    bool readStr(String* dest);
private:
    FILE* _file;
};

#endif // __FILE_H__