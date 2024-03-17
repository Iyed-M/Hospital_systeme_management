#ifndef DATABASEFUNCS_H
#define DARABASEFUNCS_H
#include "types.h"
#include <libpq-fe.h>

void printTable(PGconn *conn, const char *tableName, const char *items, const char *condition);
void create_database_tables(PGconn *conn, PGresult *result,const char* admin_username, const char* admin_password);
void exit_nicely(PGconn *conn);
void add_doctor(PGconn *conn, Doctor doctor);
void add_patient(PGconn *conn, Patient patient);
int getDoctorFromDB(PGconn *conn, int doctorId, Doctor *doctor);
int getPatientFromDB(PGconn *conn, int patientId, Patient *patient_ptr);
int update_doctor(PGconn *conn, Doctor doctor);
int update_patient(PGconn *conn, Patient patient);
int get_patient_id_from_db(PGconn *conn, Patient *p);
/// @brief u get the id  name and familyname from table1 that are referencing ur id as a table 2
/// @param table1 this is where u get id and name from
/// @param table2 Ur table
/// @param table1_2 connection between table 1 and table 2 (many to many)
/// @param myid anId in the table2
void get_id_and_name(PGconn *conn, const char *table1, const char *table2, const char *table1_2, int myid);
void print_pgresult(PGresult *res);
int get_doctor_id_from_db(PGconn *conn, Doctor *p);
#endif