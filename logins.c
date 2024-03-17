#include <stdio.h>
#include <unistd.h>
#include "types.h"
#include <stdlib.h>

void admin_login(Admin *p)
{

    printf("\t\t===========\t Admin Login \t========== \n");
    printf("username : ");
    scanf("%s", &(p->user_name));
    printf("password : ");
    scanf("%s", &(p->password));
    printf("\n\n\n");
}
void doctor_login(Doctor *p)
{
    // to add a while loop to verify password and username in database
    printf("\t\t===========\t Doctor Login \t========== \n");
    printf("username : ");
    scanf("%s", &(p->user_name));
    printf("password : ");
    scanf("%s", &(p->password));
    printf("\n\n\n");
}
int patient_login(Patient *p)
{
    // to add a while loop to verify password and username in database
    printf("===========\t Patient Login \t========== \n");
    printf("username : ");
    scanf("%s", &(p->user_name));
    printf("password : ");
    scanf("%s", &(p->password));
    printf("\n\n\n");
}
