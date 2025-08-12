#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>

typedef struct NoLista {
    int id;
    struct NoLista* proximo;
} NoLista;

NoLista* cabeca_lista = NULL;
int contador_base = 50000;

// Adiciona um ID no início da lista
void adicionar_inicio(int id) {
    NoLista* novo_no = malloc(sizeof(NoLista));
    novo_no->id = id;
    novo_no->proximo = cabeca_lista;
    cabeca_lista = novo_no;
}

// Verifica se um ID já existe na lista
int id_existe(NoLista* cabeca, int id) {
    NoLista* atual = cabeca;
    while (atual != NULL) {
        if (atual->id == id) {
            return 1;
        }
        atual = atual->proximo;
    }
    return 0;
}

// Conta quantos IDs estão na lista
int contar_ids() {
    int contador = 0;
    NoLista* atual = cabeca_lista;
    while (atual != NULL) {
        contador++;
        atual = atual->proximo;
    }
    return contador;
}

// Gera um novo ID único usando embaralhamento alternado
int gerar_id_unico() {
    int novo_id;
    do {
        // Embaralha usando padrão alternado
        if (contador_base % 2 == 0) {
            novo_id = (contador_base * 23 + 41) % 800000 + 200000;
        } else {
            novo_id = (contador_base * 19 + 37) % 600000 + 400000;
        }
        contador_base += 3;
        if (contador_base > 999999) contador_base = 50000;
    } while (id_existe(cabeca_lista, novo_id));
    
    adicionar_inicio(novo_id);
    return novo_id;
}

int main() {
    srand(time(NULL));
    
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

    printf("Servidor LISTA ENCADEADA aguardando conexoes na porta 8080...\n");
    printf("IDs armazenados na lista: %d\n", contar_ids());
    
    while ((novo_socket = accept(socket_servidor, (struct sockaddr *)&endereco, 
           (socklen_t*)&tamanho_endereco)) >= 0) {
        memset(buffer, 0, sizeof(buffer));
        read(novo_socket, buffer, 1024);
        
        if (strncmp(buffer, "GET_ID", 6) == 0) {
            int id_gerado = gerar_id_unico();
            sprintf(resposta, "ID:%d", id_gerado);
            send(novo_socket, resposta, strlen(resposta), 0);
            printf("Enviado: %s (Total na lista: %d)\n", resposta, contar_ids());
        }
        close(novo_socket);
    }
    return 0;
}
