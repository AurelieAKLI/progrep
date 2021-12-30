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

#define MAX_SOMMETS 100

// Global variables
int sockfd = 0;
int boolEnvoie=0;//si =0 alors envoyer numero de port sinon on envoie numero de sommet
int recu=0;//on a toujour pas recu la liste de num_port; sinon envoyer numSommet

int numSommet; //numero du sommet à comparer avec les autres
int num_port; //numero de port de la socket d'ecoute
char liste[MAX_SOMMETS][MAX_SOMMETS];
int compteur; //compteur dans la liste

volatile sig_atomic_t flag = 0;
void catch_ctrl_c_and_exit(int sig) {
        flag = 1;
}

void send_msg_handler() {
        /*char msg[10];
        printf("%d\t%d\n", recu, boolEnvoie);
        sleep(1);
        printf("char\n");
        while(recu==0 || boolEnvoie==0){
                printf("ici52\n");
                if (boolEnvoie==0){
                        sprintf(msg, "%d\n", num_port);
                        send(sockfd, msg, strlen(msg), 0);
                        boolEnvoie=1;
                }
                sleep(2);
        }

        if(boolEnvoie==1 && recu==1){
                sprintf(msg, "%d\n", numSommet);
                send(sockfd, msg, strlen(msg), 0);
        }*/

        char msg[10];
        sprintf(msg, "%d\n", num_port);
        send(sockfd, msg, strlen(msg), 0);


        catch_ctrl_c_and_exit(2);
}

void recv_msg_handler() {
        int reception=0;
        while (reception<=0) {//doit att tant que l'on ne reçoit qqc
                char message[10];
                reception = recv(sockfd, message, MAX_SOMMETS, 0);
                if (reception > 0) {
                        while (reception>1){//si on reçoit
                                reception = recv(sockfd, message, MAX_SOMMETS, 0);
                                strcpy(liste[compteur],message);
                                printf("%s\n", liste[compteur]);
                                compteur++;
                        }


                }
        }
}

int main(int argc, char **argv){
        if(argc != 3){
                printf("Usage: %s <port> <numSommet>\n", argv[0]);
                return 1;
        }

        char *ip = "127.0.0.1";
        int port = atoi(argv[1]);

        //creation socket d'ecoute
        int sockEcoute= socket(AF_INET, SOCK_STREAM,0);
        struct sockaddr_in serv_ecoute_client;
        serv_ecoute_client.sin_family=AF_INET;
        serv_ecoute_client.sin_addr.s_addr= htonl(INADDR_ANY);
        serv_ecoute_client.sin_port=htons(0);

        int longueur=sizeof(serv_ecoute_client);
        if(bind(sockEcoute, (struct sockaddr*)&serv_ecoute_client, longueur) < 0) {
                perror("ERROR: Socket binding failed 1");
                return EXIT_FAILURE;
        }

        //on recupere le numero de port de la socket d'ecoute
        int x=sizeof(serv_ecoute_client);
        if (getsockname(sockEcoute, ((struct sockaddr*)&serv_ecoute_client), &x) == -1) {
                perror("getsockname() failed");
                return -1;
        }
        num_port= ntohs(serv_ecoute_client.sin_port);
        printf("Local port is: %d\n", num_port);

        signal(SIGINT, catch_ctrl_c_and_exit);

        struct sockaddr_in server_addr;
        printf("ici 104\n");

        /* Parametres socket d'écriture */
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(ip);
        server_addr.sin_port = htons(port);
        printf("ici 111\n");


        if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))== -1) {
                printf("ERROR: connect\n");
                return EXIT_FAILURE;
        }
        printf("ici 120\n");
