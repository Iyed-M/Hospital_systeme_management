#include <stdlib.h>
#include <libpq-fe.h>
#include <stdio.h>
#include <unistd.h>
#include "types.h"
#include "stdbool.h"
void exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}

void print_pgresult(PGresult *res)
{
    if (res == NULL)
    {
        fprintf(stderr, "Result is NULL.\n");
        return;
    }

    int num_fields = PQnfields(res);
    int num_rows = PQntuples(res);

    if (num_rows == 0)
    {
        printf("Empty result set.\n");
        return;
    }

    // Print column headers
    for (int i = 0; i < num_fields; i++)
    {
        printf("%-15s|", PQfname(res, i));
    }
    printf("\n");

    // Print separator line
    for (int i = 0; i < num_fields; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            printf("-");
        }
    }
    printf("\n");

    // Print data
    for (int row = 0; row < num_rows; row++)
    {
        for (int col = 0; col < num_fields; col++)
        {
            printf("%-15s|", PQgetvalue(res, row, col));
        }
        printf("\n");
    }
}
/// @brief u get the id  name and familyname from table1 that are referencing ur id as a table 2
/// @param table1 this is where u get id and name from
/// @param table2 Ur table
/// @param table1_2 connection between table 1 and table 2 (many to many)
/// @param myid anId in the table2
void get_id_and_name(PGconn *conn, const char *table1, const char *table2, const char *table1_2, int myid)
{
    if (conn == NULL)
    {
        fprintf(stderr, "Connection not established.\n");
        return;
    }
    char query[256];
    if (table1 == "doctor" || table1 == "patient")
    {
        snprintf(query, sizeof(query),
                 "SELECT %s.id, %s.name , %s.family_name FROM %s WHERE %s.id IN (SELECT %s_id FROM %s WHERE %s_id = %d)",
                 table1, table1, table1, table1, table1, table1, table1_2, table2, myid);
    }
    // Construct the SQL query based on the provided input
    else
    {
        snprintf(query, sizeof(query),
                 "SELECT * FROM %s WHERE %s.id IN (SELECT %s_id FROM %s WHERE %s_id = %d)", table1, table1, table1, table1_2, table2, myid);
    }

    // Execute the query
    PGresult *res = PQexec(conn, query);

    // Check for errors in the query execution
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Query execution failed: %s", PQresultErrorMessage(res));
        PQclear(res);
        return;
    }
    print_pgresult(res);
    printf("\n");
}

