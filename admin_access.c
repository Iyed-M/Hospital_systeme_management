#include <libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "databasefuncs.h"
#include "menus.h"
#include "admin_utility.h"
#include "logins.h"
#include "check_login_in_db.h"

void admin_access(PGconn *conn)
{
    Admin admin;
    Admin *admin_ptr = &admin;
    clearScreen();
    admin_login(admin_ptr);

    char choice;

    // loop until successful login
    while (successful_admin_login(admin_ptr, conn) == 0)
    {
        clearScreen();
        printf("login failed ..\n do u want try again [Y/n] ");
        scanf("%c", &choice);
        if (choice == 'N' || choice == 'n')
        {
            return;
        }
        else
            admin_login(admin_ptr);
    }
    // THIS IS THE ADMIN MENU WHERE HE CAN ENTER SUB_MENUS
    unsigned int option;

    do
    {
        clearScreen();
        admin_menu();
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            clearScreen();
            admin_manage_service(conn);
            break;
        case 2:
            clearScreen();
            admin_manage_doctor(conn);
            break;
        case 3:
            clearScreen();
            admin_manage_patient(conn);
            break;
        case 4:
            clearScreen();
            admin_manage_appointments(conn);
            break;
        case 5:
            clearScreen();
            admin_manage_public_holidays(conn);
            break;

        case 6:
            clearScreen();
            break;
        case 7:

            printTable(conn, "patient", "*", " id!=0");
        }
    } while (option != 6);
}
