
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>  // Pour htonl(), htons()

int main()
{
    int sd;
    char msg[100];
    struct sockaddr_in adresse_srv;

    // Création du socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        printf("Erreur de création du socket\n");
        return 1;
    }

    // Configuration de l'adresse du serveur
    adresse_srv.sin_family = AF_INET;
    adresse_srv.sin_port = htons(1230);  // Utiliser htons pour le port
    adresse_srv.sin_addr.s_addr = inet_addr("127.0.0.1");  // Connexion au serveur local

    // Connexion au serveur
    if (connect(sd, (struct sockaddr *)&adresse_srv, sizeof(adresse_srv)) < 0) {
        printf("Erreur de connexion\n");
        close(sd);  // Fermer le socket en cas d'erreur
        return 1;
    }

    // Demander un message à l'utilisateur
    printf("Donnez votre message : ");
    fgets(msg, sizeof(msg), stdin);  // Utiliser fgets au lieu de gets pour éviter les risques de buffer overflow

    // Envoi du message au serveur
    send(sd, msg, strlen(msg), 0);

    // Fermeture du socket
    close(sd);

    return 0;
}