void create_database_tables(PGconn *conn, PGresult *result, const char *admin_pass, const char *admin_usr)
{ // Create the admin table
    char query[256];
    result = PQexec(conn, "CREATE TABLE IF NOT EXISTS  admin (user_name  VARCHAR UNIQUE, password VARCHAR)");
    if (PQresultStatus(result) == PGRES_COMMAND_OK)
    {
        printf("Table admin successfully.\n");
        snprintf(query, sizeof(query), "INSERT INTO admin (user_name, password) VALUES ('%s', '%s')", admin_usr, admin_pass);
        result = PQexec(conn, query);
    }
    else
    {
        fprintf(stderr, "Error creating admin table: %s", PQresultErrorMessage(result));
    }

    PQclear(result);
    // holiday table
    result = PQexec(conn, "CREATE TABLE IF NOT EXISTS public_holiday (day DATE)");
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error creating holidays table: %s", PQerrorMessage(conn));
        PQclear(result);
        exit_nicely(conn);
    }
    PQclear(result);

    // Create the service table
    result = PQexec(conn, "CREATE TABLE IF NOT EXISTS service (id SERIAL PRIMARY KEY, name VARCHAR UNIQUE)");
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error creating Services table: %s", PQerrorMessage(conn));
        PQclear(result);
        exit_nicely(conn);
    }
    PQclear(result);

    // Create the patient table
    result = PQexec(conn, "CREATE TABLE IF NOT EXISTS patient (id SERIAL PRIMARY KEY, user_name  VARCHAR UNIQUE,password VARCHAR ,name VARCHAR,family_name VARCHAR ,date_of_birth DATE,sex BOOLEAN, descritption TEXT)");
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error creating Patient table: %s", PQerrorMessage(conn));
        PQclear(result);
        exit_nicely(conn);
    }
    PQclear(result);

    // create the doctors table

    result = PQexec(conn, "CREATE TABLE IF NOT EXISTS doctor (id SERIAL PRIMARY KEY, user_name  VARCHAR UNIQUE, password VARCHAR, name VARCHAR, family_name VARCHAR, date_of_birth DATE,sex BOOLEAN, serv_id INT REFERENCES service(id) ON DELETE CASCADE)");
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error creating doctors table: %s", PQerrorMessage(conn));
        PQclear(result);
        exit_nicely(conn);
    }
    PQclear(result);

    // Create the APPOOINTMENT(rendez-vous) table
    result = PQexec(conn, "CREATE TABLE IF NOT EXISTS appointment (id SERIAL PRIMARY KEY, patient_id INT REFERENCES patient(id) ON DELETE CASCADE , doctor_id INT REFERENCES doctor(id) ON DELETE CASCADE, validity BOOLEAN , requested_by_patient BOOLEAN , date DATE , time TIME)");
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error creating appointment table: %s", PQerrorMessage(conn));
        PQclear(result);
        exit_nicely(conn);
    }
    PQclear(result);

    // create the RECEIVE_SERVICE table
    result = PQexec(conn, "CREATE TABLE IF NOT EXISTS receive_service (id SERIAL PRIMARY KEY, service_id INT REFERENCES service(id) ON DELETE CASCADE, patient_id INT REFERENCES patient(id) ON DELETE CASCADE  )");
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error creating receive_service table: %s", PQerrorMessage(conn));
        PQclear(result);
        exit_nicely(conn);
    }
    PQclear(result);

    // create doctor_patient table
    result = PQexec(conn, "CREATE TABLE IF NOT EXISTS doctor_patient (id SERIAL PRIMARY KEY, patient_id INT REFERENCES patient(id) ON DELETE CASCADE, doctor_id INT REFERENCES doctor(id) ON DELETE CASCADE )");
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error creating doctor_patient table: %s", PQerrorMessage(conn));
        PQclear(result);
        exit_nicely(conn);
    }
    PQclear(result);

    // create ordonnance table
    result = PQexec(conn, "CREATE TABLE IF NOT EXISTS ordonnance (id SERIAL PRIMARY KEY, patient_id INT REFERENCES patient(id) ON DELETE CASCADE, doctor_id INT REFERENCES doctor(id) ON DELETE CASCADE,content TEXT )");
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error creating ordonnance table: %s", PQerrorMessage(conn));
        PQclear(result);
        exit_nicely(conn);
    }
    PQclear(result);

    // create certificat table
    result = PQexec(conn, "CREATE TABLE IF NOT EXISTS certificat (id SERIAL PRIMARY KEY, patient_id INT REFERENCES patient(id) ON DELETE CASCADE, doctor_id INT REFERENCES doctor(id) ON DELETE SET NULL, created_at DATE, duration INT , cause VARCHAR , requested_by_patient BOOLEAN,approved BOOLEAN )");
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error creating certificat table: %s", PQerrorMessage(conn));
        PQclear(result);
        exit_nicely(conn);
    }
    PQclear(result);

    /* // ____DISABLED____ create does_service table (tableau intermediaire entre doctor et service)
    result = PQexec(conn, "CREATE TABLE IF NOT EXISTS does_service (id SERIAL PRIMARY KEY, service_id INT REFERENCES service(id) ON DELETE CASCADE, doctor_id INT REFERENCES doctor(id) ON DELETE CASCADE )");
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error creating does_service table: %s", PQerrorMessage(conn));
        PQclear(result);
        exit_nicely(conn);
    }
    PQclear(result);
    */
}

/// @brief printTable(conn, "patient", "name, sex", " id=1")==>SELECT items FROM tableName WHERE condition
/// @param conn
/// @param tableName
/// @param items
void printTable(PGconn *conn, const char *tableName, const char *items, const char *condition)
{

    PGresult *res;

    char query[1024];
    if (condition[0] == '\0')
    {
        snprintf(query, sizeof(query), "SELECT %s FROM %s", items, tableName);
    }
    else
    {
        snprintf(query, sizeof(query), "SELECT %s FROM %s WHERE %s", items, tableName, condition);
    }

    res = PQexec(conn, query);
    print_pgresult(res);
    /*old_print
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            fprintf(stderr, "Query execution failed: %s", PQerrorMessage(conn));
            PQclear(res);
            return;
        }

        int numFields = PQnfields(res);
        for (int i = 0; i < numFields; i++)
        {
            printf("%-22s\t", PQfname(res, i));
        }
        printf("\n");

        int numRows = PQntuples(res);
        for (int i = 0; i < numRows; i++)
        {
            for (int j = 0; j < numFields; j++)
            {
                printf("%-22s\t", PQgetvalue(res, i, j));
            }
            printf("\n");
        }

        // Clean up
        PQclear(res);*/
}

