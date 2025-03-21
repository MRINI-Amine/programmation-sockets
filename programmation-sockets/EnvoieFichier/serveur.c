#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>



int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[200];

    // Création de la socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1500);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Liaison de la socket
    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Écoute des connexions
    listen(server_sock, 5);
    printf("Serveur en attente de connexion...\n");

    // Accepter une connexion client
    addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
    printf("Client connecté.\n");

    // Réception et affichage des lignes du fichier
    while (1) {
        recv(client_sock, buffer, sizeof(buffer), 0);
        if (strcmp(buffer, "EOF") == 0) break;  // Fin du fichier
        printf("Ligne reçue : %s", buffer);
    }

    // Fermeture de la connexion
    close(client_sock);
    close(server_sock);

    return 0;
}
