#ifndef ERROR_PARSE_H
#define ERROR_PARSE_H

#define ERROR_PARSING -15
#define WRONG_FORMAT -16
#define WRONG_FORMAT_ROOT_NODE -19
#define WRONG_FORMAT_FIRST_CHILD -22
#define EMPTY_DOC -17
#define NO_MINLAT_ATTRIBUT_DEFINED -23
#define NO_MAXLAT_ATTRIBUT_DEFINED -24
#define NO_MINLON_ATTRIBUT_DEFINED -25
#define NO_MAXLON_ATTRIBUT_DEFINED -26
#define NO_ID_ATTRIBUT_DEFINED -12
#define NO_REF_ATTRIBUT_DEFINED -14
#define NO_LON_ATTRIBUT_DEFINED -29
#define NO_LAT_ATTRIBUT_DEFINED -28
#define NO_KEY_ATTRIBUT_DEFINED -30
#define NO_VALUE_ATTRIBUT_DEFINED -35

/* Définition du type d'erreur */
typedef int error;


/* Retourne la derniere erreur de parsing */
char * getErrorParsing();

#endif
