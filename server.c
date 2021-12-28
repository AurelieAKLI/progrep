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
#include <netdb.h>


#define MAX_SOMMETS 100
#define BUFFER 2048

static _Atomic unsigned int compteur_sommets = 0;
static int uid = 10;

/* Client structure */
typedef struct{
    struct sockaddr_in address;
    int sockfd;
    int uid;
    char name[32];
} sommet_t;

char liste[MAX_SOMMETS][MAX_SOMMETS];

sommet_t *sommets[MAX_SOMMETS];
int nbSommets;

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;


/* ajout sommet */
void queue_add(sommet_t *cl){
        pthread_mutex_lock(&clients_mutex);

        for(int i=0; i < MAX_SOMMETS; ++i){
                if(!sommets[i]){
                        sommets[i] = cl;
                        break;
                }
        }

        pthread_mutex_unlock(&clients_mutex);
}

//envoie message
void send_message(char *s, int uid){
        pthread_mutex_lock(&clients_mutex);

        for(int i=0; i<MAX_SOMMETS; ++i){
                if(sommets[i]){
                        if(sommets[i]->uid != uid){
                                if(write(sommets[i]->sockfd, s, strlen(s)) < 0){
                                        perror("Errueur descripteur");
                                        break;
                                }
                        }
                }
        }

        pthread_mutex_unlock(&clients_mutex);
}

//Fonction lancée à la création du thread du sommet correspondant
void *handle_client(void *arg){
        //char buffer[BUFFER];

        int *i= (int *)arg;
        int a=*i;
        int x=sommets[a]->uid;
        printf("a : %d\n", a);
        //printf("compteur : %d\n", compteur_sommets);
        //sommet_t *som = (sommet_t *)arg;
        for(int j=0; j<nbSommets; ++j){
                printf("uid : %d\n", x);
                send_message(liste[j], x);
                sleep(2);
        }

        pthread_detach(pthread_self());
        sleep(2);
        return NULL;
}

int main(int argc, char **argv){
        if(argc != 2){
                printf("Usage: %s <nbSommets>\n", argv[0]);
                return EXIT_FAILURE;
        }

        nbSommets=atoi(argv[1]);
        printf("ici déjà\n");


        char *adr_ip = "127.0.0.1";
        int num_port = 5555;
        int option = 1;
        int sock, sockAccepte ;
        struct sockaddr_in adr_annuaire;
        struct sockaddr_in adr_sommet;
        int liste_uid[MAX_SOMMETS];
        //pthread_t *idThread= (pthread_t *)malloc(nbSommets*sizeof(pthread_t));
        pthread_t idThread[MAX_SOMMETS];

        printf("ici &\n");

        /* parametres socket */
        sock = socket(AF_INET, SOCK_STREAM, 0);
        adr_annuaire.sin_family = AF_INET;
        adr_annuaire.sin_addr.s_addr = inet_addr(adr_ip);
        adr_annuaire.sin_port = htons(num_port);

        /*if (setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) {
                perror("setsockopt");
                exit(1);
        }*/

        if(bind(sock, (struct sockaddr*)&adr_annuaire, sizeof(adr_annuaire)) < 0) {
                perror("Problème bind");
                return EXIT_FAILURE;
        }

        if (listen(sock, 10) < 0) {
                perror("Problème listen");
                return EXIT_FAILURE;
        }


        while(compteur_sommets!=nbSommets){
                socklen_t clilen = sizeof(adr_sommet);
                sockAccepte = accept(sock, (struct sockaddr*)&adr_sommet, &clilen);


                /* Client settings */
                sommet_t *som = (sommet_t *)malloc(sizeof(sommet_t));
                som->address = adr_sommet;
                som->sockfd = sockAccepte;
                som->uid = uid++;
                liste_uid[compteur_sommets]=uid;

                char hbuf[1025];
                char bbuf[1025];
                int x=sizeof(struct sockaddr_in);

                if (getnameinfo((struct sockaddr*)&adr_sommet, x, hbuf, sizeof(hbuf), bbuf, sizeof (bbuf), 32))  printf("ne trouve pas le numero de port d'un sommet\n");
                else  printf("numero de port =%s\n", bbuf);

                strcpy(liste[compteur_sommets],bbuf);

                /* Add client to the queue and fork thread */
                queue_add(som);
                compteur_sommets++;
                //pthread_create(&idThread, NULL, &handle_client, (void*)som);

                /* Reduce CPU usage */
                sleep(1);


        }

        for (int i=0; i<nbSommets;++i){
                printf("%s\t%d\n",liste[i], liste_uid[i]);
        }
//séquentielle

     
                for(int j=0; j<nbSommets; ++j){
                        strcat((liste[j]),"_");
                        send_message(liste[j], sommets[j]->uid);
                }
      
        /*
        int *i= (int *)malloc(sizeof (int));
        *i=0;
//parallele
        for ( int j=0; j<nbSommets; ++j){
                printf("cc : %d\n", *i);
                pthread_create(&idThread[j], NULL, *handle_client, (void *)i);
                //pthread_create(&idThread, NULL, &handle_client, (void*)som);
                *i=*i+1;
                sleep(2);
        }

        for(int i=0; i<nbSommets; ++i){
                if(pthread_join(idThread[i], NULL)!=0){
                        perror("Probleme terminaison thread");
                }
        } */

       // free(i);

        close(sock);
        return EXIT_SUCCESS;
}
