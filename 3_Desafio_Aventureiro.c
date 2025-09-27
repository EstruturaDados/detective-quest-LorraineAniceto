#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10  // tamanho simples da tabela hash

// ==========================
// Struct da Sala
// ==========================
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ==========================
// Struct da BST de pistas
// ==========================
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// ==========================
// Tabela hash: pista -> suspeito
// ==========================
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode* proximo;
} HashNode;

HashNode* hashTable[HASH_SIZE];

// ==========================
// Funções utilitárias
// ==========================
unsigned int hashFunction(const char* str) {
    unsigned int hash = 0;
    while (*str) hash = (hash * 31 + *str++) % HASH_SIZE;
    return hash;
}

void inserirNaHash(const char* pista, const char* suspeito) {
    unsigned int idx = hashFunction(pista);
    HashNode* novo = (HashNode*)malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = hashTable[idx];
    hashTable[idx] = novo;
}

const char* encontrarSuspeito(const char* pista) {
    unsigned int idx = hashFunction(pista);
    HashNode* atual = hashTable[idx];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0) return atual->suspeito;
        atual = atual->proximo;
    }
    return NULL;
}

// ==========================
// Criação da sala
// ==========================
Sala* criarSala(const char* nome, const char* pista) {
    Sala* s = (Sala*)malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    strcpy(s->pista, pista ? pista : "");
    s->esquerda = s->direita = NULL;
    return s;
}

// ==========================
// Inserção na BST de pistas
// ==========================
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (!raiz) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0) raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0) raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

// ==========================
// Exibição da BST em ordem
// ==========================
void exibirPistas(PistaNode* raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// ==========================
// Exploração da mansão
// ==========================
void explorarSalas(Sala* atual, PistaNode** arvorePistas) {
    char escolha;
    while (atual) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }

        printf("Escolha o caminho: esquerda (e), direita (d) ou sair (s): ");
        scanf(" %c", &escolha);

        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda) atual = atual->esquerda;
            else printf("Não há sala à esquerda.\n");
        } else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita) atual = atual->direita;
            else printf("Não há sala à direita.\n");
        } else if (escolha == 's' || escolha == 'S') {
            printf("Exploração encerrada.\n");
            break;
        } else {
            printf("Opção inválida.\n");
        }
    }
}

// ==========================
// Verificação final do suspeito
// ==========================
void verificarSuspeitoFinal(PistaNode* arvorePistas, const char* suspeito) {
    int count = 0;
    if (!arvorePistas) return;

    // Função recursiva para contar pistas apontando para o suspeito
    void contar(PistaNode* n) {
        if (!n) return;
        const char* s = encontrarSuspeito(n->pista);
        if (s && strcmp(s, suspeito) == 0) count++;
        contar(n->esquerda);
        contar(n->direita);
    }

    contar(arvorePistas);

    if (count >= 2)
        printf("\nVocê tem evidências suficientes! %s é o culpado!\n", suspeito);
    else
        printf("\nNão há pistas suficientes para acusar %s.\n", suspeito);
}

// ==========================
// Main
// ==========================
int main() {
    // Limpa hashTable
    for (int i=0; i<HASH_SIZE; i++) hashTable[i] = NULL;

    // Monta mansão
    Sala* hall = criarSala("Hall", "Pegada estranha perto da porta");
    Sala* salaEstar = criarSala("Sala de Estar", "Carta rasgada no sofá");
    Sala* cozinha = criarSala("Cozinha", "Copo quebrado na pia");
    Sala* biblioteca = criarSala("Biblioteca", "Marcas de tinta no chão");
    Sala* jardim = criarSala("Jardim", "Pegadas na grama");

    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    // Associa pistas a suspeitos na hash
    inserirNaHash("Pegada estranha perto da porta", "Sr. Verde");
    inserirNaHash("Carta rasgada no sofá", "Sra. Rosa");
    inserirNaHash("Copo quebrado na pia", "Sr. Verde");
    inserirNaHash("Marcas de tinta no chão", "Sra. Azul");
    inserirNaHash("Pegadas na grama", "Sra. Rosa");

    // BST de pistas coletadas
    PistaNode* arvorePistas = NULL;

    printf("Bem-vindo(a) à mansão Detective Quest!\n");
    explorarSalas(hall, &arvorePistas);

    printf("\n===== Pistas coletadas =====\n");
    exibirPistas(arvorePistas);

    char acusacao[50];
    printf("\nDigite o suspeito que deseja acusar: ");
    scanf(" %[^\n]", acusacao);

    verificarSuspeitoFinal(arvorePistas, acusacao);

    // Liberação simplificada de memória (salas)
    free(jardim); free(biblioteca); free(cozinha);
    free(salaEstar); free(hall);

    printf("\nObrigado por jogar!\n");
    return 0;
}
