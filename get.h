
#ifndef GET_H
#define GET_H
#include "types.h"
#include <libpq-fe.h>

void get_doctor(PGconn *conn, Doctor *pD);
void get_patient(Patient *pp);
int modify_doctor(PGconn *conn, Doctor *pd);
int modify_patient(Patient *pP);
#endif