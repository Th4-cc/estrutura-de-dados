/* aluna: Thais Oliveira de Almeida
*/

/*
  Survival Inventory & Sorting - Projeto de listas e ordenacao
  - Estruturas:
      * Componente/Item: nome, tipo, prioridade
      * Inventario vetor (estatico, max 20)
      * Lista encadeada dinamica (insercao, remocao, busca sequencial)
  - Ordenacoes:
      * Bubble sort por nome (comparacoes, tempo)
      * Insertion sort por tipo  (comparacoes, tempo)
      * Selection sort por prioridade (comparacoes, tempo)
  - Buscas:
      * Busca sequencial (vetor e lista)
      * Busca binaria por nome (iterativa e recursiva) - requer vetor ordenado por nome
  - Medicoes:
      * Numero de comparacoes e tempo (clock) para cada algoritmo
  - Interface menu-driven e funcões modularizadas
*/

/* Bibliotecas*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_ITEMS 20
#define NAME_LEN 30
#define TYPE_LEN 20

/* ----- Tipo Componente/Item ----- */
typedef struct {
    char nome[NAME_LEN];
    char tipo[TYPE_LEN];
    int prioridade; // 1..10
} Item;

/* ----- Lista encadeada ----- */
typedef struct Node {
    Item dados;
    struct Node *next;
} Node;

/* ----- utilitários ----- */
void trim_newline(char *s) {
    size_t n = strlen(s);
    if (n && s[n-1] == '\n') s[n-1] = '\0';
}
void ler_string(char *buf, int len) {
    if (!fgets(buf, len, stdin)) { buf[0] = '\0'; return; }
    trim_newline(buf);
}
int stricmp_local(const char *a, const char *b) {
    for (; *a && *b; ++a, ++b) {
        char ca = tolower((unsigned char)*a), cb = tolower((unsigned char)*b);
        if (ca != cb) return (int)(ca - cb);
    }
    return (int)(tolower((unsigned char)*a) - tolower((unsigned char)*b));
}

/* ----- operações vetor (estático) ----- */
/* inserção no final */
int vetor_inserir(Item v[], int *n, Item it) {
    if (*n >= MAX_ITEMS) return 0;
    v[*n] = it;
    (*n)++;
    return 1;
}
/* remoção por índice */
int vetor_remover(Item v[], int *n, int idx) {
    if (idx < 0 || idx >= *n) return 0;
    for (int i = idx; i < *n - 1; ++i) v[i] = v[i+1];
    (*n)--;
    return 1;
}
/* busca sequencial no vetor por nome (case-insensitive), retorna índice ou -1 */
int vetor_busca_seq(Item v[], int n, const char *nome, long *comparacoes) {
    long comps = 0;
    for (int i = 0; i < n; ++i) {
        comps++;
        if (stricmp_local(v[i].nome, nome) == 0) {
            if (comparacoes) *comparacoes = comps;
            return i;
        }
    }
    if (comparacoes) *comparacoes = comps;
    return -1;
}

/* ----- lista encadeada (dinâmica) ----- */
/* inserir no fim */
void lista_inserir(Node **head, Item it) {
    Node *novo = (Node*) malloc(sizeof(Node));
    if (!novo) { perror("malloc"); return; }
    novo->dados = it; novo->next = NULL;
    if (!*head) { *head = novo; return; }
    Node *p = *head;
    while (p->next) p = p->next;
    p->next = novo;
}
/* remover por nome (primeira ocorrencia) - retorna 1 se removido */
int lista_remover(Node **head, const char *nome) {
    Node *p = *head, *prev = NULL;
    while (p) {
        if (stricmp_local(p->dados.nome, nome) == 0) {
            if (prev) prev->next = p->next;
            else *head = p->next;
            free(p);
            return 1;
        }
        prev = p; p = p->next;
    }
    return 0;
}
/* busca sequencial na lista por nome: retorna ponteiro para Node ou NULL; conta comparacoes */
Node *lista_busca_seq(Node *head, const char *nome, long *comparacoes) {
    long comps = 0;
    Node *p = head;
    while (p) {
        comps++;
        if (stricmp_local(p->dados.nome, nome) == 0) {
            if (comparacoes) *comparacoes = comps;
            return p;
        }
        p = p->next;
    }
    if (comparacoes) *comparacoes = comps;
    return NULL;
}
/* liberar lista */
void lista_liberar(Node **head) {
    Node *p = *head;
    while (p) {
        Node *t = p; p = p->next; free(t);
    }
    *head = NULL;
}

