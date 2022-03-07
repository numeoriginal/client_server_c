#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include "helpers.h"
#include "requests.h"
#include <time.h>

char username[25];
char password[35];
char *snd_msg;
char *rcv_msg;
int sockFd;
char **biscuits;
char command[50];
char *Token;

// functia de inregistrare
void register_foo() {

    char **buffer;
    // prompt pentru input
    printf("username=");
    scanf("%s", username);
    printf("password=");
    scanf("%s", password);

    // formare string
    buffer = calloc(1, sizeof(char *));
    buffer[0] = calloc(50, sizeof(char));
    strcat(buffer[0], "{\n\t\"username\": ");
    strcat(buffer[0], "\"");
    strcat(buffer[0], username);
    strcat(buffer[0], "\"");
    strcat(buffer[0], ",\n\t\"password\": ");
    strcat(buffer[0], "\"");
    strcat(buffer[0], password);
    strcat(buffer[0], "\"");
    strcat(buffer[0], "\n}\0");


    snd_msg = compute_post_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                                   "/api/v1/tema/auth/register",
                                   "application/json", buffer, 1, NULL, 0, NULL);
    //puts(snd_msg);
    send_to_server(sockFd, snd_msg);
    printf("\n");
    rcv_msg = receive_from_server(sockFd);
    //puts(rcv_msg);
    if (strstr(rcv_msg, "error")) { // verificam daca nu am primit erori de la server
        char *err = calloc(100, sizeof(char));
        srand(time(NULL));
        int proprose = rand() / 10000000;
        puts("Username taken.");
        sprintf(err, "Try username %s%d!", username, proprose); // mic sfat pentru utilizator Kappa.
        puts(err);
        printf("--------------------------------------------------------------------------\n");
        free(err);
    } else {
        printf("Registration successful!. Welcome to our server %s.\n", username);
        printf("--------------------------------------------------------------------------\n");
    }
    free(buffer);
}

// functia de logare
void login_foo() {

    // alloc the biscuits jar.
    biscuits = calloc(3, sizeof(char));
    for (int i = 0; i < 3; ++i) {
        biscuits[i] = calloc(100, sizeof(char));
    } // confirm we logged in

    char **buffer;
    // prompt comanda
    printf("username=");
    scanf("%s", username);
    printf("password=");
    scanf("%s", password);

    // formare string
    buffer = calloc(1, sizeof(char *));
    buffer[0] = calloc(50, sizeof(char));
    strcat(buffer[0], "{\n\t\"username\": ");
    strcat(buffer[0], "\"");
    strcat(buffer[0], username);
    strcat(buffer[0], "\"");
    strcat(buffer[0], ",\n\t\"password\": ");
    strcat(buffer[0], "\"");
    strcat(buffer[0], password);
    strcat(buffer[0], "\"");
    strcat(buffer[0], "\n}\0");

    snd_msg = compute_post_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/tema/auth/login",
                                   "application/json", buffer, 1, NULL, 0, NULL);
    send_to_server(sockFd, snd_msg);
    rcv_msg = receive_from_server(sockFd);
    printf("\n");
    // verifica daca datele de login introduse sunt valide
    if (strstr(rcv_msg, "error")) {
        puts("Incorrect credentials!");
        puts("Verify your password or username and try again.");
        printf("--------------------------------------------------------------------------\n");
    } else {
        char *token = strstr(rcv_msg, "Set-Cookie: ");
        token = strstr(token, " ");
        char *util;
        util = strtok(token + 1, ";");
        memset(biscuits[0], 0, 100);
        strcat(biscuits[0], util);
        memset(biscuits[1], 0, 100);
        strcat(biscuits[1], "Path=/");
        memset(biscuits[2], 0, 100);
        strcat(biscuits[2], "HttpOnly");

        char *respond = NULL;
        respond = calloc(50, sizeof(char));
        printf("%s\n", biscuits[0]);
        strcpy(respond, "Welcome back "); // confirmam ca ne-am logat
        strcat(respond, username); // un mesaj de welcome back pentru utilizator
        strcat(respond, "!");
        puts(respond);
        printf("--------------------------------------------------------------------------\n");
        free(respond);
    }
    free(buffer);

}

void enter_library_foo() {

    // alloc token mem and confirm we entered library
    Token = calloc(250, sizeof(char));
    if (biscuits != NULL) { // verifica daca suntem logati

        char *dap = "get";
        snd_msg = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                                      "/api/v1/tema/library/access", NULL, biscuits, 3, dap);

        send_to_server(sockFd, snd_msg);
        rcv_msg = receive_from_server(sockFd);
        // verifica daca am primit erori de la server sau nu
        char *err = strstr(rcv_msg, "error");
        if (err != NULL) {
            puts(basic_extract_json_response(rcv_msg));
        } else {
            char *token = strstr(rcv_msg, "token");
            token = strstr(token, ":");
            char *util;
            util = strtok(token + 1, "\"");
            // construim token-ul
            strcpy(Token, "\nAuthorization: Bearer ");
            strcat(Token, util);
            puts(Token);
            puts("Welcome to our library!");
            printf("--------------------------------------------------------------------------\n");
        }
    } else {
        puts("You are not logged in. Hint use command: login if you have an account ");
        puts("or command: register if you don't have one.");
        printf("--------------------------------------------------------------------------\n");
    }
}

