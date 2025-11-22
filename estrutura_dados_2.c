/*
  Ordenacao de Componentes - Torre de Fuga
  - Bubble sort por nome (string)        -> conta comparações e mede tempo
  - Insertion sort por tipo (string)     -> conta comparações e mede tempo
  - Selection sort por prioridade (int)  -> conta comparações e mede tempo
  - Busca binária por nome (após ordenar por nome)
  - Cadastro de até 20 componentes
  - Menu interativo e exibição de estatísticas
 aluna: Thais Oliveira de Almeida
*/

/* Bibliotecas*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_COMP 20
#define NOME_LEN 30
#define TIPO_LEN 20

typedef struct {
    char nome[NOME_LEN];
    char tipo[TIPO_LEN];
    int prioridade;
} Componente;

/* ---------- utilitários ---------- */

/* remove newline do fgets */
void trim_newline(char *s) {
    size_t n = strlen(s);
    if (n == 0) return;
    if (s[n-1] == '\n') s[n-1] = '\0';
}

/* compara strings case-insensitive (retorna <0,0,>0 como strcmp) */
int stricmp_local(const char *a, const char *b) {
    for (; *a && *b; ++a, ++b) {
        char ca = tolower((unsigned char)*a);
        char cb = tolower((unsigned char)*b);
        if (ca != cb) return (int)(ca - cb);
    }
    return (int)(tolower((unsigned char)*a) - tolower((unsigned char)*b));
}

/* copia vetor de componentes (usado para preservar original) */
void copiar_vetor(Componente *src, Componente *dst, int n) {
    for (int i = 0; i < n; ++i) dst[i] = src[i];
}

/* exibe vetor formatado */
void mostrar_componentes(Componente v[], int n) {
    printf("Índice | Nome                          | Tipo               | Prioridade\n");
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < n; ++i) {
        printf("%5d  | %-28s | %-18s | %3d\n", i, v[i].nome, v[i].tipo, v[i].prioridade);
    }
}

/* ---------- algoritmos de ordenação ---------- */

/* Bubble sort por nome (string).
   Conta comparações via ponteiro long *comparacoes. */
