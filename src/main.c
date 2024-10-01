#include <h2o.h>
#include <uv.h>
#include <stdio.h>
#include "controller/application_controller.h"
#include "controller/network_controller.h"

int main(int argc, char **argv)
{
    h2o_hostconf_t *hostconf;
    h2o_globalconf_t config;
    h2o_context_t ctx;

    h2o_config_init(&config);

    hostconf = h2o_config_register_host(&config, h2o_iovec_init(H2O_STRLIT("default")), 65535);

    register_routes(hostconf);

    start_server(&ctx, &config);

    h2o_config_dispose(&config);

    return 0;
}
