#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include "tresor.h"
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "tresor.h"

#define N 10
#define PORT_NUMBER 5555


int main(int argc, char **argv) {
        int domaine=AF_INET;
        int type=SOCK_STREAM;
        int protocole=0;

        struct sommet *liste_s;
        liste_s=malloc(sizeof(sommet)*(nbSommets-1));


        //retourne un entier, qui identifiera la socket créée dans le reste du programme.
        int sid = socket(domaine,type, protocole);
        if (sid == -1) {
                perror("Socket creation error");
                return 1;
        }

        //association du socket
        struct sockaddr_in serv_dest;
        memset(&serv_dest, 0, sizeof(serv_dest));
        serv_dest.sin_family = domaine;
        serv_dest.sin_port = htons(PORT_NUMBER);
        serv_dest.sin_addr.s_addr = inet_addr("127.0.0.1");


        if (bind(sid, (struct sockaddr*) &serv_dest, sizeof(serv_dest))==-1) {
                perror("Bind error");
                close(sid);
                return 2;
        }

        //serveur mis sur écoute
        if (listen(sid, 15)==-1){ //jusqu'à 15 clients dans la fille d'attente
                perror("Listen error");
                close(sid);
                return 3;
        }

        struct sockaddr_in addrdist;
        socklen_t addr_size;
        int sid_bis;

        for (int i=0; i<nbSommets;++i){
                sid_bis = accept(sid, (struct sockaddr *) &addrdist, &addr_size);
                if (sid_bis < 0) {
                        perror("accept failed");
                        close(sid_bis);
                        return 4;
                }
                //reception coordonnees du client
                recv(sid_bis, &message_in, 4,0);
        }

        for (int i=0; i<nbSommets;++i){
                send(sid_bis, liste_s, 1,0);

        }
        close(sid_bis);

        return 0;



}


