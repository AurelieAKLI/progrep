#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5555

void main(){

        int sockEcoute;
        struct sockaddr_in annuaire;

        int sockAccept;
        struct sockaddr_in adrSommet;

        socklen_t taille_ad;
        char msg[1024];

        sockEcoute = socket(AF_INET, SOCK_STREAM, 0);
        printf("Création socket de l'annuaire réussie.\n");
        memset(&annuaire, '\0', sizeof(annuaire));

        annuaire.sin_family = AF_INET;
        annuaire.sin_port = htons(PORT);
        annuaire.sin_addr.s_addr = inet_addr("127.0.0.1");

        bind(sockEcoute, (struct sockaddr*)&annuaire, sizeof(annuaire));
        printf("Lié au port numero [%d].\n", 4455);

        listen(sockEcoute, 5);
        printf("En écoute...\n");

        sockAccept = accept(sockEcoute, (struct sockaddr*)&adrSommet, &taille_ad);

        strcpy(msg, "Bonjour");

        send(sockAccept, msg, strlen(msg), 0);
        recv(sockAccept, msg, 1024, 0);

        printf("%s\n", msg);
        printf("Fermeture de la connexion.\n");



}
