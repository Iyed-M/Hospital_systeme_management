#include "types.h"
#include <libpq-fe.h>
#include <stdio.h>
#include "databasefuncs.h"
#include "menus.h"
#include "logins.h"
#include "check_login_in_db.h"
#include <string.h>

int doctor_access(PGconn *conn){
    // LOGIN SECTION
    Appointment app;
    Appointment *appointment=&app;
    Certificat cert;
    Certificat *certificat=&cert;
    Ordonnance ord;
    Ordonnance *ordonnance=&ord;
    Doctor doc;
    Doctor *doctor=&doc;
    Patient pat;
    Patient *patient=&pat;
    clearScreen();
    doctor_login(doctor);
    PGresult *result;
    char NewDesc[200];
    char condition[100];
    char choice,opt;
    int option,exit;
    char query[100];
    int exit_from_doctor_menu;
    //LOGIN SECTION
    while (successful_doctor_login(doctor, conn) == 0)
    {
        printf("login failed ..\ndo u want try again [Y/n] : ");
        scanf(" %c", &choice);
        if (choice == 'N' || choice == 'n')
            return 0;
        else
            doctor_login(doctor);
    }
DOCTOR_MENU:
    clearScreen();
    get_doctor_id_from_db(conn, doctor);//=> gets the doctor id that matches the password and username
    getDoctorFromDB(conn, doctor->id, doctor);
    do
    {      
      printf("Welcome  %s %s \nHere are your patients\n\n\n", doctor->name,doctor->family_name);
      get_id_and_name(conn,"patient","doctor","doctor_patient",doctor->id);
      printf("\nEnter the ID of the patient you want to manage :");
      scanf(" %d", &patient->id);
      getPatientFromDB(conn, patient->id, patient);
      doctor_menu();
      scanf(" %d", &option);
      switch(option){
        case 1:
          clearScreen();
          do
          {
            printf("Here s %s Medical Record : \n %s\n", patient->name,patient->Description);
            printf("enter 1 to Go back: ");
            scanf(" %d", &exit);
          }while(!exit);
          goto DOCTOR_MENU;
        case 2:
          do
            {
              clearScreen();
              printf("Enter The Description : \n");
              fgets(patient->Description, sizeof(patient->Description), stdin);
              sprintf(query, "UPDATE patient SET descritption = '%s' where id = %d", patient->Description, patient->id);
              result=PQexec(conn, query);
              if (PQresultStatus(result)==PGRES_COMMAND_OK){
                  printf("Medical record Added succefully!\n");
              }
              else{
                  fprintf(stderr, "Error while Adding Medical record  %s\n Let's try again", PQresultErrorMessage(result));
                  PQclear(result);
              }
                  printf("enter 1 to Go back: ");
                  scanf("%d", &exit);
            } while (!exit);
            break;
        case 3:
rmv_mdc_record:
          clearScreen();
          printf("Are You Sure You Want To Remove The Medical Record Of %s %s [Y/n] :",patient->name,patient->family_name);
          scanf(" %c", &opt);
          if (opt != 'Y' && opt != 'y')
          patient->Description[0] = '\0';
          sprintf(query,"update patient set descritption = NULL where id = %d", patient->id);
          result =PQexec(conn, query);
          if (PQresultStatus(result)==PGRES_COMMAND_OK){
              printf("Medical record  Removed succefully!\n");
          }
          
          else{
            fprintf(stderr, "Error while Removing Medical record  %s\n Let's try again", PQresultErrorMessage(result));
            PQclear(result);
            goto rmv_mdc_record;
            }
          break;
case 4:
        edit_medical_record:
            clearScreen();
            printf("Here Is The Current Medical Description For %s : \n%s \n", patient->name,patient->Description);
            printf("Add To The Description:\n");
            scanf("%s", NewDesc);
            strcat(patient->Description, NewDesc);
            sprintf(query, "UPDATE patient SET descritption = '%s' where id = %d", patient->Description, patient->id);
            result=PQexec(conn, query);
            if (PQresultStatus(result)==PGRES_COMMAND_OK){
                printf("Medical record edited succefully!\n");
            }
            else{
                fprintf(stderr, "Error while editing Medical record %s\n Let's try again", PQresultErrorMessage(result));
                PQclear(result);
                goto edit_medical_record;
            }
            break;
        case 5:
    add_perscription:
            clearScreen();
            printf("Here's the patient Medical Record : \n%s\n", patient->Description);
            fgets(ordonnance->content, sizeof(ordonnance->content), stdin);
            sprintf(query, "INSERT INTO ordonnance (patient_id, doctor_id, content) VALUES (%d, %d, '%s')", patient->id, doctor->id, ordonnance->content);
            
            result=PQexec(conn,query);
            if (PQresultStatus(result)==PGRES_COMMAND_OK){
                printf("Perscription added succefully!\n");
            }
            else{
                fprintf(stderr, "Error while adding Perscription %s\n Let's try again", PQresultErrorMessage(result));
                PQclear(result);
                goto add_perscription;
            }
            break;
  
        case 6:
            clearScreen();
            appointment->requested_by_patient=false;
            appointment->patient_id=patient->id;
            appointment->doctor_id=doctor->id;
            printf("Enter The Date Of The Appointment [MM-DD] :\n");
            scanf("%s",appointment->date);
            sprintf(query, "INSERT INTO appointment (requested_by_patient,patient_id, doctor_id, date) VALUES (false , %d, %d, '2023-%s')", patient->id, doctor->id, appointment->date);
            result = PQexec(conn, query);
            if (PQresultStatus(result)==PGRES_COMMAND_OK){
                printf("Appointment requested succefully!\nWaiting for the admin to accept it");
            }
            else{
                fprintf(stderr, "Error while adding Perscription %s\n Let's try again", PQresultErrorMessage(result));
                PQclear(result);
            }
            break;
        case 7:
        // TODO : make a year variable where the admin changes its value every year
make_certificat:
            clearScreen();
            certificat->approved=true;
            certificat->requested_by_patient=false;
            printf("Hello %s Here You Make a Certifcate for %s \n:", doctor->name, patient->name);
            printf("Enter The Cause of The Certificat : \n");
            scanf(" ");
            fgets(certificat->cause, sizeof(certificat->cause), stdin);
            printf("Enter The Starting Day for The Certificate [MM-DD]:\n");
            scanf("%s",certificat->created_at);
            printf("Enter The Duration of The Certficate [NumberOfDays] :\n");
            scanf("%d",&(certificat->duration));
            sprintf(query," INSERT INTO certificat (patient_id,doctor_id,created_at,duration,approved,cause,requested_by_patient)VALUES (%d, %d, '2023-%s', %d,'true' , '%s', 'false');",patient->id,doctor->id,certificat->created_at,certificat->duration,certificat->cause);
            result = PQexec(conn,query);
            if (PQresultStatus(result)==PGRES_COMMAND_OK){
                printf("certificat added succefully!\n");
            } 
            else{
                fprintf(stderr, "Error while adding certificat %s\n Let's try again", PQresultErrorMessage(result));
                PQclear(result);
                goto make_certificat;
            }
            break;

        case 8:
            clearScreen();
            
        check_certificat:
            printf("Checking if %s requested a certificate : \n", patient->name);
            sleep(0.2);
        
            sprintf(query, "SELECT id ,created_at , duration , cause FROM certificat WHERE patient_id = %d AND requested_by_patient = TRUE AND approved == FALSE", patient->id);
            result = PQexec(conn,query);
            if (PQntuples(result) == 0){
                PQclear(result);
                printf("No Certificat Requested From Patient %s %s Yet\n", patient->name,patient->family_name);
                printf("Would u like to make a certificate for %s %s? [Y/n] :", patient->name);
                scanf("%c",&opt);
                if (opt == 'Y' || opt == 'y'){
                    goto make_certificat;
                }
            }
            else{
                printf("Here's The Certificat(s) Requested From Patient %s %s \n: \n", patient->name,patient->family_name);
                print_pgresult(result);
                PQclear(result);
                printf("enter the id of the certificat you want to respond to : \n");
                scanf("%d",&certificat->id);
                printf("do you approve the certificat [true/false] : \n");
                scanf("%d",&(certificat->approved));
                sprintf(query, "UPDATE certificat SET approved = %s WHERE id = %d", certificat->approved ? "TRUE" : "FALSE", certificat->id);
                result = PQexec(conn,query);
                goto check_certificat;
            }
            return 1;

        case 9:
            goto DOCTOR_MENU;
      }
    } while (!exit_from_doctor_menu);

}