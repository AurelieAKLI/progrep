#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT_NUMBER 0

/*
char message_out[8];
char message_in[8];
*/
char *message_out="le sommet vers annuaire\n";
char *message_in="l'annuaire vers sommet\n";




int main(int argc, char **argv) {
        printf("1\n");
        //printf("%s", message_in);

        if (argc!=2){//il faut compiler avec le numero du sommet
                fprintf(stderr, "Erreur : ./sommet [numeroSommet]\n");
                return 1;
        }

        message_out= argv[1];

        int domaine = AF_INET;
        int type = SOCK_STREAM;
        int protocole = 0;
        printf("1\n");

        int sid_envoie = socket(domaine, type, protocole);
        if (sid_envoie == -1) {
                perror("Socket creation error");
                return 2;
        }
        printf("2\n");

        struct sockaddr_in serv_annuaire;
        //demande de connexion au serveur
        memset(&serv_annuaire, 0, sizeof(serv_annuaire));
        serv_annuaire.sin_family = domaine;
        serv_annuaire.sin_port = htons(PORT_NUMBER);
        inet_pton(domaine, "127.0.0.1", &(serv_annuaire.sin_addr));

        printf("3\n");

        socklen_t ad=sizeof(serv_annuaire);
        if (connect(sid_envoie, (struct sockaddr*) &serv_annuaire, ad ) == -1) {
                perror("Connection error");
                close(sid_envoie);
                return 3;
        }
        printf("5\n");

        send(sid_envoie, &message_out, 8, 0);
        recv(sid_envoie, &message_in, 8, 0);

        close (sid_envoie);

        return 0;


}
