#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Créer un socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Erreur de création du socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Lier le socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erreur de bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Écouter les connexions
    if (listen(server_fd, 5) == -1) {
        perror("Erreur de listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Serveur en écoute sur le port %d...\n", PORT);

    // Accepter les connexions des clients
    while (1) {
        client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("Erreur d'acceptation");
            continue;
        }

        printf("Client connecté\n");

        // Créer un processus pour gérer le client
        if (fork() == 0) {
            // Dans le processus enfant, on gère le client
            close(server_fd);  // Fermeture du socket serveur dans le processus enfant

            // Envoyer un message de bienvenue
            char *welcome_message = "Bienvenue sur le serveur !\n";
            send(client_socket, welcome_message, strlen(welcome_message), 0);

            // Recevoir et renvoyer les messages du client
            while (1) {
                int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
                if (bytes_received <= 0) {
                    break;
                }

                buffer[bytes_received] = '\0';
                printf("Message du client: %s\n", buffer);

                // Répondre au client (echo)
                send(client_socket, buffer, bytes_received, 0);
            }

            close(client_socket);
            printf("Client déconnecté\n");
            exit(0);  // Terminer le processus enfant
        } else {
            close(client_socket);  // Le parent ferme la connexion du client pour continuer à accepter d'autres clients
        }
    }

    close(server_fd);
    return 0;
}
