#include "types.h"
#include <stdlib.h>
#include <libpq-fe.h>
#include <stdio.h>

int successful_admin_login(Admin *p, PGconn *conn)
{
    PGresult *result;

    char query[256];
    snprintf(query, sizeof(query), "SELECT COUNT(*) FROM admin WHERE user_name = '%s' AND password = '%s'", p->user_name, p->password);

    result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error executing query in admin table while logging in: %s", PQerrorMessage(conn));
        PQclear(result);
        exit(EXIT_FAILURE);
    }

    int rowCount = atoi(PQgetvalue(result, 0, 0));

    PQclear(result);

    // Return 1 if login is successful, 0 otherwise
    return ((rowCount > 0) ? 1 : 0);
}

int successful_doctor_login(Doctor *p, PGconn *conn)
{
    PGresult *result;

    char query[256];
    snprintf(query, sizeof(query), "SELECT COUNT(*) FROM doctor WHERE user_name = '%s' AND password = '%s'", p->user_name, p->password);

    result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error executing query in doctor table while logging in: %s", PQerrorMessage(conn));
        PQclear(result);
        exit(EXIT_FAILURE);
    }

    int rowCount = atoi(PQgetvalue(result, 0, 0));

    PQclear(result);

    // Return 1 if login is successful, 0 otherwise
    return ((rowCount > 0) ? 1 : 0);
}

int successful_patient_login(Patient *p, PGconn *conn)
{
    PGresult *result;

    char query[256];
    snprintf(query, sizeof(query), "SELECT COUNT(*) FROM patient WHERE user_name = '%s' AND password = '%s'", p->user_name, p->password);

    result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error executing query: %s", PQerrorMessage(conn));
        PQclear(result);
        exit(EXIT_FAILURE);
    }

    int rowCount = atoi(PQgetvalue(result, 0, 0));

    PQclear(result);

    // Return 1 if login is successful, 0 otherwise
    return ((rowCount > 0) ? 1 : 0);
}
