#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 1230
#define BUFFER_SIZE 100

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Création du socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket error");
        return 1;
    }

    // Configuration du serveur
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Liaison du socket
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Écoute des connexions
    listen(server_fd, 5);
    printf("En attente de connexion...\n");

    // Acceptation d'un client
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    printf("Client connecté.\n");

    // Réception du message
    recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    printf("Message reçu: %s\n", buffer);

    // Fermeture des sockets
    close(client_fd);
    close(server_fd);

    return 0;
}
