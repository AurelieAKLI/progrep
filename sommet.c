#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5555
#define VAL(str) #str
#define TOSTRING(str) VAL(str)

void main(){

        int socketSommet;
        struct sockaddr_in adrAnnuaire;
        char msg[1024];

        socketSommet = socket(PF_INET, SOCK_STREAM, 0);
        printf("Création socket du sommet réussie.\n");

        memset(&adrAnnuaire, '\0', sizeof(adrAnnuaire));
        adrAnnuaire.sin_family = AF_INET;
        adrAnnuaire.sin_port = htons(PORT);
        adrAnnuaire.sin_addr.s_addr = inet_addr("127.0.0.1");

        connect(socketSommet, (struct sockaddr*)&adrAnnuaire, sizeof(adrAnnuaire));
        printf("Connecté à l'annuaire.\n");

        recv(socketSommet, msg, 1024, 0);
        printf("Message reçu de l'annuaire: %s\n", msg);

        strcpy(msg, TOSTRING(PORT));

        send(socketSommet, msg, strlen(msg), 0);
        printf("Fermeture connexion.\n");

}
