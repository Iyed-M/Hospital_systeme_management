#ifndef ADMIN_ACCESS_H
#define ADMIN_ACCESS_H

#include <libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"types.h"
#include"databasefuncs.h"
#include"menus.h"
#include"admin_utility.h"
#include"logins.h"
#include"types.h"

void admin_access(PGconn *conn);

#endif

