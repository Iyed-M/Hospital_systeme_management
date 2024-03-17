#ifndef ADMIN_UTILITY_H
#define ADMIN_UTILITY_H
#include <libpq-fe.h>
#include "types.h"
void admin_manage_service(PGconn *conn);
void admin_manage_doctor(PGconn *conn);
void admin_manage_patient(PGconn *conn);
void admin_manage_public_holidays(PGconn *conn);
int admin_manage_appointments(PGconn *conn);
#endif
