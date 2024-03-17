#include "types.h"
#include <stdio.h>
#include <unistd.h>
#include <libpq-fe.h>
#include "databasefuncs.h"
#include <string.h>

void get_doctor(PGconn *conn, Doctor *p){

    printf("doctor username : ");
    scanf("%s", &(p->user_name));
    printf("doctor password : ");
    scanf("%s", &(p->password));
    printf("doctor name : ");
    scanf("%s", &(p->name));
    printf("doctor family_name : ");
    scanf("%s", &(p->family_name));
    printf("doctor date_of_birth [YYYY-MM-DD]: ");
    scanf("%s", &(p->date_of_birth));
    printf("doctor sex [1 for male , 0 for female]: ");
    scanf("%d", &(p->sex));
    printTable(conn, "service", " * ", "");
    printf("doctor serv_id : ");
    scanf("%d", &(p->serv_id));
}

void get_patient(Patient *p){

    printf("patient username(*) : ");
    scanf("%s", &(p->user_name));
    printf("patient password(*) : ");
    scanf("%s", &(p->password));
    printf("patient name (*): ");
    scanf("%s", (p->name));
    printf("patient family_name(*) : ");
    scanf("%s", &(p->family_name));
    printf("patient date_of_birth(*) [YYYY-MM-DD]: ");
    scanf("%s", &(p->date_of_birth));
    printf("patient sex (*) [1 for male , 0 for female]: ");
    scanf("%d", &(p->sex));
    printf("patient description(optional) : ");
    scanf("%s", &(p->Description));
}

int modify_doctor(PGconn *conn, Doctor *p)
{
    char option[10];

    printf("Enter The Numbers Of The Attributes You Want To Update separated by no spaces [ex : 132]\n");
    printf("0.  Name : %s\n", p->name);
    printf("1.  Family_name : %s\n", p->family_name);
    printf("2.  Date_of_birth : %s\n", p->date_of_birth);
    printf("3.  Sex : %s\n", (p->sex) ? "Male" : "Female");
    printf("Here are the services available : \n");
    printTable(conn, "service", " * ", "");
    printf("4.  serv_id : %d\n", p->serv_id);
    printf("5.  Exit\n");
    scanf("%s", option);
    if (option[0] == '5')
        return 0;
    for (int i = 0; i < 5; i++)
    {

        switch (option[i])
        {
        case '0':
            printf("name : ");
            scanf("%s", &(p->name));
            break;
        case '1':
            printf("family_name : ");
            scanf("%s", &(p->family_name));
            break;
        case '2':
            printf("Birthday : ");
            scanf("%s", &(p->date_of_birth));
            break;
        case '3':
            printf("sex : ");
            scanf("%d", &(p->sex));
            break;
        case '4':
            printTable(conn, "service", " * ", "");
            printf("enter the service id :");
            scanf("%d", &(p->serv_id));
            break;
        case '6':
            return 0;
            break;
        }
    }
    return 1;
}

int modify_patient(Patient *p)
{
    char doc_table_attributes[5][50] = {"name", "family_name", "date_of_birth", "sex", "Descritpion"};
    char option[5] = {'6', '6', '6', '6', '6'};

    printf("Enter The Numbers Of The Attributes You Want To Update attached [ex : 132]\n");
    printf("0.  Name : %s\n", p->name);
    printf("1.  Family_name : %s\n", p->family_name);
    printf("2.  Date_of_birth : %s\n", p->date_of_birth);
    printf("3.  Sex : %s\n", (p->sex) ? "Male" : "Female");
    printf("4.  Description : %s\n", p->Description);
    printf("5.  Exit\n");
    scanf("%s", &option);

    for (int i = 0; i < 5; i++)
    {
        printf("%c", option[i]);
        switch (option[i])
        {
        case '0':
            printf("Enter The New %s : ", doc_table_attributes[0]);
            scanf("%s", &(p->name));
            break;
        case '1':
            printf("Enter The New %s : ", doc_table_attributes[1]);
            scanf("%s", &(p->family_name));
            break;
        case '2':
            printf("Enter The New %s : ", doc_table_attributes[2]);
            scanf("%s", &(p->date_of_birth));
            break;
        case '3':
            printf("Enter The New %s [Male:1 ,Female:0]: ", doc_table_attributes[3]);
            scanf("%d", &(p->sex));
            break;
        case '4':
            printf("Enter The New %s : ", doc_table_attributes[4]);
            scanf("%s", &(p->name));
            break;
        case '5':
            return 0;
            break;
        }
    }
    return 1;
}

