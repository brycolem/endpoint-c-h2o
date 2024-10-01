#ifndef APPLICATION_SERVICE_H
#define APPLICATION_SERVICE_H

#include "../model/application_model.h"
#include <libpq-fe.h>

char *get_application_data_json(PGconn *conn);

#endif