void add_book_foo() {
    if (biscuits != NULL) {
        if (Token != NULL) {
            puts("All fields must be filled.");
            char **buffer;
            char input[20];
            char *cnv;
            long count = 0;
            buffer = calloc(1, sizeof(char *));
            buffer[0] = calloc(1024, sizeof(char));
            // creaza stringul
            strcpy(buffer[0], "{\n\t\"title\" : \"");
            printf("title=");
            scanf("%s", input);

            count = strtol(input, &cnv, 10);
            if (count != 0) {
                printf("\n");
                puts("Invalid input! title not specified or contains numbers.");
                printf("--------------------------------------------------------------------------\n");
                return;
            }

            strcat(buffer[0], "\",""\n\t\"author\": \"");
            printf("author=");
            scanf("%s", input); // autorul
            strcat(buffer[0], input);

            count = strtol(input, &cnv, 10);
            if (count != 0) {
                printf("\n");
                puts("Invalid input! author not specified or contains numbers.");
                printf("--------------------------------------------------------------------------\n");
                return;
            }

            strcat(buffer[0], "\",""\n\t\"genre\": \"");
            printf("genre=");
            scanf("%s", input); // genul
            strcat(buffer[0], input);

            count = strtol(input, &cnv, 10);
            if (count != 0) {
                printf("\n");
                puts("Invalid input! genre not specified or contains numbers.");
                printf("--------------------------------------------------------------------------\n");
                return;
            }
            strcat(buffer[0], "\",""\n\t\"page_count\": \"");
            printf("page_count=");
            scanf("%s", input); // nr pagini
            count = strtol(input, &cnv, 10);
            if (count == 0) {
                printf("\n");
                puts("Invalid input! page_count not specified or not a number.");
                printf("--------------------------------------------------------------------------\n");
                return;
            }
            sprintf(buffer[0] + strlen(buffer[0]), "%ld", count);
            strcat(buffer[0], "\",""\n\t\"publisher\": \"");
            printf("publisher=");
            scanf("%s", input); // publisher
            strcat(buffer[0], input);
            strcat(buffer[0], "\"\n}");
            count = strtol(input, &cnv, 10);
            if (count != 0) {
                printf("\n");
                puts("Invalid input! publisher not specified or contains numbers.");
                printf("--------------------------------------------------------------------------\n");
                return;
            }

            snd_msg = compute_post_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                                           "/api/v1/tema/library/books",
                                           "application/json", buffer, 1, NULL, 0, Token);
            //puts(snd_msg);
            send_to_server(sockFd, snd_msg);
            rcv_msg = receive_from_server(sockFd);
            //puts(rcv_msg);
            // verifica daca nu am primit vreo eroare de la server
            char *err = strstr(rcv_msg, "error");
            if (err == NULL) {
                printf("\n");
                puts("Book added successfully!");
                printf("--------------------------------------------------------------------------\n");
            } else puts(basic_extract_json_response(rcv_msg));
            free(buffer);
        } else {
            puts("You are not in the library. Hint: use command enter_library");
            printf("--------------------------------------------------------------------------\n");
        }
    } else {
        puts("You are not logged in. Hint use command: login if you have an account ");
        puts("or command: register if you don't have one.");
        printf("--------------------------------------------------------------------------\n");
    }
}

void delete_get_book_foo(char *token) {

    if (biscuits != NULL) {

        if (Token != NULL) {
            int id;
            printf("id=");
            scanf("%d", &id);
            printf("\n");
            char *addr;
            char url[50] = "/api/v1/tema/library/books/";
            addr = calloc(128, sizeof(char));
            strcpy(addr, url);

            int length = snprintf(NULL, 0, "%d", id);
            char *str = malloc(length + 1);
            snprintf(str, length + 1, "%d", id);
            strcat(addr, str);
            free(str);
            // to do fct delete
            snd_msg = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                                          addr, Token, NULL, 0, token);
            send_to_server(sockFd, snd_msg);
            rcv_msg = receive_from_server(sockFd);
            char *err = strstr(rcv_msg, "error");
            if (err == NULL && strcmp(token, "delete") == 0) {
                puts("Book vanished in the abyss.");
                printf("--------------------------------------------------------------------------\n");
            } else if (strcmp(token, "delete") == 0) {
                puts("No such book in our library.");
                printf("--------------------------------------------------------------------------\n");
            }
            if (err == NULL && strcmp(token, "get") == 0) {
                puts(basic_extract_json_response(rcv_msg));
            } else if (strcmp(token, "get") == 0) {
                puts("No such book in our library.");
                printf("--------------------------------------------------------------------------\n");
            }
            free(addr);
        } else {
            puts("You are not in the library. Hint: use command enter_library");
            printf("--------------------------------------------------------------------------\n");
        }
    } else {
        puts("You are not logged in. Hint use command: login if you have an account ");
        puts("or command: register if you don't have one.");
        printf("--------------------------------------------------------------------------\n");
    }
}

