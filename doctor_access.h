#ifndef DOCTOR_ACCESS_H
#define DOCTOR_ACCESS_H
#include "types.h"
#include <libpq-fe.h>
#include <stdio.h>
#include "databasefuncs.h"

#include<libpq-fe.h>
int doctor_access(PGconn *conn);


#endif 