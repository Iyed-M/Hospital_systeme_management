#include "types.h"
#include "menus.h"
#include <libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "databasefuncs.h"
#include "admin_access.h"
#include "patient_access.h"
#include "doctor_access.h"
#define USER "postgres"
#define DATABSAE_NAME "postgres"
#define PASSWORD "postgres"
#define ADMIN_PASS "test"
#define ADMIN_USR "test"

int main()
{
    char conninfo[50];
    sprintf(conninfo, "dbname=%s user=%s password=%s", DATABSAE_NAME, USER, PASSWORD);
    PGconn *conn = PQconnectdb(conninfo);
    PGresult *result;

    // check for a successful connection
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        exit_nicely(conn);
    }
    // CREATES THE DATABASE TABLES IN CASE THEY DON'T EXIST
    create_database_tables(conn, result, ADMIN_USR, ADMIN_PASS);

    int option;
    char choice[10];
    do
    {
        main_menu();
        scanf("%d", &option);

        switch (option)
        {
        case 1: // CHOOSE ADMIN
            admin_access(conn);
            break;

        case 2: // CHOOSE DOCTOR
            doctor_access(conn);
            break;
        case 3: // CHOOSE PATIENT
            patient_access(conn);
            break;

        case 4:
            exit_nicely(conn);
        }

    } while (option < 1 || option > 4);
    return 1;
}
