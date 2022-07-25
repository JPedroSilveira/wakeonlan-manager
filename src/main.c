#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <stdint.h>
#include <pthread.h>
#include <netinet/in.h>

#define PORT 4000

typedef struct __packet{
 uint16_t type; //Tipo do pacote (p.ex. DATA | CMD)
 uint16_t seqn; //Número de sequência
 uint16_t length; //Comprimento do payload
 uint16_t timestamp; // Timestamp do dado
 const char* _payload; //Dados da mensagem
} packet;

typedef struct meta {
int type; // 1 para manager 0 pra client
char *address; // endereco de ip 
bool awake;	// Mais coisas se precisar
} meta;

void *sender_thread(void *arg){
	while(1) {
	int t = * (int *) arg;
	printf("Essa thread vai enviar dados\n Aplicacâo do tipo");
		if(t = 1 ){
		printf("manager\n");
		} else if ( t = 0 ){
		printf("cliente\n");
		} else{
		printf("???????wtf\n");}
	sleep(1);
	}
}

void *reciever_thread(void *arg){
	while(1) {
	int t = * (int *) arg;
	printf("Essa thread vai receber dados\n Aplicacâo do tipo");
		if(t = 1 ){
		printf("manager\n");
		} else if ( t = 0 ){
		printf("cliente\n");
		} else{
		printf("???????wtf\n");}
	sleep(1);
	}
} 

struct meta init (int argc, char argv[]) {
struct meta ini;
	if (argc == 1){
		ini.type = 0;
	 } else if (strcmp(argv[1], "manager") == 0) {
		ini.type = 1;
         } else {
		ini.type = 0;
         }
	ini.address = "endereco";
                          
return ini;
}


int main(int argc, char *argv[]) {
	pthread_t thread_sender, thread_reciever;				// Nessa linha vamos ter que adicionar mais uma thread
	struct meta self = init(argc, *argv); 	// Acho que a segfault tá acontecendo aqui 

	if (argc == 1){
		printf("oi, eu sou o participante\n");
	} else if (strcmp(argv[1], "manager") == 0) {		
		printf("oi, eu sou o manager\n");
		
		//Cria thread nova para enviar os dados pros outros participantes
		pthread_create(&thread_sender, NULL, sender_thread, &self.type);
		pthread_create(&thread_reciever, NULL, reciever_thread, &self.type);
			while(1){
			printf("Essa thread vai fazer os io\n");
        		sleep(1);
        		}
	} else {
		printf("ERROR: invalid input\n");
	}
	return 0; 
}

