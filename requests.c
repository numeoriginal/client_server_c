#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *query_params,
                          char **cookies, int cookies_count,char * util) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    if (strcmp(util,"delete") == 0){
        if (query_params != NULL) {
            sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
        } else {
            sprintf(line, "DELETE %s HTTP/1.1", url);
        }
    } else {
        if (query_params != NULL) {
            sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
        } else {
            sprintf(line, "GET %s HTTP/1.1", url);
        }
    }

    compute_message(message, line);

    // Step 2: add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    char cok[500];
    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookies != NULL) {
        strcpy(cok, "Cookie: ");
        strcat(cok, cookies[0]);
        strcat(cok, "; ");
        for (int i = 1; i < cookies_count; ++i) {
            if (i == cookies_count - 1) {
                strcat(cok, cookies[i]);
                compute_message(message, cok);
                continue;
            }
            strcat(cok, cookies[i]);
            strcat(cok, "; ");
        }
    }
    // Step 4: add final new line
    compute_message(message, "");
    return message;
}

char *compute_post_request(char *host, char *url, char *content_type, char **body_data,
                           int body_data_fields_count, char **cookies, int cookies_count, char *token) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    char *body_data_buffer = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL and protocol type

    if (token != NULL) {
        sprintf(line, "POST %s?%s HTTP/1.1", url, token);
    } else {
        sprintf(line, "POST %s HTTP/1.1", url);
    }
    compute_message(message, line);
    // Step 2: add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
    sprintf(line, "Content-Type: %s", content_type);

    compute_message(message, line);
    int content_length = 0;
    for (int j = 0; j < body_data_fields_count; ++j) {
        content_length += strlen(body_data[j]);
    }
    content_length += body_data_fields_count - 1;
    sprintf(line, "Content-Length: %d", content_length);
    compute_message(message, line);


    // Step 4 (optional): add cookies
    char cok[500];
    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookies != NULL) {
        strcpy(cok, "Set-Cookie: ");
        strcat(cok, cookies[0]);
        strcat(cok, "; ");
        for (int i = 1; i < cookies_count; ++i) {
            if (i == cookies_count - 1) {
                strcat(cok, cookies[i]);
                compute_message(message, cok);
                continue;
            }
            strcat(cok, cookies[i]);
            strcat(cok, "; ");
        }
    }
    // Step 5: add new line at end of header
    compute_message(message, "");

    // Step 6: add the actual payload data
    memset(line, 0, LINELEN);
    for (int k = 0; k < body_data_fields_count; ++k) {
        if (k == body_data_fields_count - 1) {
            strcat(body_data_buffer, body_data[k]);
            continue;
        }
        strcat(body_data_buffer, body_data[k]);
        strcat(body_data_buffer, "&");
    }
    compute_message(message, body_data_buffer);

    free(line);
    return message;
}