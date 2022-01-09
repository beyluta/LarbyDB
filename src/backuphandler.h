#pragma once
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "main.h"

struct passwd *pw = getpwuid(getuid());
const char *homedir = pw->pw_dir;