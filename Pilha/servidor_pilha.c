#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>

typedef struct NoPilha {
    int id;
    struct NoPilha* anterior;
} NoPilha;

typedef struct {
    NoPilha* topo;
    int tamanho;
} Pilha;

Pilha* ids_usados;
int contador_base = 1000;

// Cria uma nova pilha vazia
Pilha* criar_pilha() {
    Pilha* nova_pilha = malloc(sizeof(Pilha));
    nova_pilha->topo = NULL;
    nova_pilha->tamanho = 0;
    return nova_pilha;
}

// Empilha um ID na pilha
void empilhar(Pilha* pilha, int id) {
    NoPilha* novo_no = malloc(sizeof(NoPilha));
    novo_no->id = id;
    novo_no->anterior = pilha->topo;
    pilha->topo = novo_no;
    pilha->tamanho++;
}

// Verifica se um ID já existe na pilha
int id_existe(Pilha* pilha, int id) {
    NoPilha* atual = pilha->topo;
    while (atual != NULL) {
        if (atual->id == id) {
            return 1;
        }
        atual = atual->anterior;
    }
    return 0;
}

// Gera um novo ID único usando embaralhamento reverso
int gerar_id_unico() {
    int novo_id;
    do {
        // Embaralha o contador usando operações reversas
        novo_id = ((contador_base * 13 + 29) % 900000) + 100000;
        contador_base--;
        if (contador_base <= 0) contador_base = 1000000;
    } while (id_existe(ids_usados, novo_id));
    
    empilhar(ids_usados, novo_id);
    return novo_id;
}

int main() {
    srand(time(NULL));
    ids_usados = criar_pilha();
    
    int socket_servidor, novo_socket;
    struct sockaddr_in endereco;
    int tamanho_endereco = sizeof(endereco);
    char buffer[1024] = {0};
    char resposta[64];

    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_port = htons(8080);

    bind(socket_servidor, (struct sockaddr *)&endereco, sizeof(endereco));
    listen(socket_servidor, 3);

    printf("Servidor PILHA aguardando conexoes na porta 8080...\n");
    printf("IDs armazenados na pilha: %d\n", ids_usados->tamanho);
    
    while ((novo_socket = accept(socket_servidor, (struct sockaddr *)&endereco, 
           (socklen_t*)&tamanho_endereco)) >= 0) {
        memset(buffer, 0, sizeof(buffer));
        read(novo_socket, buffer, 1024);
        
        if (strncmp(buffer, "GET_ID", 6) == 0) {
            int id_gerado = gerar_id_unico();
            sprintf(resposta, "ID:%d", id_gerado);
            send(novo_socket, resposta, strlen(resposta), 0);
            printf("Enviado: %s (Total na pilha: %d)\n", resposta, ids_usados->tamanho);
        }
        close(novo_socket);
    }
    return 0;
}
