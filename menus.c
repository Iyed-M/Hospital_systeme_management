#include <stdio.h>
#include <stdlib.h>
void clearScreen() {
    // Use the "clear" command on Unix-based systems
    // Use the "cls" command on Windows
    #ifdef _WIN32 
        system("cls");
    #else // --> Linux User :/
        system("clear");
    #endif
}
void main_menu()
{
    //clearScrean();
    printf("============\tWelcome to the hospital\t============\n");
    printf("1. Admin\n");
    printf("2. Doctor\n");
    printf("3. Patient\n");
    printf("4. Exit\n");
    printf("\n");
}

// ADMIN MENU SECTION
void admin_menu() //
{
    //clearScrean();
    printf("===============\tAdmin Menu\t===============\n");
    printf("1. Manage services\n");
    printf("2. Manage doctors\n");
    printf("3. Manage patients\n");
    printf("4. Manage appointments\n"); // TODO
    printf("5. Manage public holidays\n");
    printf("6. Exit\n");
    printf("\n");
}

void manage_service_menu() // done
{
    printf("===============\tManage Service Menu\t============\n");
    printf("1. Add services\n");
    printf("2. Remove Services\n");
    printf("3. Show All Services\n");
    printf("4. Exit\n");
    printf("\n\n\n\n");
}

void manage_doctor_menu() // done
{
    printf("===============\tManage Doctor Menu\t============\n");
    printf("1. Add Doctor\n");
    printf("2. Remove Doctor\n");
    printf("3. Show ALL Doctors\n");
    printf("4. Modify Doctor\n");
    printf(". Exit\n");
    printf("\n\n\n");
    //clearScrean();
}

void manage_patient_menu() // done
{
    //clearScrean();
    printf("===============\tManage Patient Menu\t============\n");
    printf("1. Show ALL  Patients\n");
    printf("2. Add Patient\n");
    printf("3. Remove Patient\n");
    printf("4. Modify Patient\n");
    printf("5. Exit\n");
    printf("\n\n\n");
}

void manage_public_holidays_menu() // done
{
    //clearScrean();
    printf("===============\tManage Public Holidays Menu\t============\n");
    printf("1. Add Public Holidays\n");
    printf("2. Remove Public Holidays\n");
    printf("3. Show All Public Holidays\n");
    printf("4. Exit\n\n\n\n”");
}

// DOCTOR MENU SECTION
void doctor_menu()
{
    printf("===============\tDoctor Menu\t===============\n");
    printf("1. View Patient Medical Record (dossier medical)\n");
    printf("2. Add Patient Medical Record (dossier medical)\n");
    printf("3. Remove Patient Medical Record (dossier medical)\n");
    printf("4. Edit Patient Record (dossier medical)\n");
    printf("5. Make Patient Prescriptions (ordonnance)\n"); //TODO MAKE  ORDONNANCE TABLE BETTER (medicamentq)
    printf("6. Suggest An Appointment(rendez_vous)\n");
    printf("7. Make Patient Certificates \n");
    printf("8. Respond to Patient Certificate Request\n");
    printf("9. Exit\n\n\n");
    printf("\n");
}

void doctor_manage_patient_record_menu()
{
    //clearScrean();
    printf("===============\tManage Patient Record Men===u\t============\n");
    printf("1. View Patient Record\n");
    printf("2. Add Patient Record\n");
    printf("3. Remove Patient Record\n");
    printf("4. Go Backn\n\n\n\n");
}
void doctor_add_patient_prescription_menu()
{
}

// PATIENT MENU SECTION
void patient_menu()
{
    //clearScrean(); 

    printf("===============\tPatient Menu\t===============\n");
    printf("1. Request Certificate\n");
    printf("2. Request Appointment\n");
    printf("3. View Description Of Diseases\n"); //
    printf("4. View Certificates\n");
    printf("5. View Prescriptions(ordonnance)\n");
    printf("6. View upcoming Appointments\n");
    printf("7. Get a doctor\n");
    printf("8. Exit\n");
    printf("\n\n\n\n");
}
