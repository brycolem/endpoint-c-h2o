#include "application_service.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_application_data_json(PGconn *conn)
{
    int application_count = 0;
    Application *applications = get_applications(conn, &application_count);

    if (!applications)
    {
        return strdup("{\"error\": \"Failed to retrieve applications\"}");
    }

    char *json = (char *)malloc(1024 * application_count);
    strcpy(json, "{\"applications\": [");

    for (int i = 0; i < application_count; i++)
    {
        char app_json[512];
        sprintf(app_json, "{\"id\": %d, \"title\": \"%s\", \"employer\": \"%s\", \"link\": \"%s\", \"companyId\": %d}",
                applications[i].id, applications[i].title, applications[i].employer,
                applications[i].link, applications[i].companyId);
        strcat(json, app_json);

        if (i != application_count - 1)
        {
            strcat(json, ",");
        }
    }

    strcat(json, "]}");

    free(applications);
    return json;
}
