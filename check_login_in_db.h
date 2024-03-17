
#ifndef CHECK_LOGIN_IN_DB_H
#define CHECK_LOGIN_IN_DB_H
#include"types.h"
#include <libpq-fe.h>

int successful_admin_login(Admin *admin_ptr, PGconn *conn);
int successful_doctor_login(Doctor *doctor_ptr, PGconn *conn);
int successful_patient_login(Patient *patient_ptr, PGconn *conn);


#endif