/*
        char msg[10];
        sprintf(msg, "%d\n", num_port);
        send(sockfd, msg, strlen(msg), 0);
        */

        pthread_t send_msg_thread;
        if(pthread_cr#include <stdio.h>
                      #include <stdlib.h>
                      #include <string.h>
                      #include <signal.h>
                      #include <unistd.h>
                      #include <sys/types.h>
                      #include <sys/socket.h>
                      #include <netinet/in.h>
                      #include <arpa/inet.h>
                      #include <pthread.h>

                      #define MAX_SOMMETS 100

// Global variables
        int sockfd = 0;
        int boolEnvoie=0;//si =0 alors envoyer numero de port sinon on envoie numero de sommet
        int recu=0;//on a toujour pas recu la liste de num_port; sinon envoyer numSommet

        int numSommet; //numero du sommet à comparer avec les autres
        int num_port; //numero de port de la socket d'ecoute
        char liste[MAX_SOMMETS][MAX_SOMMETS];
        int compteur; //compteur dans la liste

        volatile sig_atomic_t flag = 0;
        void catch_ctrl_c_and_exit(int sig) {
                flag = 1;
        }

        void send_msg_handler() {
                /*char msg[10];
                printf("%d\t%d\n", recu, boolEnvoie);
                sleep(1);
                printf("char\n");
                while(recu==0 || boolEnvoie==0){
                        printf("ici52\n");
                        if (boolEnvoie==0){
                                sprintf(msg, "%d\n", num_port);
                                send(sockfd, msg, strlen(msg), 0);
                                boolEnvoie=1;
                        }
                        sleep(2);
                }
        
                if(boolEnvoie==1 && recu==1){
                        sprintf(msg, "%d\n", numSommet);
                        send(sockfd, msg, strlen(msg), 0);
                }*/

                char msg[10];
                sprintf(msg, "%d\n", num_port);
                send(sockfd, msg, strlen(msg), 0);


                catch_ctrl_c_and_exit(2);
        }

        void recv_msg_handler() {
                int reception=0;
                while (reception<=0) {//doit att tant que l'on ne reçoit qqc
                        char message[10];
                        reception = recv(sockfd, message, MAX_SOMMETS, 0);
                        if (reception > 0) {
                                while (reception>1){//si on reçoit
                                        reception = recv(sockfd, message, MAX_SOMMETS, 0);
                                        strcpy(liste[compteur],message);
                                        printf("%s\n", liste[compteur]);
                                        compteur++;
                                }


                        }
                }
        }

        int main(int argc, char **argv){
                if(argc != 3){
                        printf("Usage: %s <port> <numSommet>\n", argv[0]);
                        return 1;
                }

                char *ip = "127.0.0.1";
                int port = atoi(argv[1]);

                //creation socket d'ecoute
                int sockEcoute= socket(AF_INET, SOCK_STREAM,0);
                struct sockaddr_in serv_ecoute_client;
                serv_ecoute_client.sin_family=AF_INET;
                serv_ecoute_client.sin_addr.s_addr= htonl(INADDR_ANY);
                serv_ecoute_client.sin_port=htons(0);

                int longueur=sizeof(serv_ecoute_client);
                if(bind(sockEcoute, (struct sockaddr*)&serv_ecoute_client, longueur) < 0) {
                        perror("ERROR: Socket binding failed 1");
                        return EXIT_FAILURE;
                }

                //on recupere le numero de port de la socket d'ecoute
                int x=sizeof(serv_ecoute_client);
                if (getsockname(sockEcoute, ((struct sockaddr*)&serv_ecoute_client), &x) == -1) {
                        perror("getsockname() failed");
                        return -1;
                }
                num_port= ntohs(serv_ecoute_client.sin_port);
                printf("Local port is: %d\n", num_port);

                signal(SIGINT, catch_ctrl_c_and_exit);

                struct sockaddr_in server_addr;
                printf("ici 104\n");

                /* Parametres socket d'écriture */
                sockfd = socket(AF_INET, SOCK_STREAM, 0);
                server_addr.sin_family = AF_INET;
                server_addr.sin_addr.s_addr = inet_addr(ip);
                server_addr.sin_port = htons(port);
                printf("ici 111\n");


                if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))== -1) {
                        printf("ERROR: connect\n");
                        return EXIT_FAILURE;
                }
                printf("ici 120\n");
/*
        char msg[10];
        sprintf(msg, "%d\n", num_port);
        send(sockfd, msg, strlen(msg), 0);
        */

                pthread_t send_msg_thread;
                if(pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0){
                        printf("ERROR: pthread\n");
                        return EXIT_FAILURE;
                }
                printf("ici 132\n");

                pthread_t recv_msg_thread;
                if(pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0){
                        printf("ERROR: pthread\n");
                        return EXIT_FAILURE;
                }


                printf("ici 141\n");

                close(sockfd);
                close(sockEcoute);

                return 0;
        }eate(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0){
                printf("ERROR: pthread\n");
                return EXIT_FAILURE;
        }
        printf("ici 132\n");

        pthread_t recv_msg_thread;
        if(pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0){
                printf("ERROR: pthread\n");
                return EXIT_FAILURE;
        }


        printf("ici 141\n");

        close(sockfd);
        close(sockEcoute);

        return 0;
}