void bubbleSortPorNome(Componente v[], int n, long *comparacoes) {
    long comps = 0;
    for (int i = 0; i < n-1; ++i) {
        int trocou = 0;
        for (int j = 0; j < n-1-i; ++j) {
            comps++;
            if (stricmp_local(v[j].nome, v[j+1].nome) > 0) {
                Componente tmp = v[j]; v[j] = v[j+1]; v[j+1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
    if (comparacoes) *comparacoes = comps;
}

/* Insertion sort por tipo (string). */
void insertionSortPorTipo(Componente v[], int n, long *comparacoes) {
    long comps = 0;
    for (int i = 1; i < n; ++i) {
        Componente key = v[i];
        int j = i - 1;
        // comparar tipos (case-insensitive)
        while (j >= 0) {
            comps++;
            if (stricmp_local(v[j].tipo, key.tipo) > 0) {
                v[j+1] = v[j];
                j--;
            } else break;
        }
        v[j+1] = key;
    }
    if (comparacoes) *comparacoes = comps;
}

/* Selection sort por prioridade (int).
   Ordena do maior para o menor (prioridade alta primeiro) — ajustar conforme necessidade. */
void selectionSortPorPrioridade(Componente v[], int n, long *comparacoes) {
    long comps = 0;
    for (int i = 0; i < n-1; ++i) {
        int idxMax = i;
        for (int j = i+1; j < n; ++j) {
            comps++;
            if (v[j].prioridade > v[idxMax].prioridade) idxMax = j;
        }
        if (idxMax != i) {
            Componente tmp = v[i]; v[i] = v[idxMax]; v[idxMax] = tmp;
        }
    }
    if (comparacoes) *comparacoes = comps;
}

/* ---------- busca binária por nome (após ordenar por nome) ---------- */
/* Retorna índice ou -1 se não encontrado. Conta comparações no ponteiro compsBin. */
int buscaBinariaPorNome(Componente v[], int n, const char chave[], long *comparacoes) {
    int lo = 0, hi = n - 1;
    long comps = 0;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        comps++;
        int cmp = stricmp_local(v[mid].nome, chave);
        if (cmp == 0) {
            if (comparacoes) *comparacoes = comps;
            return mid;
        } else if (cmp < 0) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    if (comparacoes) *comparacoes = comps;
    return -1;
}

/* ---------- temporizador genérico para medir tempo de execução ---------- */
/* Recebe ponteiro para função que recebe (Componente[], int, long*) */
typedef void (*alg_sort_fn)(Componente[], int, long*);

/* mede tempo em milissegundos e retorna via ponteiro tempo_ms e comparacoes */
void medir_tempo_e_comparacoes(alg_sort_fn algoritmo, Componente v[], int n, double *tempo_ms, long *comparacoes) {
    clock_t inicio = clock();
    algoritmo(v, n, comparacoes);
    clock_t fim = clock();
    if (tempo_ms) *tempo_ms = ((double)(fim - inicio) * 1000.0) / CLOCKS_PER_SEC;
}

/* ---------- entrada de componentes ---------- */

/* lê uma string com fgets e limpa newline */
void ler_string(char *buf, int len) {
    if (!fgets(buf, len, stdin)) {
        buf[0] = '\0';
        return;
    }
    trim_newline(buf);
}

/* cadastra componentes manualmente (até MAX_COMP) */
int cadastrar_componentes(Componente v[]) {
    int n = 0;
    char entrada[16];
    printf("Quantos componentes deseja cadastrar (max %d)? ", MAX_COMP);
    if (!fgets(entrada, sizeof(entrada), stdin)) return 0;
    n = atoi(entrada);
    if (n < 1) n = 1;
    if (n > MAX_COMP) n = MAX_COMP;

    for (int i = 0; i < n; ++i) {
        printf("\n--- Componente %d ---\n", i);
        printf("Nome: ");
        ler_string(v[i].nome, NOME_LEN);
        if (strlen(v[i].nome) == 0) { strcpy(v[i].nome, "componente_sem_nome"); }

        printf("Tipo: ");
        ler_string(v[i].tipo, TIPO_LEN);
        if (strlen(v[i].tipo) == 0) { strcpy(v[i].tipo, "geral"); }

        char prio_str[8];
        int prio = 1;
        do {
            printf("Prioridade (1 a 10): ");
            ler_string(prio_str, sizeof(prio_str));
            prio = atoi(prio_str);
            if (prio < 1 || prio > 10) printf("Valor invalido. ");
        } while (prio < 1 || prio > 10);
        v[i].prioridade = prio;
    }
    return n;
}

/* preenche exemplos rapidamente */
int preencher_exemplo(Componente v[]) {
    int n = 10;
    const char *nomes[] = {"chip central","painel controle","suporte base","motor propulsor","fuselagem",
                           "antena", "parafusoM3", "bateria", "sensor termo", "placa logica"};
    const char *tipos[] = {"controle","controle","suporte","propulsao","suporte",
                           "comunicacao","fixacao","energia","sensor","controle"};
    int prios[] = {9,8,5,10,4,6,2,9,3,8};
    for (int i = 0; i < n; ++i) {
        strncpy(v[i].nome, nomes[i], NOME_LEN-1); v[i].nome[NOME_LEN-1]='\0';
        strncpy(v[i].tipo, tipos[i], TIPO_LEN-1); v[i].tipo[TIPO_LEN-1]='\0';
        v[i].prioridade = prios[i];
    }
    return n;
}

/* ---------- menu e fluxo principal ---------- */

int main(void) {
    Componente original[MAX_COMP];
    Componente trabalho[MAX_COMP];
    int n = 0;
    int sorted_by_name = 0; // flag para permitir busca binária
    srand((unsigned)time(NULL));

    printf("=== Sistema de Priorizacao para Torre de Fuga ===\n\n");
    printf("Voce deseja:\n1) Preencher com exemplo\n2) Inserir manualmente\nEscolha (1/2): ");
    char op[8];
    if (!fgets(op, sizeof(op), stdin)) return 0;
    if (op[0] == '1') {
        n = preencher_exemplo(original);
    } else {
        n = cadastrar_componentes(original);
    }

    // copia para area de trabalho
    copiar_vetor(original, trabalho, n);

    int sair = 0;
    while (!sair) {
        printf("\n--- Menu ---\n");
        printf("1) Mostrar componentes atuais\n");
        printf("2) Ordenar por nome (Bubble Sort)\n");
        printf("3) Ordenar por tipo (Insertion Sort)\n");
        printf("4) Ordenar por prioridade (Selection Sort)\n");
        printf("5) Busca binaria por nome (requer ordenacao por nome)\n");
        printf("6) Resetar para ordem original\n");
        printf("7) Exportar lista para arquivo (componentes.txt)\n");
        printf("0) Sair\n");
        printf("Escolha: ");
        if (!fgets(op, sizeof(op), stdin)) break;
        int escolha = atoi(op);

        if (escolha == 1) {
            mostrar_componentes(trabalho, n);
        } else if (escolha == 2) {
            copiar_vetor(original, trabalho, n);
            long comps = 0; double tempo = 0.0;
            medir_tempo_e_comparacoes(bubbleSortPorNome, trabalho, n, &tempo, &comps);
            printf("Ordenacao por nome (Bubble Sort) concluida.\nComparacoes: %ld | Tempo: %.3f ms\n", comps, tempo);
            mostrar_componentes(trabalho, n);
            sorted_by_name = 1;
        } else if (escolha == 3) {
            copiar_vetor(original, trabalho, n);
            long comps = 0; double tempo = 0.0;
            medir_tempo_e_comparacoes(insertionSortPorTipo, trabalho, n, &tempo, &comps);
            printf("Ordenacao por tipo (Insertion Sort) concluida.\nComparacoes: %ld | Tempo: %.3f ms\n", comps, tempo);
            mostrar_componentes(trabalho, n);
            sorted_by_name = 0;
        } else if (escolha == 4) {
            copiar_vetor(original, trabalho, n);
            long comps = 0; double tempo = 0.0;
            medir_tempo_e_comparacoes(selectionSortPorPrioridade, trabalho, n, &tempo, &comps);
            printf("Ordenacao por prioridade (Selection Sort) concluida.\nComparacoes: %ld | Tempo: %.3f ms\n", comps, tempo);
            mostrar_componentes(trabalho, n);
            sorted_by_name = 0;
        } else if (escolha == 5) {
            if (!sorted_by_name) {
                printf("AVISO: busca binaria so e valida apos ordenacao por nome.\nDeseja ordenar por nome agora? (s/n): ");
                char resposta[8];
                if (!fgets(resposta, sizeof(resposta), stdin)) break;
                if (resposta[0] == 's' || resposta[0] == 'S') {
                    copiar_vetor(original, trabalho, n);
                    long comps = 0; double tempo = 0.0;
                    medir_tempo_e_comparacoes(bubbleSortPorNome, trabalho, n, &tempo, &comps);
                    printf("Ordenado por nome. Comparacoes: %ld | Tempo: %.3f ms\n", comps, tempo);
                    sorted_by_name = 1;
                } else {
                    printf("Busca abortada.\n");
                    continue;
                }
            }
            char chave[NOME_LEN];
            printf("Informe o nome exato do componente para buscar: ");
            ler_string(chave, NOME_LEN);
            if (strlen(chave) == 0) {
                printf("Nome vazio, abortando busca.\n");
                continue;
            }
            long compsBin = 0;
            int idx = buscaBinariaPorNome(trabalho, n, chave, &compsBin);
            if (idx >= 0) {
                printf("Componente encontrado no indice %d: %s | %s | %d\n", idx, trabalho[idx].nome, trabalho[idx].tipo, trabalho[idx].prioridade);
                printf("Comparacoes na busca binaria: %ld\n", compsBin);
            } else {
                printf("Componente nao encontrado. Comparacoes: %ld\n", compsBin);
            }
        } else if (escolha == 6) {
            copiar_vetor(original, trabalho, n);
            sorted_by_name = 0;
            printf("Vetor resetado para a ordem original.\n");
        } else if (escolha == 7) {
            FILE *f = fopen("componentes.txt", "w");
            if (!f) { perror("Erro ao criar arquivo"); continue; }
            fprintf(f, "Nome|Tipo|Prioridade\n");
            for (int i = 0; i < n; ++i) {
                fprintf(f, "%s|%s|%d\n", trabalho[i].nome, trabalho[i].tipo, trabalho[i].prioridade);
            }
            fclose(f);
            printf("Lista exportada para componentes.txt\n");
        } else if (escolha == 0) {
            sair = 1;
        } else {
            printf("Opcao invalida.\n");
        }
    }

    printf("Encerrando programa.\n");
    return 0;
}
