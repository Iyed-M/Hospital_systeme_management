all:
	gcc main.c admin_access.c patient_access.c patient_functions.c doctor_access.c doctor_functions.c check_login_in_db.c get.c admin_updates.c databasefuncs.c menus.c logins.c admin_utility.c -o main -lpq
