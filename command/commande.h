#include "../file/dir.h"
#include "../file/file.h"
#include "../file/ifile.h"
#include <unistd.h>

#define MAX_ENTRY_LENGTH 255
#define MAX_COMMAND_LENGTH 10
#define MAX_OPTION_LENGTH 255

void ls(const char** arg);

void cat(const char** arg);

void cd(const char** arg);

