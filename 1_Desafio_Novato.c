#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================
// Struct da Sala
// ==========================
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ==========================
// Função para criar uma sala
// ==========================
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// ==========================
// Função para explorar salas
// ==========================
void explorarSalas(Sala* atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Verifica se é nó-folha
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Você chegou a uma sala sem caminhos. Fim da exploração!\n");
            break;
        }

        printf("Escolha o caminho: esquerda (e), direita (d) ou sair (s): ");
        scanf(" %c", &escolha);

        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Não há sala à esquerda. Tente outro caminho.\n");
            }
        } else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Não há sala à direita. Tente outro caminho.\n");
            }
        } else if (escolha == 's' || escolha == 'S') {
            printf("Exploração encerrada pelo jogador.\n");
            break;
        } else {
            printf("Opção inválida. Digite e, d ou s.\n");
        }
    }
}

// ==========================
// Função main
// ==========================
int main() {
    // Criando o mapa da mansão (árvore binária)
    Sala* hallEntrada = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardim = criarSala("Jardim");

    // Montando a árvore
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    // Início da exploração
    printf("Bem-vindo(a) à mansão Detective Quest!\n");
    explorarSalas(hallEntrada);

    // Liberando memória
    free(jardim);
    free(biblioteca);
    free(cozinha);
    free(salaEstar);
    free(hallEntrada);

    printf("Obrigado por jogar!\n");
    return 0;
}
