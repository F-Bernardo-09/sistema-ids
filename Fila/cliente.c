#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    int quantidade_ids;
    char mensagem[] = "GET_ID";
    char resposta_servidor[2000];
    int ids_recebidos = 0;
    
    printf("Quantas identidades deseja solicitar? ");
    scanf("%d", &quantidade_ids);
    
    clock_t tempo_inicio = clock();
    
    for (int i = 0; i < quantidade_ids; i++) {
        int socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_cliente < 0) {
            perror("Erro ao criar socket");
            break;
        }
        
        struct sockaddr_in servidor;
        servidor.sin_addr.s_addr = inet_addr(SERVER_IP);
        servidor.sin_family = AF_INET;
        servidor.sin_port = htons(SERVER_PORT);
        
        if (connect(socket_cliente, (struct sockaddr *)&servidor, sizeof(servidor)) < 0) {
            perror("Erro na conexao");
            close(socket_cliente);
            break;
        }
        
        if (send(socket_cliente, mensagem, strlen(mensagem), 0) < 0) {
            perror("Erro no envio");
            close(socket_cliente);
            break;
        }
        
        memset(resposta_servidor, 0, sizeof(resposta_servidor));
        if (recv(socket_cliente, resposta_servidor, 2000, 0) > 0) {
            ids_recebidos++;
            printf("ID %d: %s\n", ids_recebidos, resposta_servidor);
        }
        
        close(socket_cliente);
    }
    
    clock_t tempo_fim = clock();
    double tempo_total = ((double)(tempo_fim - tempo_inicio)) / CLOCKS_PER_SEC;
    
    printf("\nResumo:\n");
    printf("Total de identidades recebidas: %d\n", ids_recebidos);
    printf("Tempo total: %.2f segundos\n", tempo_total);
    printf("IDs por segundo: %.2f\n", ids_recebidos / tempo_total);
    
    return 0;
}
