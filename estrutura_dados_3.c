/*
  Tetris - Versão em C

aluna: Thais Oliveira de Almeida
*/

/* Bibliotecas*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 10   // Capacidade máxima da fila e pilha

//---------------------------------------------
// STRUCT PEÇA
//---------------------------------------------
typedef struct {
    char nome[20];   // nome da peça (T, L, Z, quadrado etc.)
} Peca;

//---------------------------------------------
// FILA CIRCULAR
//---------------------------------------------
typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int qtd;
} Fila;

// Inicializa fila
void initFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->qtd = 0;
}

// Verifica cheia
int filaCheia(Fila *f) {
    return (f->qtd == MAX);
}

// Verifica vazia
int filaVazia(Fila *f) {
    return (f->qtd == 0);
}

// Inserção na fila
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("\n[FILA] A fila está cheia!\n");
        return;
    }
    f->fim = (f->fim + 1) % MAX;
    f->itens[f->fim] = p;
    f->qtd++;
    printf("\n[FILA] Peça '%s' adicionada!\n", p.nome);
}

// Remoção da fila
Peca desenfileirar(Fila *f) {
    Peca vazio = {"VAZIO"};
    if (filaVazia(f)) {
        printf("\n[FILA] A fila está vazia!\n");
        return vazio;
    }
    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->qtd--;
    return p;
}

// Exibição
void mostrarFila(Fila *f) {
    printf("\n--- FILA (próximas peças) ---\n");
    if (filaVazia(f)) {
        printf("Fila vazia!\n");
        return;
    }
    int i = f->inicio;
    for (int c = 0; c < f->qtd; c++) {
        printf("[%s] ", f->itens[i].nome);
        i = (i + 1) % MAX;
    }
    printf("\n");
}

//---------------------------------------------
// PILHA
//---------------------------------------------
typedef struct {
    Peca itens[MAX];
    int topo;
} Pilha;

// Inicializa pilha
void initPilha(Pilha *p) {
    p->topo = -1;
}

// Cheia
int pilhaCheia(Pilha *p) {
    return (p->topo == MAX - 1);
}

// Vazia
int pilhaVazia(Pilha *p) {
    return (p->topo == -1);
}

// Empilhar
void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        printf("\n[PILHA] Cheia!\n");
        return;
    }
    p->topo++;
    p->itens[p->topo] = x;
    printf("\n[PILHA] Peça '%s' reservada!\n", x.nome);
}

// Desempilhar
Peca pop(Pilha *p) {
    Peca vazio = {"VAZIO"};
    if (pilhaVazia(p)) {
        printf("\n[PILHA] Vazia!\n");
        return vazio;
    }
    Peca topo = p->itens[p->topo];
    p->topo--;
    return topo;
}

// Exibir pilha
void mostrarPilha(Pilha *p) {
    printf("\n--- PILHA (reservas) ---\n");
    if (pilhaVazia(p)) {
        printf("Pilha vazia!\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%s]\n", p->itens[i].nome);
    }
}

//---------------------------------------------
// INTEGRAÇÃO FILA ↔ PILHA
//---------------------------------------------

// Reserva peça: fila → pilha
void reservarPeca(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila)) {
        printf("\nNão há peça na fila!\n");
        return;
    }
    Peca p = desenfileirar(fila);
    push(pilha, p);
}

// Recupera peça: pilha → fila
void recuperarPeca(Pilha *pilha, Fila *fila) {
    if (pilhaVazia(pilha)) {
        printf("\nNão há peça na pilha!\n");
        return;
    }
    Peca p = pop(pilha);
    enfileirar(fila, p);
}

// Troca topo da pilha com primeira da fila
void trocarTopo(Fila *fila, Pilha *pilha) {
    if (pilhaVazia(pilha) || filaVazia(fila)) {
        printf("\nNão é possível trocar, estrutura vazia.\n");
        return;
    }

    Peca tempFila = fila->itens[fila->inicio];
    Peca tempPilha = pilha->itens[pilha->topo];

    fila->itens[fila->inicio] = tempPilha;
    pilha->itens[pilha->topo] = tempFila;

    printf("\nTroca realizada com sucesso!\n");
}

//---------------------------------------------
// MENU PRINCIPAL
//---------------------------------------------
int main() {

    Fila fila;
    Pilha pilha;
    initFila(&fila);
    initPilha(&pilha);

    int opc;
    Peca nova;

    do {
        printf("\n\n====== TETRIS STACK ======\n");
        printf("1 - Inserir peça na Fila\n");
        printf("2 - Remover peça da Fila\n");
        printf("3 - Mostrar Fila\n");
        printf("4 - Reservar peça (Fila → Pilha)\n");
        printf("5 - Recuperar peça (Pilha → Fila)\n");
        printf("6 - Trocar topo da pilha com início da fila\n");
        printf("7 - Mostrar Pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);
        getchar();

        switch (opc) {

        case 1:
            printf("Nome da peça: ");
            fgets(nova.nome, 20, stdin);
            nova.nome[strcspn(nova.nome, "\n")] = 0;
            enfileirar(&fila, nova);
            break;

        case 2: {
            Peca p = desenfileirar(&fila);
            printf("Peça removida: %s\n", p.nome);
            break;
        }

        case 3:
            mostrarFila(&fila);
            break;

        case 4:
            reservarPeca(&fila, &pilha);
            break;

        case 5:
            recuperarPeca(&pilha, &fila);
            break;

        case 6:
            trocarTopo(&fila, &pilha);
            break;

        case 7:
            mostrarPilha(&pilha);
            break;

        case 0:
            printf("\nEncerrando...\n");
            break;

        default:
            printf("\nOpção inválida!\n");
        }

    } while (opc != 0);

    return 0;
}
