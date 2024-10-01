#ifndef APPLICATION_MODEL_H
#define APPLICATION_MODEL_H

#include <libpq-fe.h>

typedef struct
{
    int id;
    char title[256];
    char employer[256];
    char link[256];
    int companyId;
} Application;

Application *get_applications(PGconn *conn, int *application_count);

#endif
