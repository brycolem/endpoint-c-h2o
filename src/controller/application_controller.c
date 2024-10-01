#include "../service/application_service.h"
#include "application_controller.h"
#include <h2o.h>
#include <libpq-fe.h>

static int application_handler(h2o_handler_t *self, h2o_req_t *req)
{
    PGconn *conn = PQconnectdb("dbname=bench user=brycolem password=your_password");

    if (PQstatus(conn) == CONNECTION_BAD)
    {
        h2o_send_error_503(req, "Service Unavailable", "Database connection failed", 0);
        PQfinish(conn);
        return 0;
    }

    char *json_data = get_application_data_json(conn);
    PQfinish(conn);

    h2o_iovec_t body = h2o_strdup(&req->pool, json_data, SIZE_MAX);
    free(json_data);

    req->res.status = 200;
    req->res.reason = "OK";
    h2o_add_header(&req->pool, &req->res.headers, H2O_TOKEN_CONTENT_TYPE, NULL, H2O_STRLIT("application/json"));
    h2o_send(req, &body, 1, H2O_SEND_STATE_FINAL);

    return 0;
}

void register_routes(h2o_hostconf_t *hostconf)
{
    h2o_pathconf_t *pathconf;
    pathconf = h2o_config_register_path(hostconf, "/application", 0);
    h2o_create_handler(pathconf, sizeof(*pathconf))->on_req = application_handler;
}
