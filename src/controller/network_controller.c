#include "network_controller.h"
#include <h2o.h>
#include <uv.h>
#include <stdio.h>
#include <sys/socket.h>

static void on_new_connection(uv_poll_t *req, int status, int events)
{
    uv_os_fd_t server_fd;
    uv_fileno((uv_handle_t *)req, &server_fd);

    if (status < 0)
    {
        fprintf(stderr, "Error on listening socket: %s\n", uv_strerror(status));
        return;
    }

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0)
    {
        perror("Failed to accept client connection");
        return;
    }

    h2o_socket_t *sock = h2o_uv_socket_create((uv_handle_t *)req, NULL);
    if (sock == NULL)
    {
        close(client_fd);
        return;
    }

    h2o_accept_ctx_t *accept_ctx = req->data;
    h2o_accept(accept_ctx, sock);
}

void start_server(h2o_context_t *ctx, h2o_globalconf_t *config)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8081);
    addr.sin_addr.s_addr = INADDR_ANY;

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1)
    {
        perror("Failed to create socket");
        return;
    }

    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) != 0)
    {
        perror("Failed to bind");
        return;
    }

    if (listen(listen_fd, 128) != 0)
    {
        perror("Failed to listen");
        return;
    }

    uv_poll_t *poll_handle = malloc(sizeof(uv_poll_t));
    uv_poll_init(ctx->loop, poll_handle, listen_fd);

    poll_handle->data = &config->hosts;

    uv_poll_start(poll_handle, UV_READABLE, on_new_connection);

    printf("Listening on port 8081...\n");
}
