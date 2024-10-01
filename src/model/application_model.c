#include "application_model.h"
#include <stdlib.h>
#include <string.h>

Application *get_applications(PGconn *conn, int *application_count)
{
    const char *query = "SELECT id, title, employer, link, company_id FROM applications;";
    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        PQclear(res);
        return NULL;
    }

    *application_count = PQntuples(res);
    Application *applications = (Application *)malloc(*application_count * sizeof(Application));

    for (int i = 0; i < *application_count; i++)
    {
        applications[i].id = atoi(PQgetvalue(res, i, 0));
        strncpy(applications[i].title, PQgetvalue(res, i, 1), 256);
        strncpy(applications[i].employer, PQgetvalue(res, i, 2), 256);
        strncpy(applications[i].link, PQgetvalue(res, i, 3), 256);
        applications[i].companyId = atoi(PQgetvalue(res, i, 4));
    }

    PQclear(res);
    return applications;
}
