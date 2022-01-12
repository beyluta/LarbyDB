#pragma once
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "main.h"
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <algorithm>
#include "file.h"

struct passwd *pw = getpwuid(getuid());
const char *homedir = pw->pw_dir;