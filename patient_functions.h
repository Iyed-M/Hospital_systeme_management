#ifndef PATIENT_FUNCTIONS_H
#define PATIENT_FUNCTIONS_H
#include "types.h"
#include <libpq-fe.h>
int patient_request_certificate(PGconn *conn, Patient *patient_ptr);
int patient_reuquest_appointment(PGconn *conn, Patient *patient_ptr);
int patient_view_description(PGconn *conn, Patient *patient_ptr);
int patient_view_certificate(PGconn *conn, Patient *patient_ptr);
int patient_view_ordonnance(PGconn *conn, Patient *patient_ptr);
int patient_view_upcoming_appointment(PGconn *conn, Patient *patient_ptr); // FOR THE NEXT 7 DAYS
int patient_get_doctor(PGconn *conn,Patient *patient_ptr);
int patient_view_upcoming_appointment(PGconn *conn, Patient *p) ;

#endif