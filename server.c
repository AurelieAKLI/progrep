#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#define MAX_SOMMETS 100

static _Atomic unsigned int compteur_sommets = 0;
static int id_s = 10;

/* on stocke les coordonnees du sommet */
typedef struct{
    struct sockaddr_in addresse;
    int sockfd;
    int id_s;
} sommet_t;

sommet_t *sommets[MAX_SOMMETS];

int nbSommets; //nombre de sommets dans le graphe renseigné par l'utilisateur
char liste[MAX_SOMMETS][MAX_SOMMETS];

pthread_mutex_t sommets_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Add sommets to queue */
void ajout_sommet(sommet_t *som){
        pthread_mutex_lock(&sommets_mutex);

        for(int i=0; i < MAX_SOMMETS; ++i){
                if(!sommets[i]){
                        sommets[i] = som;
                        break;
                }
        }

        pthread_mutex_unlock(&sommets_mutex);
}


/* Send message to all sommets except sender */
void envoie_message(char *s, int id_s){
        pthread_mutex_lock(&sommets_mutex);

        for(int i=0; i<MAX_SOMMETS; ++i){
                if(sommets[i]){
                        if(sommets[i]->id_s != id_s){
                                if(write(sommets[i]->sockfd, s, strlen(s)) < 0){
                                        perror("Erreur descripteur");
                                        break;
                                }
                        }
                }
        }

        pthread_mutex_unlock(&sommets_mutex);
}

/* Handle all communication with the client */
void *fonction_sommet(void *arg){
        char port[32];

        sommet_t *som = (sommet_t *)arg;
        printf("id_s et sockfd dans handle : %d\t%d\n", som->id_s, som->sockfd);



        if(recv(som->sockfd, port, 32, 0) <= 0){
                printf("Probleme reception numero de port.\n");
                compteur_sommets++;

        } else{
                strcpy(liste[compteur_sommets], port);
                printf("Numero de port bien reçu %s\n", liste[compteur_sommets]);
                compteur_sommets++;
        }
        int j=0;
        while(compteur_sommets!=nbSommets){
        }
        for (int (i) = 0; (i) <nbSommets ; ++(i)) {
                envoie_message(liste[i], som->id_s);
        }


        /* Delete client from queue and yield thread */
        close(som->sockfd);
        free(som);
        pthread_detach(pthread_self());

        return NULL;
}

int main(int argc, char **argv){
        if(argc != 2){
                printf("Usage: %s <nbSommets>\n", argv[0]);
                return 1;
        }

        nbSommets=atoi(argv[1]);

        char *ip = "127.0.0.1";
        int port = 5555;
        int sockEcoute = 0, connexion_acc = 0;
        struct sockaddr_in adr_annuaire;
        struct sockaddr_in adr_sommet;
        pthread_t id_thread;

        /* Socket settings */
        sockEcoute = socket(AF_INET, SOCK_STREAM, 0);
        adr_annuaire.sin_family = AF_INET;
        adr_annuaire.sin_addr.s_addr = inet_addr(ip);
        adr_annuaire.sin_port = htons(port);

        /* Ignore pipe signals */
        signal(SIGPIPE, SIG_IGN);


        /* Bind */
        if(bind(sockEcoute, (struct sockaddr*)&adr_annuaire, sizeof(adr_annuaire)) < 0) {
                perror("ERROR: Socket binding failed");
                return 2;
        }

        /* Listen */
        if (listen(sockEcoute, 10) < 0) {
                perror("ERROR: Socket listening failed");
                return 3;
        }


        while(nbSommets!=compteur_sommets){
                socklen_t lgr_som = sizeof(adr_sommet);
                connexion_acc = accept(sockEcoute, (struct sockaddr*)&adr_sommet, &lgr_som);

                /* Check if max sommets is reached */


                /* Client settings */
                sommet_t *som = (sommet_t *)malloc(sizeof(sommet_t));
                som->addresse = adr_sommet;
                som->sockfd = connexion_acc;
                som->id_s = id_s++;
                printf("id_s avant pthread_create : %d\n", id_s);

                /* Add client to the queue and fork thread */
                ajout_sommet(som);
                pthread_create(&id_thread, NULL, &fonction_sommet, (void*)som);

                /* Reduce CPU usage */
                sleep(1);
        }

        return 0;
}
