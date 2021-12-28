#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define LENGTH 2048
#define MAX_SOMMETS 50

// Global variables
volatile sig_atomic_t flag = 0;
int sockEnvoie = 0;
char liste[MAX_SOMMETS][MAX_SOMMETS];
int compteur=0;
int compteur2;
struct sockaddr_in adr_Ecoute;
int sockEcoute;
char *numSommet;


void str_overwrite_stdout() {
        printf("%s", "> ");
        fflush(stdout);
}


void catch_ctrl_c_and_exit(int sig) {
        flag = 1;
}
/*
void send_msg_handler() {
        char message[LENGTH] = {};
        char buffer[LENGTH + 32] = {};

        while(1) {

                        sprintf(buffer, "%s: %d\n", name, gethostname(ad_port,100));
                        send(sockfd, buffer, strlen(buffer), 0);
                }

        }
        catch_ctrl_c_and_exit(2);
}*/

void recv_msg_handler() {
        char message[LENGTH] = {};
        while (1) {
                int recu = recv(sock, message, LENGTH, 0);
                if (recu > 0) {
                        strcpy(liste[compteur],message);
                        printf("%s\n",liste[compteur]);
                        compteur++;
                        //str_overwrite_stdout();
                } else if (recu == 0) {
                        break;
                } else {
                        perror("Probleme recv");
                }

        }
}

int main(int argc, char **argv){
        if(argc != 3){
                printf("Usage: %s <num_port> <numSommet>\n", argv[0]);
                return EXIT_FAILURE;
        }
        
        int numSommet=atoi(argv[2]);

        char *adr_ip = "127.0.0.1";
        int num_port = atoi(argv[1]);

        signal(SIGINT, catch_ctrl_c_and_exit);

        struct sockaddr_in adr_annuaire;

        //Paramètres socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        adr_annuaire.sin_family = AF_INET;
        adr_annuaire.sin_addr.s_addr = inet_addr(adr_ip);
        adr_annuaire.sin_port = htons(num_port);


        // Connexion à l'annuaire
        int err = connect(sock, (struct sockaddr *)&adr_annuaire, sizeof(adr_annuaire));
        if (err == -1) {
                perror("Problème connexion");
                return EXIT_FAILURE;
        }


        pthread_t recv_msg_thread;
        if(pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0){
                perror("Problème création thread");
                return EXIT_FAILURE;
        }
        //les numéros de ports affichés dans le client doivent être enrigistrés 
        //Créer un nouveau socket qui va écrire sur les numéros de port qu'on a pu afficher sur le client et qui va se connecter sur les autres
        // le client reçois des numéros de sommets de ses voisins et compare avec sa valeur 
        


        while (1){
                if(flag){
                        printf("\nFin\n");
                        break;
                }
        }
        pthread_join(recv_msg_thread, NULL);
        
        sockEcoute=socket(AF_INET, SOCK_STREAM, 0);

        adr_Ecoute.sin_family= AF_INET;
        adr_Ecoute.sin_addr.s_addr= inet_addr(adr_ip);

        pthread_t idEcriture[MAX_SOMMETS];
        for(int i=0; i<compteur;++i){
                if(pthread_create(&idEcriture, NULL, (void *) liaison, NULL) != 0){
                        perror("Problème création thread");
                        return EXIT_FAILURE;
                }
        }

        for(int i=0; i<compteur; ++i){
                pthread_join(idEcriture, NULL);

        }
        close(sockEcoute);
        close(sock);

        return EXIT_SUCCESS;
}
