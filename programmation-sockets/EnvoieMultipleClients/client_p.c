#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Créer un socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Erreur de création du socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Se connecter au serveur
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erreur de connexion");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // Lire et afficher le message de bienvenue
    int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("%s", buffer);
    }

    // Envoyer des messages au serveur
    while (1) {
        printf("Entrez un message: ");
        fgets(buffer, sizeof(buffer), stdin);

        // Quitter si l'utilisateur tape "exit"
        if (strncmp(buffer, "exit", 4) == 0) {
            break;
        }

        send(client_fd, buffer, strlen(buffer), 0);

        // Attendre la réponse du serveur
        bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("Réponse du serveur: %s\n", buffer);
        }
    }

    close(client_fd);
    return 0;
}
