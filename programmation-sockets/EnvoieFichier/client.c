#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 1500
#define BUFFER_SIZE 200

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char file_name[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    FILE *file;

    // Création de la socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Adresse locale

    // Connexion au serveur
    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Demander le nom du fichier
    printf("Entrez le nom du fichier à envoyer : ");
    scanf("%s", file_name);

    // Ouvrir le fichier en mode lecture
    file = fopen(file_name, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Lire et envoyer les lignes du fichier
    while (fgets(buffer, sizeof(buffer), file)) {
        send(sock, buffer, sizeof(buffer), 0);
    }

    // Envoyer "EOF" pour indiquer la fin du fichier
    strcpy(buffer, "EOF");
    send(sock, buffer, sizeof(buffer), 0);

    // Fermer le fichier et la connexion
    fclose(file);
    close(sock);

    printf("Fichier envoyé avec succès.\n");

    return 0;
}
