#include <libpq-fe.h>
#include "types.h"
#include "databasefuncs.h"

int patient_request_certificate(PGconn *conn, Patient *p)
{
certificat_req: 
    char day[5];
    Certificat cert;
    cert.requested_by_patient = true;

    printf("Hello %s \n plz give us some informations about your certifcate \n", p->name);
    printf("here are your doctors :\n");
    get_id_and_name(conn, "doctor", "patient", "doctor_patient", p->id);

    printf("Enter the ID of your doctor :");
    scanf("%d", &cert.doctor_id);
    printf("\nEnter cause : ");
    scanf("%s", cert.cause);
    printf("Enter The number of days : ");
    scanf("%d", &(cert.duration));
    printf("Enter the starting day for the certificate [MM-DD]: ");
    scanf("%s", day);
    sprintf(cert.created_at, "2023-%s", day);
    char query[500];

    sprintf(query, "INSERT INTO certificat(doctor_id,patient_id,duration,cause,requested_by_patient,created_at) VALUES (%d,%d,%d, '%s', %s,'%s')", cert.doctor_id, p->id, cert.duration, cert.cause, cert.requested_by_patient ? "true" : "false", cert.created_at);

    PGresult *result = PQexec(conn, query);
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error invalid certifcat request: %s\n", PQresultErrorMessage(result));
        PQclear(result);
        printf("Do u want to try again [Y/n] : ");
        char opt;
        scanf("%c", &opt);
        if (opt == 'Y' || opt == 'y')
            goto certificat_req;
    }
    else
        printf("Certificat Requested Successfully \nwe wait for your doctor to approve it\n");
    
    return 1;
}

int patient_reuquest_appointment(PGconn *conn, Patient *p)
{
Appointment_req:
    Appointment appointment;
    Appointment *app=&appointment;

    (app->patient_id) = (p->id);
    app->requested_by_patient = true;
    char query[500];
    char day[5];
    // GET DOCTOR
    printf("Hello %s \n plz give us some informations about your appointment \nHere are your doctors :\n", p->name);
    get_id_and_name(conn, "doctor", "patient", "doctor_patient", p->id);
    printf("Enter the ID of the doctor of the appointment :");
    scanf("%d", &(app->doctor_id));
    printf("Enter the date of the appointment [MM-DD]: ");
                //TODO APPOINTMNT TIME

    scanf("%s",day);
    sprintf(app->date, "2023-%s", day);



    sprintf(query,"INSERT INTO appointment(patient_id,doctor_id,requested_by_patient,date) VALUES ( %d , %d , %s , '%s')",app->patient_id,app->doctor_id,app->requested_by_patient ? "true" : "false", app->date );
    PGresult *result = PQexec(conn, query);


    //IN CASE QUERY ERROR
        char opt;
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error invalid appointment request: %s\n", PQresultErrorMessage(result));
        PQclear(result);
        printf("Do u want to try again [Y/n] : ");
        scanf("%c", &opt);
        if (opt == 'Y' || opt == 'y')
            goto Appointment_req;
    }
    printf("\nAppointment Requested Successfully \nwe wait for your doctor to approve it\n");
    return 1;
}

int patient_view_description(PGconn *conn, Patient *p) {
    char opt = 'Y';
    do{
        char query[500];
        sprintf(query, "SELECT descritption FROM patient WHERE id = %d", p->id);
        PGresult *result = PQexec(conn, query);
        print_pgresult(result);

        printf("Do u want Return  [Y/n] : ");
        scanf(" %c", &opt);
        if (opt == 'Y' || opt == 'y')
            return 1;
        
    }while(opt != 'Y' && opt != 'y');
    return 1;
}

int patient_view_certificate(PGconn *conn, Patient *p) {
    char opt = 'Y';
    do{
        char query[500];
        sprintf(query, "SELECT certificat.created_at , certificat.duration , certificat.cause FROM certificat WHERE ( certificat.patient_id = %d AND certificat.approved = true)", p->id);
        PGresult *result = PQexec(conn, query);
        print_pgresult(result);

        printf("Do u want Return  [Y/n] : ");
        scanf(" %c", &opt);
        if (opt == 'Y' || opt == 'y')
            return 1;
        
    }while(opt != 'Y' && opt != 'y');
    return 1;
}

int patient_view_ordonnance(PGconn *conn, Patient *p) {
    char opt = 'Y';
    do{
        char query[500];
        sprintf(query, "SELECT doctor.name , ordonnance.content FROM doctor , ordonnance WHERE ordonnance.patient_id = %d  AND doctor.id = ordonnance.doctor_id; ", p->id);
        PGresult *result = PQexec(conn, query);
        print_pgresult(result);

        printf("Do u want Return  [Y/n] : ");
        scanf(" %c", &opt);
        if (opt == 'Y' || opt == 'y')
            return 1;
        
    }while(opt != 'Y' && opt != 'y');
    return 1;
}

int patient_get_doctor(PGconn *conn, Patient *p){
// SHOW DOCTORS ASSOCIATED WITH THIS PATIENT
    printf("Here are your doctors :\n");
    get_id_and_name(conn, "doctor", "patient", "doctor_patient", p->id);
    int serv_id,doc_id;

// CHOOSE THE SERVICE
    printTable(conn,"service"," * ","id !=0");
    printf("\n\nEnter the ID of the service u want to get a doctor from: \n");
    scanf(" %d", &serv_id);
// CHOOSE THE DOCTOR
    printf("Here are the available doctors for the chosen service:\n");
    char condition[50];
    sprintf(condition,"serv_id = %d",serv_id);
    printTable(conn,"doctor"," id , name , family_name ",condition);
    printf("enter The ID of The doctor :\n");
    scanf("%d",&doc_id);
// ASSOCIATE THE CHOSEN DOCTOR TO THE PATIENT
    char query[500];
    sprintf(query, "INSERT INTO doctor_patient(patient_id,doctor_id) VALUES (%d,%d)", p->id,doc_id);
    PGresult *result = PQexec(conn,query);
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error invalid get_doctor request: %s\n", PQresultErrorMessage(result));
        PQclear(result);
        return 0;
    }   
    printf("Doctor added succefully! \n\n\n");
    return 1;
}

int patient_view_upcoming_appointment(PGconn *conn, Patient *p) {
    printf("Here are your appointments for the next week :\n");
    char query[500];
    sprintf(query,"SELECT appointment.date ,doctor.name, doctor.family_name  FROM appointment , doctor WHERE appointment.date BETWEEN CURRENT_DATE AND CURRENT_DATE + INTERVAL '7 days' and appointment.doctor_id = doctor.id and appointment.patient_id = %d;", p->id);
    PGresult *result = PQexec(conn, query);
    print_pgresult(result);
    return 1;
}

// id SERIAL PRIMARY KEY, patient_id INT REFERENCES patient(id) ON DELETE CASCADE, doctor_id INT REFERENCES doctor(id) ON DELETE SET NULL, created_at DATE, duration INT , cause VARCHAR , requested_by_patient BOOLEAN