#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>

typedef struct NoFila {
    int id;
    struct NoFila* proximo;
} NoFila;

typedef struct {
    NoFila* inicio;
    NoFila* fim;
    int tamanho;
} Fila;

Fila* ids_usados;
int contador_base = 1;

// Cria uma nova fila vazia
Fila* criar_fila() {
    Fila* nova_fila = malloc(sizeof(Fila));
    nova_fila->inicio = NULL;
    nova_fila->fim = NULL;
    nova_fila->tamanho = 0;
    return nova_fila;
}

// Adiciona um ID na fila
void enfileirar(Fila* fila, int id) {
    NoFila* novo_no = malloc(sizeof(NoFila));
    novo_no->id = id;
    novo_no->proximo = NULL;
    
    if (fila->fim == NULL) {
        fila->inicio = novo_no;
        fila->fim = novo_no;
    } else {
        fila->fim->proximo = novo_no;
        fila->fim = novo_no;
    }
    fila->tamanho++;
}

// Verifica se um ID já existe na fila
int id_existe(Fila* fila, int id) {
    NoFila* atual = fila->inicio;
    while (atual != NULL) {
        if (atual->id == id) {
            return 1;
        }
        atual = atual->proximo;
    }
    return 0;
}

// Gera um novo ID único usando embaralhamento simples
int gerar_id_unico() {
    int novo_id;
    do {
        // Embaralha o contador usando operações matemáticas simples
        novo_id = (contador_base * 31 + 17) % 1000000 + (contador_base % 7) * 100000;
        if (novo_id == 0) novo_id = 1;
        contador_base++;
    } while (id_existe(ids_usados, novo_id));
    
    enfileirar(ids_usados, novo_id);
    return novo_id;
}

int main() {
    srand(time(NULL));
    ids_usados = criar_fila();
    
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

    printf("Servidor FILA aguardando conexoes na porta 8080...\n");
    printf("IDs armazenados na fila: %d\n", ids_usados->tamanho);
    
    while ((novo_socket = accept(socket_servidor, (struct sockaddr *)&endereco, 
           (socklen_t*)&tamanho_endereco)) >= 0) {
        memset(buffer, 0, sizeof(buffer));
        read(novo_socket, buffer, 1024);
        
        if (strncmp(buffer, "GET_ID", 6) == 0) {
            int id_gerado = gerar_id_unico();
            sprintf(resposta, "ID:%d", id_gerado);
            send(novo_socket, resposta, strlen(resposta), 0);
            printf("Enviado: %s (Total na fila: %d)\n", resposta, ids_usados->tamanho);
        }
        close(novo_socket);
    }
    return 0;
}
