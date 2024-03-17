#ifndef PATIENT_ACCESS_H
#define PATIENT_ACCESS_H
#include <libpq-fe.h>

int patient_access(PGconn *conn);

#endif