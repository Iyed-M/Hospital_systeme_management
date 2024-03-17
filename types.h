#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
// Date : this is used to store birthdates
/**
 * @Date this is a date datatype
 *
 */



typedef struct
{
    int day;
    int month;
    int year;
} Date;

// Admin
typedef struct
{
    char user_name[100];
    char password[100];
} Admin;

// Services
typedef struct
{
    int id;
    char name[100];
} Service;

// DoesService
typedef struct
{
    int id;
    int service_id;
    int doctor_id;
} DoesService;

//  Patient
typedef struct
{
    char user_name[100];
    char password[100];
    char name[100];
    char family_name[100];
    char date_of_birth[11];
    char Description[1000];
    bool sex;
    int id;
} Patient;

//  Doctor_Patient
typedef struct
{
    int id;
    int patient_id;
    int doctor_id;
} Doctor_Patient;

// Doctors
typedef struct
{
    int id;
    char user_name[100];
    char password[100];
    char name[100];
    char family_name[100];
    char date_of_birth[11];
    bool sex;
    int serv_id;
} Doctor;

// Table Certificat

/// @brief Table Certificat id SERIAL PRIMARY KEY, patient_id INT REFERENCES patient(id) ON DELETE CASCADE, doctor_id INT REFERENCES doctor(id) ON DELETE SET NULL, created_at DATE, duration INT , cause VARCHAR , requested_by_patient BOOLEAN
/// @param created_at ==> format [MM-DD]
typedef struct
{
    int id;
    int patient_id;
    int doctor_id;
    char created_at[11];
    int duration;
    char cause[50];
    bool requested_by_patient;
    bool approved;
} Certificat;

// Table Ordonnances
typedef struct
{
    int id;
    int patient_id;
    int doctor_id;
    char content[1000];
} Ordonnance;

// Table Appointment
typedef struct
{
    int id;
    bool requested_by_patient;
    int patient_id;
    int doctor_id;
    char date[11];
    bool validity;
} Appointment;

// Table Receive_Service
typedef struct
{
    int id;
    int service_id;
    int patient_id;
} Receive_Service;

#endif