void add_doctor(PGconn *conn, Doctor doctor)
{
    char query[500];
    sprintf(query, "INSERT INTO doctor(user_name,password,name,family_name,date_of_birth,sex,serv_id) VALUES ('%s','%s','%s','%s','%s','%d','%d')", doctor.user_name, doctor.password, doctor.name, doctor.family_name, doctor.date_of_birth, doctor.sex, doctor.serv_id);

    PGresult *result = PQexec(conn, query);
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error executing query adding service: %s\n", PQresultErrorMessage(result));
        PQclear(result);
        exit_nicely(conn);
    }
    else
    {
        printf("doctor %s added succefully!\n", doctor.name);
    }
    PQclear(result);
}

void add_patient(PGconn *conn, Patient patient)
{
    char query[500];
    printf("patient NAME %s !\n", patient.name);
    sprintf(query, "INSERT INTO patient (user_name,password,name,family_name,date_of_birth,sex,descritption) VALUES ('%s','%s','%s','%s','%s','%d','%s')", patient.user_name, patient.password, patient.name, patient.family_name, patient.date_of_birth, patient.sex, patient.Description);
    PGresult *result = PQexec(conn, query); // user_name,password,name,family_name,date_of_birth,sex,descritption
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error executing query adding patient: %s\n", PQresultErrorMessage(result));
        PQclear(result);
        exit_nicely(conn);
    }
    else
    {
        printf("patient %s added succefully!\n", patient.name);
    }
    PQclear(result);
}

int getDoctorFromDB(PGconn *conn, int doctorId, Doctor *doctor)
{
    char to_be_converted_to_int[1000];
    char query[100];
    sprintf(query, "SELECT * FROM doctor WHERE id = %d", doctorId);

    PGresult *result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error fetching doctor: %s", PQresultErrorMessage(result));
        PQclear(result);
        return 0;
    }

    int rows = PQntuples(result);
    if (rows == 1)
    {
        doctor->id = atoi(PQgetvalue(result, 0, 0));
        snprintf(doctor->user_name, sizeof(doctor->user_name), "%s", PQgetvalue(result, 0, 1));
        snprintf(doctor->password, sizeof(doctor->password), "%s", PQgetvalue(result, 0, 2));
        snprintf(doctor->name, sizeof(doctor->name), "%s", PQgetvalue(result, 0, 3));
        snprintf(doctor->family_name, sizeof(doctor->family_name), "%s", PQgetvalue(result, 0, 4));
        snprintf(doctor->date_of_birth, sizeof(doctor->date_of_birth), "%s", PQgetvalue(result, 0, 5));
        doctor->sex = (PQgetvalue(result, 0, 6)[0] == 't');
        snprintf(to_be_converted_to_int, sizeof(to_be_converted_to_int), "%s", PQgetvalue(result, 0, 7));
        doctor->serv_id = atoi(to_be_converted_to_int);

        PQclear(result);
        return 1;
    }
    else
    {
        fprintf(stderr, "Doctor with ID %d not found.\n", doctorId);
        PQclear(result);
        return 0;
    }
}

int update_doctor(PGconn *conn, Doctor doctor)
{
    char query[500];
    sprintf(query, "UPDATE doctor SET name = '%s', family_name = '%s' ,date_of_birth = '%s', sex = '%d' ,serv_id = '%d'  WHERE id = %d", doctor.name, doctor.family_name, doctor.date_of_birth, doctor.sex, doctor.serv_id, doctor.id);
    PGresult *result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error Executing query modifying doctor: %s\n", PQresultErrorMessage(result));
        PQclear(result);
        exit_nicely(conn);
    }
    else
    {
        printf("doctor updated succefully");
        PQclear(result);

        // goto manage_doctor_menu;
    }
}

