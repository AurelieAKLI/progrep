#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define N 10
#define PORT_NUMBER 0

char *message_in="le sommet vers annuaire\n";
char *message_out="l'annuaire vers sommet\n";

int main(int argc, char **argv) {
        int domaine=AF_INET;
        int type=SOCK_STREAM;
        int protocole=0;
        printf("1\n");

        int sid_ecoute = socket(domaine, type, protocole);
        if (sid_ecoute == -1) {
                perror("Socket creation error");
                return 1;
        }
        printf("2\n");

        struct sockaddr_in serv_sommet;
        //demande de connexion au serveur
        memset(&serv_sommet, 0, sizeof(serv_sommet));
        serv_sommet.sin_family = domaine;
        serv_sommet.sin_port = htons(PORT_NUMBER);
        serv_sommet.sin_addr.s_addr = htonl(INADDR_ANY);;

        socklen_t ad=sizeof(serv_sommet);
        printf("3\n");

        if (bind(sid_ecoute, (struct sockaddr*) &serv_sommet, ad )==-1) {
                perror("Bind error");
                close(sid_ecoute);
                return 2;
        }

        //inet_pton(domaine, "127.0.0.1", &(serv_sommet.sin_addr));
        printf("4\n");

        if (listen(sid_ecoute, 1)==-1){ //jusqu'à 15 clients dans la fille d'attente
                perror("Listen error");
                close(sid_ecoute);
                return 3;
        }
        printf("5\n");


        if ( accept(sid_ecoute, (struct sockaddr *) &serv_sommet, &ad)== -1){
                perror("error accept");
                return 4;
        }

        printf("6\n");

        while (1){
                printf("7\n");
                recv(sid_ecoute, &message_in, 4,0);
                send(sid_ecoute, "c'est l'annuaire", 4, 0);

        }
  
        return 0;


}
