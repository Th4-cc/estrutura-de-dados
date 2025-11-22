/*
  Detective Quest - Versão em C

aluna: Thais Oliveira de Almeida
*/

/* Bibliotecas*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10

// ---------------------------
// ÁRVORE BINÁRIA – MAPA
// ---------------------------
typedef struct MapNode {
    char room[50];
    struct MapNode *left;
    struct MapNode *right;
} MapNode;

MapNode* createRoom(char *room) {
    MapNode *newNode = (MapNode*) malloc(sizeof(MapNode));
    strcpy(newNode->room, room);
    newNode->left = newNode->right = NULL;
    return newNode;
}

void showMap(MapNode *root, int depth) {
    if (!root) return;
    for (int i = 0; i < depth; i++) printf("  ");
    printf("- %s\n", root->room);
    showMap(root->left, depth + 1);
    showMap(root->right, depth + 1);
}

// ---------------------------
// ÁRVORE DE BUSCA – PISTAS
// ---------------------------
typedef struct ClueNode {
    char clue[100];
    struct ClueNode *left;
    struct ClueNode *right;
} ClueNode;

ClueNode* insertClue(ClueNode *root, char *clue) {
    if (!root) {
        ClueNode *newNode = (ClueNode*) malloc(sizeof(ClueNode));
        strcpy(newNode->clue, clue);
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    if (strcmp(clue, root->clue) < 0)
        root->left = insertClue(root->left, clue);
    else
        root->right = insertClue(root->right, clue);

    return root;
}

void showClues(ClueNode *root) {
    if (!root) return;
    showClues(root->left);
    printf("• %s\n", root->clue);
    showClues(root->right);
}

// ---------------------------
// TABELA HASH – PISTA → SUSPEITO
// ---------------------------
typedef struct Pair {
    char clue[100];
    char suspect[50];
    struct Pair *next;
} Pair;

Pair* hashTable[TABLE_SIZE];

int hash(char *str) {
    int sum = 0;
    for (int i = 0; str[i] != '\0'; i++)
        sum += str[i];
    return sum % TABLE_SIZE;
}

void addRelation(char *clue, char *suspect) {
    int index = hash(clue);

    Pair *newPair = (Pair*) malloc(sizeof(Pair));
    strcpy(newPair->clue, clue);
    strcpy(newPair->suspect, suspect);
    newPair->next = hashTable[index];
    hashTable[index] = newPair;
}

char* findSuspect(char *clue) {
    int index = hash(clue);
    Pair *curr = hashTable[index];

    while (curr) {
        if (strcmp(curr->clue, clue) == 0)
            return curr->suspect;
        curr = curr->next;
    }
    return NULL;
}

void showRelations() {
    printf("\n=== Relações Pista → Suspeito ===\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Pair *curr = hashTable[i];
        while (curr) {
            printf("Pista: %-25s | Suspeito: %s\n", curr->clue, curr->suspect);
            curr = curr->next;
        }
    }
}

// ---------------------------
// MAIN – JOGO
// ---------------------------
int main() {
    // --------------------
    // Mapa da mansão
    // --------------------
    MapNode *map = createRoom("Entrada");
    map->left  = createRoom("Sala de Estar");
    map->right = createRoom("Biblioteca");

    map->left->left  = createRoom("Cozinha");
    map->left->right = createRoom("Sala de Jantar");

    map->right->left  = createRoom("Escritório");
    map->right->right = createRoom("Porão");

    printf("\n===== MAPA DA MANSÃO =====\n");
    showMap(map, 0);

    // --------------------
    // Inserção de Pistas (BST)
    // --------------------
    ClueNode *clues = NULL;
    clues = insertClue(clues, "Chave dourada");
    clues = insertClue(clues, "Pegadas de lama");
    clues = insertClue(clues, "Luvas rasgadas");
    clues = insertClue(clues, "Carta misteriosa");

    printf("\n===== PISTAS COLETADAS =====\n");
    showClues(clues);

    // --------------------
    // Tabela Hash (pista → suspeito)
    // --------------------
    addRelation("Chave dourada", "Sr. Roberto");
    addRelation("Pegadas de lama", "Jardineiro");
    addRelation("Luvas rasgadas", "Cozinheira");

    showRelations();

    // Busca um suspeito
    char searchClue[100];
    printf("\nBuscar suspeito pela pista: ");
    fgets(searchClue, 100, stdin);
    searchClue[strcspn(searchClue, "\n")] = 0;

    char *suspect = findSuspect(searchClue);
    if (suspect)
        printf("A pista \"%s\" aponta para: %s\n", searchClue, suspect);
    else
        printf("Nenhum suspeito encontrado para esta pista.\n");

    return 0;
}
