#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================
// Struct da Sala
// ==========================
typedef struct Sala {
    char nome[50];
    char pista[100];  // Pista opcional da sala
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ==========================
// Struct do nó da árvore BST de pistas
// ==========================
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// ==========================
// Função para criar uma sala
// ==========================
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (!novaSala) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    if (pista)
        strcpy(novaSala->pista, pista);
    else
        strcpy(novaSala->pista, "");  // Sala sem pista
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// ==========================
// Função para criar um nó da BST de pistas
// ==========================
PistaNode* criarPistaNode(const char* pista) {
    PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
    if (!novo) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// ==========================
// Inserir pista na BST (ordenada alfabeticamente)
// ==========================
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (!raiz) return criarPistaNode(pista);

    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // duplicatas não são inseridas
    return raiz;
}

// ==========================
// Exibir pistas em ordem alfabética
// ==========================
void exibirPistas(PistaNode* raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// ==========================
// Exploração das salas com coleta de pistas
// ==========================
void explorarSalasComPistas(Sala* atual, PistaNode** arvorePistas) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Coleta a pista, se existir
        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
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
    // Montando mapa da mansão
    Sala* hallEntrada = criarSala("Hall de Entrada", "Pegada estranha perto da porta");
    Sala* salaEstar = criarSala("Sala de Estar", "Carta rasgada no sofá");
    Sala* cozinha = criarSala("Cozinha", "Copo quebrado na pia");
    Sala* biblioteca = criarSala("Biblioteca", "Marcas de tinta no chão");
    Sala* jardim = criarSala("Jardim", "Pegadas na grama");

    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    // BST de pistas coletadas
    PistaNode* arvorePistas = NULL;

    printf("Bem-vindo(a) à mansão Detective Quest!\n");
    explorarSalasComPistas(hallEntrada, &arvorePistas);

    printf("\n===== Pistas coletadas em ordem alfabética =====\n");
    exibirPistas(arvorePistas);

    // Liberando memória
    free(jardim);
    free(biblioteca);
    free(cozinha);
    free(salaEstar);
    free(hallEntrada);

    // Aqui poderíamos liberar a BST de pistas recursivamente, mas omitido para simplificação
    printf("Obrigado por jogar!\n");
    return 0;
}