int update_patient(PGconn *conn, Patient patient)
{
    char query[500];
    sprintf(query, "UPDATE patient SET name = '%s', family_name = '%s' ,date_of_birth = '%s', sex = '%d' ,descritption = '%s'  WHERE id = %d", patient.name, patient.family_name, patient.date_of_birth, patient.sex, patient.Description, patient.id);
    PGresult *result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error Executing query modifying patient: %s\n", PQresultErrorMessage(result));
        PQclear(result);
        exit_nicely(conn);
    }
    else
    {
        printf("patient updated succefully");
        PQclear(result);
    }
}

int getPatientFromDB(PGconn *conn, int patientId, Patient *patient)
{
    char query[100];
    sprintf(query, "SELECT * FROM patient WHERE id = %d", patientId);

    PGresult *result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error fetching patient: %s", PQresultErrorMessage(result));
        PQclear(result);
        return 0;
    }

    int rows = PQntuples(result);
    if (rows != 0)
    {
        patient->id = atoi(PQgetvalue(result, 0, 0));
        snprintf(patient->user_name, sizeof(patient->user_name), "%s", PQgetvalue(result, 0, 1));
        snprintf(patient->password, sizeof(patient->password), "%s", PQgetvalue(result, 0, 2));
        snprintf(patient->name, sizeof(patient->name), "%s", PQgetvalue(result, 0, 3));
        snprintf(patient->family_name, sizeof(patient->family_name), "%s", PQgetvalue(result, 0, 4));
        snprintf(patient->date_of_birth, sizeof(patient->date_of_birth), "%s", PQgetvalue(result, 0, 5));
        patient->sex = (PQgetvalue(result, 0, 6)[0] == 't');
        snprintf(patient->Description, sizeof(patient->Description), "%s", PQgetvalue(result, 0, 7));

        PQclear(result);
        return 1;
    }
    else
    {
        fprintf(stderr, "patient with ID %d not found.\n", patientId);
        PQclear(result);
        return 0;
    }
}

int get_patient_id_from_db(PGconn *conn, Patient *p)
{
    char query[256];
    snprintf(query, sizeof(query),
             "SELECT id FROM patient WHERE user_name = '%s' AND password = '%s'", p->user_name, p->password);
    // Execute the query using PQexec
    PGresult *result = PQexec(conn, query);
    p->id = atoi(PQgetvalue(result, 0, 0));
    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Query execution failed while get_patient_id_from_db: %s", PQerrorMessage(conn));
        PQclear(result);
        exit_nicely(conn);
    }
}

int get_doctor_id_from_db(PGconn *conn, Doctor *p)
{
    char query[256];

    snprintf(query, sizeof(query),
             "SELECT id FROM doctor WHERE user_name = '%s' AND password = '%s'", p->user_name, p->password);
    // Execute the query using PQexec
    PGresult *result = PQexec(conn, query);
    p->id = atoi(PQgetvalue(result, 0, 0));
    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Query execution failed while get_doctor_id_from_db: %s", PQerrorMessage(conn));
        PQclear(result);
        exit_nicely(conn);
    }
}

/*int getDoctorFromDB(PGconn *conn, int doctorId,Doctor *doctor){
   char query[100];
    sprintf(query, "SELECT * FROM doctor WHERE id = %d", doctorId);

    PGresult *result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error while getDoctorFromDB: %s", PQresultErrorMessage(result));
        PQclear(result);
        return 0;
    }

    int rows = PQntuples(result);
    if (rows != 0)
    {
        doctor->id = atoi(PQgetvalue(result, 0, 0));
        snprintf(doctor->user_name, sizeof(doctor->user_name), "%s", PQgetvalue(result, 0, 1));
        snprintf(doctor->password, sizeof(doctor->password), "%s", PQgetvalue(result, 0, 2));
        snprintf(doctor->name, sizeof(doctor->name), "%s", PQgetvalue(result, 0, 3));
        snprintf(doctor->family_name, sizeof(doctor->family_name), "%s", PQgetvalue(result, 0, 4));
        snprintf(doctor->date_of_birth, sizeof(doctor->date_of_birth), "%s", PQgetvalue(result, 0, 5));
        doctor->sex = (PQgetvalue(result, 0, 6)[0] == 't');
        doctor->id = atoi(PQgetvalue(result, 0, 7));

        PQclear(result);
        return 1;
    }
    else
    {
        fprintf(stderr, "doctor with ID %d not found.\n", doctorId);
        PQclear(result);
        return 0;
    }
}*/
