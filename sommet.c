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

int main(int argc, char **argv) {
        int domaine=AF_INET;
        int type=SOCK_STREAM;
        int protocole=0
                ;
        int nbSommets= atoi(argv[1]);

        struct sommet s;
        struct sommet *liste_s;
        liste_s=malloc(sizeof(sommet)*(nbSommets-1));

        //retourne un entier, qui identifiera la socket créée dans le reste du programme.
        int sid = socket(domaine,type, protocole);
        if (sid == -1) {
                perror("Socket creation error");
                return 1;
        }

        struct sockaddr_in serv_dest;
        //demande de connexion au serveur
        memset(&serv_dest, 0, sizeof(serv_dest));
        serv_dest.sin_family = domaine;
        serv_dest.sin_port = htons(PORT_NUMBER);

        inet_pton(domaine, "146.59.237.169", &(serv_dest.sin_addr));

        if (connect(sid, (struct sockaddr*) &serv_dest,sizeof(serv_dest)) == -1) {
                perror("Connection error");
                close(sid);
                return EXIT_FAILURE;
        }

        //envoie message au serveur
        send(sid, &s, 8, 0);

        //reception message du serveur
        recv(sid, &liste_s, 8, 0);

        close(sid);