void get_books_foo() {

    if (biscuits != NULL) {
        if (Token != NULL) {
            char dap[10] = "get";
            snd_msg = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                                          "/api/v1/tema/library/books", Token, biscuits, 3, dap);

            send_to_server(sockFd, snd_msg);
            rcv_msg = receive_from_server(sockFd);

            char *err = strstr(rcv_msg, "error");
            if (err == NULL) {
                if (basic_extract_json_response(rcv_msg) == NULL) {
                    puts("No books available at the moment, maybe you can add one!");
                    printf("--------------------------------------------------------------------------\n");
                } else
                    printf("Available books: %s\n", basic_extract_json_response(rcv_msg));
                printf("--------------------------------------------------------------------------\n");


            } else {
                puts(basic_extract_json_response(rcv_msg));
                printf("--------------------------------------------------------------------------\n");
            }
        } else {
            puts("You are not in the library. Hint: use command enter_library");
            printf("--------------------------------------------------------------------------\n");
        }
    } else {
        puts("You are not logged in. Hint use command: login if you have an account ");
        puts("or command: register if you don't have one");
        printf("--------------------------------------------------------------------------\n");
    }
}

void logout_foo() {
    if (biscuits != NULL) { // verifica daca suntem logati
        char dap[10] = "get";
        // creaza mesajul
        snd_msg = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                                      "/api/v1/tema/auth/logout", NULL, biscuits, 3, dap);
        // trimite mesajul
        send_to_server(sockFd, snd_msg);
        // primeste mesajul de la server
        rcv_msg = receive_from_server(sockFd);

        char *err = strstr(rcv_msg, "error");
        // verifica daca delogarea a avut loc cu succes sau nu .
        if (err == NULL) {
            puts("You logged out successfully!");
            printf("--------------------------------------------------------------------------\n");
            free(biscuits);
            biscuits = NULL;
        } else {
            puts("Something went wrong!");
            printf("--------------------------------------------------------------------------\n");
        }
    } else {
        puts("Your are not logged in.");
        printf("--------------------------------------------------------------------------\n");
    }
}

int main(int argc, char *argv[]) {

    char commands[200] = "register, login, enter_library.";
    char library[75] = "add_book, get_book, get_books, delete_book.";

    for (;;) {
        sockFd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
        memset(command, 0, sizeof(command));
        if (Token == NULL) {
            if (biscuits != NULL) {
                printf("Logged in as  : %s.\n", username);
            }
            puts("Valid commands:");
            puts(commands);
            puts("For more info type command: info.");
        } else {
            puts("You are in our library.");
            puts("Valid commands in library:");
            puts(library);
            puts("For more info type command: info");
        }

        puts("Enter command:");
        scanf("%s", command);

        if (strcmp(command, "exit") == 0) {
            puts("See you soon.");
            free(Token);
            free(biscuits);
            snd_msg = NULL;
            rcv_msg = NULL;
            free(biscuits);
            biscuits = NULL;
            // inchide conexiunea cu serverul.
            close(sockFd);
            // inchide programul.
            return 0;
        } else if (strcmp(command, "register") == 0) {
            if (biscuits == NULL) {
                // functia pentru inregistrare
                register_foo();

            } else {
                puts("You must logout first.");
                printf("--------------------------------------------------------------------------\n");
            }
        } else if (strcmp(command, "login") == 0) {
            if (biscuits == NULL) {
                // fctia de login
                login_foo();
            } else {
                puts("You must logout first.");
                printf("--------------------------------------------------------------------------\n");
            }
        } else if (strcmp(command, "enter_library") == 0) {
            // fctia accesare librarie
            enter_library_foo();
        } else if (strcmp(command, "add_book") == 0) {
            // functia pentru adaugare de carti in librarie
            add_book_foo();
        } else if (strcmp(command, "delete_book") == 0) {
            // functia pentru stergerea unei carti din librarie
            char token[10] = "delete";
            delete_get_book_foo(token);
        } else if (strcmp(command, "get_books") == 0) {
            printf("\n");
            // functia pentru printare a unei carti specifice
            get_books_foo();
        } else if (strcmp(command, "get_book") == 0) {
            // functia pentru printare a catalogului de carti
            char token[10] = "get";
            delete_get_book_foo(token);
        } else if (strcmp(command, "logout") == 0) {
            // functia pentru delogare
            logout_foo();
        } else if (strcmp(command, "info") == 0) {
            printf("\n");
            if (Token == NULL) {
                puts("Command: register -- creates and account.");
                puts("Command: login -- logs you into an existing account.");
                puts("Command: enter_library -- gives you access to the library.");
                printf("--------------------------------------------------------------------------\n");
            } else {
                puts("Command: add_book -- adds a book to our library.");
                puts("Command: delete_book -- deletes a book from our library.");
                puts("Command: get_book -- shows you a book from our library.");
                puts("Command: get_books -- shows you all books from our library.");
                printf("--------------------------------------------------------------------------\n");
            }
        } else {
            puts("Please enter a valid command."); // in case u cant type
            printf("--------------------------------------------------------------------------\n");
        }
    }
}
