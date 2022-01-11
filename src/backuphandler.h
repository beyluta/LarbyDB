#pragma once
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "main.h"
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>

struct passwd *pw = getpwuid(getuid());
const char *homedir = pw->pw_dir;