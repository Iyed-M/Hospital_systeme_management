#include <stdlib.h>
#include <libpq-fe.h>
#include <stdio.h>
#include <libpq-fe.h>
#include "databasefuncs.h"
#include "types.h"
#include "menus.h"
#include "get.h"
void admin_manage_service(PGconn *conn)
{   
    clearScreen();
    PGresult *result;
    int option;
    int service_id;
    char service_name[50];
    char query[250];
    do
    {

        manage_service_menu();
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            // ADD SERVICE
            printf("Service name to add: ");
            scanf("%s", &service_name);
            sprintf(query, "INSERT INTO service(name) VALUES ('%s')", service_name);
            result = PQexec(conn, query);

            if (PQresultStatus(result) != PGRES_COMMAND_OK)
            {
                fprintf(stderr, "Error executing query adding service: %s\n", PQresultErrorMessage(result));
                PQclear(result);
                exit_nicely(conn);
            }
            else
            {
                printf("service %s added succefully!\n", service_name);
            }
            break;

        case 2:
            // DELETE SERVICE
            printf("Here are the current services\n");
            printTable(conn, "service", " * ", "");
            printf("Service id to delete: \n");
            scanf("%d", &service_id);
            sprintf(query, "DELETE FROM service WHERE id = ('%d')", service_id);
            result = PQexec(conn, query);
            if (PQresultStatus(result) != PGRES_COMMAND_OK)
            {
                fprintf(stderr, "Error executing query deleting service: %s\n", PQresultErrorMessage(result));
                PQclear(result);
                exit_nicely(conn);
            }
            else
            {
                printf("service %s deleted succefully!", service_name);
            }

            break;

        case 3: // PRINTS ALL SERVICES
            printf("Here are the current services\n");
            printTable(conn, "service", " * ", "");
            break;
        case 4:
            return;
        }
    } while (option != 4);
}
void admin_manage_doctor(PGconn *conn)
{
    // manage_doctor_menu :
    int choice;
    Doctor doctor;
    int doc_id;
    Doctor *doctor_ptr = &doctor;
    do
    {

        manage_doctor_menu();
        scanf(" %d", &choice);
        switch (choice)
        {
        case 1:
            // ADD DOCTOR

            get_doctor(conn, doctor_ptr);
            add_doctor(conn, doctor);
            // goto manage_doctor_menu;
            break;
        case 2:
            // remove doctor
            printf("\t\t HERE ARE THE CURRENT DOCTORS\n");
            printTable(conn, "doctor", " * ", "");

            printf("Enter the ID of the doctor to remove :");

            scanf("%d", &doc_id);
            char query[250];
            sprintf(query, "DELETE FROM doctor WHERE id = ('%d')", doc_id);
            PGresult *result = PQexec(conn, query);
            if (PQresultStatus(result) != PGRES_COMMAND_OK)
            {
                fprintf(stderr, "Error executing query deleting doctor: %s\n", PQresultErrorMessage(result));
                PQclear(result);
                exit_nicely(conn);
            }
            else
            {
                printf("doctor deleted succefully\n");
                PQclear(result);
            }
            break;
        case 3:
            // PRINTS ALL DOCTORS
            printf("Here are the current doctors\n");
            printTable(conn, "doctor", " * ", "");
            break;
        case 4:
            // Modify doctor
            printf("Here are the current doctors\n");
            printTable(conn, "doctor", " * ", "");
            printf("Enter the ID of the doctor to modify : ");

            scanf("%d", &(doctor.id));
            getDoctorFromDB(conn, doctor.id, doctor_ptr);
            modify_doctor(conn, doctor_ptr);
            update_doctor(conn, doctor);

            break;
        case 5:
            return;
        }
    } while (choice != 5);
}
void admin_manage_patient(PGconn *conn)
{

    int option;
    Patient patient;
    Patient *patient_ptr = &patient;
    do
    {
        manage_patient_menu();
        scanf(" %d", &option);

        switch (option)
        {
        case 1: // show
            printf("HERE ARE ALL THE CURRENT PATIENTS \n");
            printTable(conn, "patient", " * ", "");
            break;
        case 2: // add
            printTable(conn, "patient", " * ", "");
            get_patient(patient_ptr);
            printf("test patient name = %s",patient.name);
            add_patient(conn, patient);

            break;
        case 3: // delete
            printTable(conn, "patient", " * ", "");
            printf("Enter the ID of the patient to remove : ");
            scanf("%d", &patient.id);
            char query[250];
            sprintf(query, "DELETE FROM patient WHERE id = ('%d')", patient.id);
            PGresult *result = PQexec(conn, query);
            if (PQresultStatus(result) != PGRES_COMMAND_OK)
            {
                fprintf(stderr, "Error executing query deleting patient: %s\n", PQresultErrorMessage(result));
                PQclear(result);
                exit_nicely(conn);
            }
            else
            {
                printf("doctor deleted succefully\n");
                PQclear(result);
            }
            break;
        case 4: // modify
            printf("Here are the current patients\n");
            printTable(conn, "patient", " * ", "");
            printf("Enter the ID of the patient to modify : ");
            scanf("%d", &patient.id);
            getPatientFromDB(conn, patient.id, patient_ptr);
            modify_patient(patient_ptr);
            update_patient(conn, patient);
            break;
        case 5:
            return;
        }

    } while (option != 5);
}
int admin_manage_appointments(PGconn *conn){
    printf("Hello Admin\nHere are the new appointments plz answer the requests \n");
    PGresult *result;
    int appointment_id,validity,exit=0;
    char query[500];   
    do {
        // Prints all appending appointments
        sprintf(query, "SELECT appointment.id, "
                    "CONCAT(doctor.name, ' ', doctor.family_name) AS doctor, "
                    "CONCAT(patient.name, ' ', patient.family_name) AS patient, "
                    "appointment.date AS date, "
                    "CASE WHEN appointment.requested_by_patient THEN 'true' ELSE 'false' END AS requested_by_patient "
                    "FROM appointment "
                    "INNER JOIN doctor ON appointment.doctor_id = doctor.id "
                    "INNER JOIN patient ON appointment.patient_id = patient.id "
                    "WHERE appointment.validity IS NULL;");
    // gets the response from admin
        result = PQexec(conn, query);
        print_pgresult(result);
        printf("Enter the ID of the appointment to Respond To : \n");
        scanf(" %d",&appointment_id);
        printf("Enter 1 to accept or 0 to reject : \n");
        scanf(" %d",&validity);
    // execute admin response 
        sprintf(query, "UPDATE appointment SET validity = %s WHERE id = %d ",validity ? "true" : "false" ,appointment_id);
        result = PQexec(conn, query);
        if (PQresultStatus(result) != PGRES_COMMAND_OK){
            fprintf(stderr, "Error executing query validating appointment: %s\n", PQresultErrorMessage(result));
        }
        else{
            fprintf(stdout,"Appointment %s succefully!\n", validity ? "accepted" : "rejected");
        }
        printf("Do U Want To Exit[1] or Keep Managing Appointments[0] : \n");
        scanf(" %d",&exit);
        
    }while(!exit);
    return 1;
}
void admin_manage_public_holidays(PGconn *conn)
{
    int option;
    char query[250];
    PGresult *result;
    char day[11];
    int ID;
    do
    {
        manage_public_holidays_menu();
        scanf("%d", &option);
        switch (option)
        {
        case 1:

            printTable(conn, "public_holiday", " * ", "");
            printf("enter the of the public holiday to add[YYYY-MM-DD]: ");
            scanf("%s", day);

            sprintf(query, "INSERT into  public_holiday(day) VALUES('%s')", day);
            result = PQexec(conn, query);
            if (PQresultStatus(result) != PGRES_COMMAND_OK)
            {
                fprintf(stderr, "Error executing query deleting day: %s\n", PQresultErrorMessage(result));
                PQclear(result);
                exit_nicely(conn);
            }
            else
            {
                printf("day deleted succefully\n");
                PQclear(result);
            }
            break;

            break;
        case 2:

            printTable(conn, "public_holiday", " * ", "");
            printf("enter the of the public holiday to remove[YYYY-MM-DD]: ");
            scanf("%s", day);

            sprintf(query, "DELETE FROM public_holiday WHERE day = ('%s')", day);
            result = PQexec(conn, query);
            if (PQresultStatus(result) != PGRES_COMMAND_OK)
            {
                fprintf(stderr, "Error executing query deleting day: %s\n", PQresultErrorMessage(result));
                PQclear(result);
                exit_nicely(conn);
            }
            else
            {
                printf("day deleted succefully\n");
                PQclear(result);
            }
            break;

        case 3:
            printTable(conn, "public_holiday", " * ", "");
            break;
        case 4:
            return;
        }

    } while (option != 4);
}
