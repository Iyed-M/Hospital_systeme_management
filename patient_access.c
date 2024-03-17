#include "types.h"
#include <stdio.h>
#include "menus.h"
#include <libpq-fe.h>
#include "types.h"
#include "patient_functions.h"
#include "logins.h"
#include "check_login_in_db.h"
#include "databasefuncs.h"
#include "time.h"

int patient_access(PGconn *conn)
{
    // LOGIN SECTION

    Patient *patient_ptr;
    patient_login(patient_ptr);

    char choice;
    int option;

    while (successful_patient_login(patient_ptr, conn) == 0)
    {
        printf("login failed ..\n do u want try again [Y/n] ");
        scanf("%c", &choice);
        if (choice == 'N' || choice == 'n')
            return 0;
        else
            patient_login(patient_ptr);
    }
    get_patient_id_from_db(conn, patient_ptr);            // gets the patient id that matches the password and username
    getPatientFromDB(conn, patient_ptr->id, patient_ptr); // stores the patient data in patient_ptr using the patient id
    do
    {
        patient_menu();
        scanf("%d", &option);
        switch (option)

        {
        case 1:
            //clearScrean();
            patient_request_certificate(conn, patient_ptr);
            break;
        case 2:
            //clearScrean();

            patient_reuquest_appointment(conn, patient_ptr);
            break;
        case 3:
            //clearScrean();

            patient_view_description(conn, patient_ptr);
            break;
             
        case 4:
            //clearScrean();
            patient_view_certificate(conn, patient_ptr);
            break;
        case 5:
            //clearScrean();
            patient_view_ordonnance(conn, patient_ptr);
            break;
  
        case 6:
            //clearScrean();
            patient_view_upcoming_appointment(conn, patient_ptr); 
            break;
        case 7:
            //clearScrean();
            patient_get_doctor(conn, patient_ptr);
            break; 
 
        case 8:
            //clearScrean();
            return 1;
        }
    } while (option != 8);
}