/* exibir vetor e lista */
void mostrar_vetor(Item v[], int n) {
    printf("Índice | Nome                       | Tipo               | Prio\n");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < n; ++i)
        printf("%5d  | %-25s | %-18s | %3d\n", i, v[i].nome, v[i].tipo, v[i].prioridade);
}
void mostrar_lista(Node *head) {
    printf("Lista encadeada:\nIdx | Nome                       | Tipo               | Prio\n");
    printf("---------------------------------------------------------------\n");
    int i = 0;
    Node *p = head;
    while (p) {
        printf("%3d | %-25s | %-18s | %3d\n", i++, p->dados.nome, p->dados.tipo, p->dados.prioridade);
        p = p->next;
    }
}

/* ----- algoritmos de ordenacao no vetor (cada um conta comparacoes) ----- */
/* bubble sort por nome */
void bubble_sort_nome(Item v[], int n, long *comparacoes) {
    long comps = 0;
    for (int i = 0; i < n-1; ++i) {
        int trocou = 0;
        for (int j = 0; j < n-1-i; ++j) {
            comps++;
            if (stricmp_local(v[j].nome, v[j+1].nome) > 0) {
                Item t = v[j]; v[j] = v[j+1]; v[j+1] = t;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
    if (comparacoes) *comparacoes = comps;
}
/* insertion sort por tipo */
void insertion_sort_tipo(Item v[], int n, long *comparacoes) {
    long comps = 0;
    for (int i = 1; i < n; ++i) {
        Item key = v[i];
        int j = i - 1;
        while (j >= 0) {
            comps++;
            if (stricmp_local(v[j].tipo, key.tipo) > 0) {
                v[j+1] = v[j]; j--;
            } else break;
        }
        v[j+1] = key;
    }
    if (comparacoes) *comparacoes = comps;
}
/* selection sort por prioridade (descendente: maior prioridade primeiro) */
void selection_sort_prioridade(Item v[], int n, long *comparacoes) {
    long comps = 0;
    for (int i = 0; i < n-1; ++i) {
        int idxMax = i;
        for (int j = i+1; j < n; ++j) {
            comps++;
            if (v[j].prioridade > v[idxMax].prioridade) idxMax = j;
        }
        if (idxMax != i) {
            Item t = v[i]; v[i] = v[idxMax]; v[idxMax] = t;
        }
    }
    if (comparacoes) *comparacoes = comps;
}

/* ----- busca binaria por nome (vetor) - iterativa e recursiva ----- */
/* iterativa */
int busca_binaria_iter(Item v[], int n, const char *chave, long *comparacoes) {
    int lo = 0, hi = n-1;
    long comps = 0;
    while (lo <= hi) {
        int mid = lo + (hi - lo)/2;
        comps++;
        int cmp = stricmp_local(v[mid].nome, chave);
        if (cmp == 0) { if (comparacoes) *comparacoes = comps; return mid; }
        else if (cmp < 0) lo = mid + 1;
        else hi = mid - 1;
    }
    if (comparacoes) *comparacoes = comps;
    return -1;
}
/* recursiva (helper) */
int busca_binaria_rec_aux(Item v[], int lo, int hi, const char *chave, long *comparacoes) {
    if (lo > hi) return -1;
    int mid = lo + (hi - lo)/2;
    (*comparacoes)++;
    int cmp = stricmp_local(v[mid].nome, chave);
    if (cmp == 0) return mid;
    else if (cmp < 0) return busca_binaria_rec_aux(v, mid+1, hi, chave, comparacoes);
    else return busca_binaria_rec_aux(v, lo, mid-1, chave, comparacoes);
}
int busca_binaria_rec(Item v[], int n, const char *chave, long *comparacoes) {
    long comps = 0;
    int idx = busca_binaria_rec_aux(v, 0, n-1, chave, &comps);
    if (comparacoes) *comparacoes = comps;
    return idx;
}

/* ----- medir tempo (clock) wrapper para funcoes de ordenacao ----- */
double medir_tempo_sort(void (*sort_fn)(Item[], int, long*), Item v[], int n, long *comparacoes) {
    clock_t ini = clock();
    sort_fn(v, n, comparacoes);
    clock_t fim = clock();
    return ((double)(fim - ini) * 1000.0) / CLOCKS_PER_SEC; // ms
}

/* ----- funções auxiliares para popular/exemplo ----- */
int preencher_exemplo(Item v[]) {
    const char *nomes[] = { "chip central","painel controle","suporte base","motor propulsor","fuselagem",
                           "antena","parafusoM3","bateria","sensor termo","placa logica" };
    const char *tipos[] = { "controle","controle","suporte","propulsao","suporte",
                            "comunicacao","fixacao","energia","sensor","controle" };
    int prios[] = {9,8,5,10,4,6,2,9,3,8};
    int n = 10;
    for (int i = 0; i < n; ++i) {
        strncpy(v[i].nome, nomes[i], NAME_LEN-1); v[i].nome[NAME_LEN-1]=0;
        strncpy(v[i].tipo, tipos[i], TYPE_LEN-1); v[i].tipo[TYPE_LEN-1]=0;
        v[i].prioridade = prios[i];
    }
    return n;
}
/* cadastrar manual */
int cadastrar_manual(Item v[]) {
    char buf[16];
    printf("Quantos itens (max %d)? ", MAX_ITEMS);
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    int n = atoi(buf); if (n < 1) n = 1; if (n > MAX_ITEMS) n = MAX_ITEMS;
    for (int i = 0; i < n; ++i) {
        printf("Item %d - nome: ", i);
        ler_string(v[i].nome, NAME_LEN);
        if (strlen(v[i].nome) == 0) strcpy(v[i].nome, "sem_nome");
        printf("Item %d - tipo: ", i);
        ler_string(v[i].tipo, TYPE_LEN);
        if (strlen(v[i].tipo) == 0) strcpy(v[i].tipo, "geral");
        int p = 0;
        do {
            printf("Prioridade (1-10): ");
            if (!fgets(buf, sizeof(buf), stdin)) { p = 1; break; }
            p = atoi(buf);
        } while (p < 1 || p > 10);
        v[i].prioridade = p;
    }
    return n;
}

/* ----- menu e fluxo principal ----- */
int main(void) {
    Item vetor[MAX_ITEMS];
    int n = 0;
    Node *lista = NULL;
    Item trabalho[MAX_ITEMS]; // vetor de trabalho
    srand((unsigned)time(NULL));

    printf("=== Listas e Ordenacao - Projeto de Estruturas (Profª Daisy) ===\n");

    printf("Escolha:\n1) Preencher exemplo\n2) Inserir manual\nOpcao: ");
    char op[8]; if (!fgets(op, sizeof(op), stdin)) return 0;
    if (op[0] == '1') n = preencher_exemplo(vetor);
    else n = cadastrar_manual(vetor);

    // popular lista com os mesmos itens
    for (int i = 0; i < n; ++i) lista_inserir(&lista, vetor[i]);

    int sair = 0;
    int ordenado_por_nome = 0;
    while (!sair) {
        printf("\n--- Menu Principal ---\n");
        printf("1) Mostrar vetor\n2) Mostrar lista encadeada\n3) Inserir item vetor\n4) Remover item vetor (por indice)\n5) Inserir item lista\n6) Remover item lista (por nome)\n7) Busca sequencial vetor (por nome)\n8) Busca sequencial lista (por nome)\n9) Ordenar vetor por nome (Bubble) e medir\n10) Ordenar vetor por tipo (Insertion) e medir\n11) Ordenar vetor por prioridade (Selection) e medir\n12) Busca binaria iterativa por nome (vetor) - requer ordenado por nome\n13) Busca binaria recursiva por nome (vetor) - requer ordenado por nome\n14) Comparar custo insercao/remocao (vetor vs lista) simples (medicoes)\n0) Sair\nEscolha: ");
        if (!fgets(op, sizeof(op), stdin)) break;
        int esc = atoi(op);
        if (esc == 1) mostrar_vetor(vetor, n);
        else if (esc == 2) mostrar_lista(lista);
        else if (esc == 3) {
            Item it; printf("Nome: "); ler_string(it.nome, NAME_LEN);
            printf("Tipo: "); ler_string(it.tipo, TYPE_LEN);
            char b[8]; int p=0; do { printf("Prioridade: "); ler_string(b, sizeof(b)); p=atoi(b); } while (p<1||p>10);
            it.prioridade = p;
            if (vetor_inserir(vetor, &n, it)) printf("Inserido no vetor.\n"); else printf("Vetor cheio.\n");
        } else if (esc == 4) {
            char b[8]; printf("Indice a remover: "); ler_string(b, sizeof(b)); int idx = atoi(b);
            if (vetor_remover(vetor, &n, idx)) printf("Removido.\n"); else printf("Indice invalido.\n");
        } else if (esc == 5) {
            Item it; printf("Nome: "); ler_string(it.nome, NAME_LEN);
            printf("Tipo: "); ler_string(it.tipo, TYPE_LEN);
            char b[8]; int p=0; do { printf("Prioridade: "); ler_string(b, sizeof(b)); p=atoi(b); } while (p<1||p>10);
            it.prioridade = p;
            lista_inserir(&lista, it); printf("Inserido na lista.\n");
        } else if (esc == 6) {
            char nome[NAME_LEN]; printf("Nome a remover da lista: "); ler_string(nome, NAME_LEN);
            if (lista_remover(&lista, nome)) printf("Removido da lista.\n"); else printf("Nao encontrado.\n");
        } else if (esc == 7) {
            char nome[NAME_LEN]; printf("Nome p/ busca no vetor: "); ler_string(nome, NAME_LEN);
            long comps = 0;
            int idx = vetor_busca_seq(vetor, n, nome, &comps);
            if (idx >= 0) printf("Encontrado no indice %d. Comparacoes: %ld\n", idx, comps);
            else printf("Nao encontrado. Comparacoes: %ld\n", comps);
        } else if (esc == 8) {
            char nome[NAME_LEN]; printf("Nome p/ busca na lista: "); ler_string(nome, NAME_LEN);
            long comps = 0;
            Node *res = lista_busca_seq(lista, nome, &comps);
            if (res) printf("Encontrado: %s | %s | %d  (Comparacoes: %ld)\n", res->dados.nome, res->dados.tipo, res->dados.prioridade, comps);
            else printf("Nao encontrado. Comparacoes: %ld\n", comps);
        } else if (esc == 9) {
            memcpy(trabalho, vetor, sizeof(Item)*n);
            long comps = 0; double ms = medir_tempo_sort(bubble_sort_nome, trabalho, n, &comps);
            printf("Bubble sort por nome: comparacoes=%ld tempo=%.3f ms\n", comps, ms);
            mostrar_vetor(trabalho, n);
            // atualizar vetor para versão ordenada
            memcpy(vetor, trabalho, sizeof(Item)*n);
            ordenado_por_nome = 1;
        } else if (esc == 10) {
            memcpy(trabalho, vetor, sizeof(Item)*n);
            long comps = 0; double ms = medir_tempo_sort(insertion_sort_tipo, trabalho, n, &comps);
            printf("Insertion sort por tipo: comparacoes=%ld tempo=%.3f ms\n", comps, ms);
            mostrar_vetor(trabalho, n);
            memcpy(vetor, trabalho, sizeof(Item)*n);
            ordenado_por_nome = 0;
        } else if (esc == 11) {
            memcpy(trabalho, vetor, sizeof(Item)*n);
            long comps = 0; double ms = medir_tempo_sort(selection_sort_prioridade, trabalho, n, &comps);
            printf("Selection sort por prioridade (desc): comparacoes=%ld tempo=%.3f ms\n", comps, ms);
            mostrar_vetor(trabalho, n);
            memcpy(vetor, trabalho, sizeof(Item)*n);
            ordenado_por_nome = 0;
        } else if (esc == 12) {
            if (!ordenado_por_nome) { printf("Vetor precisa estar ordenado por nome. Ordenar agora? (s/n): "); char r[8]; ler_string(r, sizeof(r)); if (r[0]=='s'||r[0]=='S') { memcpy(trabalho, vetor, sizeof(Item)*n); long c=0; medir_tempo_sort(bubble_sort_nome, trabalho, n, &c); memcpy(vetor, trabalho, sizeof(Item)*n); ordenado_por_nome = 1; } else continue; }
            char chave[NAME_LEN]; printf("Nome chave para busca binaria (iterativa): "); ler_string(chave, NAME_LEN);
            long comps = 0;
            int idx = busca_binaria_iter(vetor, n, chave, &comps);
            if (idx >= 0) printf("Encontrado idx=%d. Comparacoes=%ld\n", idx, comps);
            else printf("Nao encontrado. Comparacoes=%ld\n", comps);
        } else if (esc == 13) {
            if (!ordenado_por_nome) { printf("Vetor precisa estar ordenado por nome. Ordenar agora? (s/n): "); char r[8]; ler_string(r, sizeof(r)); if (r[0]=='s'||r[0]=='S') { memcpy(trabalho, vetor, sizeof(Item)*n); long c=0; medir_tempo_sort(bubble_sort_nome, trabalho, n, &c); memcpy(vetor, trabalho, sizeof(Item)*n); ordenado_por_nome = 1; } else continue; }
            char chave[NAME_LEN]; printf("Nome chave para busca binaria (recursiva): "); ler_string(chave, NAME_LEN);
            long comps = 0;
            int idx = busca_binaria_rec(vetor, n, chave, &comps);
            if (idx >= 0) printf("Encontrado idx=%d. Comparacoes=%ld\n", idx, comps);
            else printf("Nao encontrado. Comparacoes=%ld\n", comps);
        } else if (esc == 14) {
            /* Medicao simples: inserir/remover N vezes e contar custo aproximado (ops de movimento e comparacoes).
               Para vetor, custo de insercao/remocao no meio = deslocamentos; para lista, custo = percorrer (n) em média.
               Aqui fazemos uma simulacao básica comparativa. */
            int trials = 1000;
            printf("Comparando custo (simples) em %d repeticoes...\n", trials);
            // vetor: inserir no inicio (deslocamento)
            long custo_v_in = 0, custo_v_rm = 0;
            for (int t = 0; t < trials; ++t) {
                // inserir: deslocar n elementos (simulado por n)
                custo_v_in += n;
                // remover index 0: deslocar (n-1)
                custo_v_rm += n;
            }
            // lista: inserir/remover no inicio custo O(1) mas buscar posição custa O(n)
            long custo_l_in = 0, custo_l_rm = 0;
            for (int t = 0; t < trials; ++t) {
                // simular que precisamos buscar posição (n/2) em media
                custo_l_in += n/2;
                custo_l_rm += n/2;
            }
            printf("Vetor: custo_insercao(simulado)=%ld custo_remocao(simulado)=%ld\n", custo_v_in, custo_v_rm);
            printf("Lista: custo_insercao(simulado)=%ld custo_remocao(simulado)=%ld\n", custo_l_in, custo_l_rm);
            printf("Interpretacao: vetor realiza muitos deslocamentos; lista tem custo de percurso (ponteiros).\n");
        } else if (esc == 0) sair = 1;
        else printf("Opcao invalida.\n");
    }

    lista_liberar(&lista);
    printf("Encerrando.\n");
    return 0;
}
