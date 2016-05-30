#include "File.h"
#include "../Platform.h"

File::File(const char* fileName)
{
    _file = fopen(fileName, "rb");
    if (_file == NULL) {
        ErrorMsg(String("Couldn't load \"") + fileName + "\"");
    }
}

File::~File()
{
    if (_file != NULL) {
        fclose(_file);
    }
}

bool File::readStr(String* dest)
{
    if (_file == NULL) {
        return false;
    }
    fseek(_file, 0, SEEK_END);
    int length = ftell(_file);
    fseek(_file, 0, SEEK_SET);

    dest->setLength(length);
    fread(dest->dataPtr(), length, 1, _file);
    return true;
}

