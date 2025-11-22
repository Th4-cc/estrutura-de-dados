/*
  War Estruturado - Versão em C

aluna: Thais Oliveira de Almeida
*/

/* Bibliotecas*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* --- Estrutura do território --- */
typedef struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* --- Protótipos (modularização por funções) --- */

/* Missões */
void criarVetorMissoes(char *missoes[], int *total);
void atribuirMissao(char **destino, char *missoes[], int totalMissoes, const char *playerColor);
/* verificarMissao mantém a assinatura pedida (aceita a string de missão que inclui
   prefixo com a cor do jogador: "Cor|descricao"). */
int verificarMissao(char *missao, Territorio *mapa, int tamanho);

/* Ataque */
void atacar(Territorio *atacante, Territorio *defensor);

/* Utilitárias */
Territorio *criarMapa(int n);
void liberarMemoria(Territorio *mapa, int n, char **missoesJogadores, int numJogadores);
void exibirMapa(Territorio *mapa, int n);
void exibirMissaoUmaVez(char *missao);

/* --- Implementação --- */

/* Preenche vetor de missões (apenas ponteiros para literais); total retorna via ponteiro */
void criarVetorMissoes(char *missoes[], int *total) {
    missoes[0] = "Conquistar 3 territorios";
    missoes[1] = "Eliminar todas as tropas da cor Vermelho";
    missoes[2] = "Controlar 5 territorios";
    missoes[3] = "Acumular 10 tropas";
    missoes[4] = "Conquistar Brasil";
    *total = 5;
}

/*
  atribuirMissao:
  - destino: ponteiro para char* (será alocado dinamicamente dentro da função)
  - missoes: vetor de descrições
  - totalMissoes: número de descrições
  - playerColor: cor do jogador (ex: "Azul")
  A missão armazenada terá formato: "Cor|Descricao" para que verificarMissao saiba de quem é.
*/
void atribuirMissao(char **destino, char *missoes[], int totalMissoes, const char *playerColor) {
    if (!destino || totalMissoes <= 0) return;
    int idx = rand() % totalMissoes;
    const char *texto = missoes[idx];

    /* montar string "Cor|Missao" */
    size_t len = strlen(playerColor) + 1 + strlen(texto) + 1; // cor + '|' + texto + '\0'
    char *buf = (char *) malloc(len);
    if (!buf) {
        fprintf(stderr, "Erro de alocacao em atribuirMissao\n");
        *destino = NULL;
        return;
    }
    snprintf(buf, len, "%s|%s", playerColor, texto);
    *destino = buf;
}

/*
  verificarMissao:
  - missao: string no formato "Cor|Descricao"
  - mapa: vetor de Territorio
  - tamanho: número de territórios
  Retorna 1 se missão cumprida, 0 caso contrário.
  NOTA: A função implementa uma lógica simples para cada descrição.
*/
int verificarMissao(char *missao, Territorio *mapa, int tamanho) {
    if (!missao || !mapa || tamanho <= 0) return 0;

    /* separar cor e texto */
    char copia[256];
    strncpy(copia, missao, sizeof(copia)-1);
    copia[sizeof(copia)-1] = '\0';
    char *sep = strchr(copia, '|');
    if (!sep) return 0;
    *sep = '\0';
    char *playerColor = copia;
    char *texto = sep + 1;

    /* condição 1: Conquistar 3 territorios -> ter >=3 territorios com a cor do jogador */
    if (strcmp(texto, "Conquistar 3 territorios") == 0) {
        int cont = 0;
        for (int i = 0; i < tamanho; ++i)
            if (strcmp(mapa[i].cor, playerColor) == 0) cont++;
        return cont >= 3;
    }

    /* condição 2: Eliminar todas as tropas da cor Vermelho -> nenhuma tropa existente com cor Vermelho */
    if (strcmp(texto, "Eliminar todas as tropas da cor Vermelho") == 0) {
        for (int i = 0; i < tamanho; ++i)
            if (strcmp(mapa[i].cor, "Vermelho") == 0 && mapa[i].tropas > 0) return 0;
        // Note: missão atribui-se a um player (playerColor), mas a instrução pede eliminar Vermelho.
        // Independentemente de quem é o jogador, a condição é global.
        return 1;
    }

    /* condição 3: Controlar 5 territorios -> ter >=5 territorios da sua cor */
    if (strcmp(texto, "Controlar 5 territorios") == 0) {
        int cont = 0;
        for (int i = 0; i < tamanho; ++i)
            if (strcmp(mapa[i].cor, playerColor) == 0) cont++;
        return cont >= 5;
    }

    /* condição 4: Acumular 10 tropas -> soma das tropas do jogador >= 10 */
    if (strcmp(texto, "Acumular 10 tropas") == 0) {
        int soma = 0;
        for (int i = 0; i < tamanho; ++i)
            if (strcmp(mapa[i].cor, playerColor) == 0) soma += mapa[i].tropas;
        return soma >= 10;
    }

    /* condição 5: Conquistar Brasil -> território nome "Brasil" deve ter cor igual ao jogador */
    if (strcmp(texto, "Conquistar Brasil") == 0) {
        for (int i = 0; i < tamanho; ++i)
            if (strcmp(mapa[i].nome, "Brasil") == 0) {
                return (strcmp(mapa[i].cor, playerColor) == 0 && mapa[i].tropas > 0);
            }
        return 0; // não encontrou Brasil
    }

    /* Se descrição não for reconhecida, retornar 0 */
    return 0;
}

/*
  atacar:
  - atacante e defensor: ponteiros para territórios
  Implementação simplificada:
  - valida ataque (cores diferentes)
  - rola um dado para cada (1..6)
  - se atacante vence (valor maior): transfere cor e metade das tropas do atacante ao defensor
  - se defensor vence ou empate: atacante perde 1 tropa
*/
void atacar(Territorio *atacante, Territorio *defensor) {
    if (!atacante || !defensor) return;

    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque inválido: mesmos donos (%s).\n", atacante->cor);
        return;
    }
    if (atacante->tropas <= 0) {
        printf("Ataque inválido: atacante sem tropas.\n");
        return;
    }

    int a_roll = (rand() % 6) + 1;
    int d_roll = (rand() % 6) + 1;
    printf("Rolagem -> Atacante %s: %d | Defensor %s: %d\n", atacante->nome, a_roll, defensor->nome, d_roll);

    if (a_roll > d_roll) {
        // atacante vence: transfere cor e metade das tropas (arredondar para baixo)
        int transfer = atacante->tropas / 2;
        if (transfer <= 0) transfer = 1; // garante pelo menos uma tropa se houver
        printf("Atacante vence! Transferindo %d tropas e cor '%s' para %s.\n", transfer, atacante->cor, defensor->nome);
        // atualiza defensor
        strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor)-1);
        defensor->cor[sizeof(defensor->cor)-1] = '\0';
        defensor->tropas = transfer;
        // diminui atacante
        atacante->tropas -= transfer;
        if (atacante->tropas < 0) atacante->tropas = 0;
    } else {
        // defensor vence ou empate: atacante perde 1 tropa
        printf("Defensor resiste! Atacante perde 1 tropa.\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

/* Cria mapa de exemplo (aloca dinamicamente) */
Territorio *criarMapa(int n) {
    if (n <= 0) return NULL;
    Territorio *mapa = (Territorio *) calloc(n, sizeof(Territorio));
    if (!mapa) return NULL;
    return mapa;
}

/* Exibe mapa (estado atual) */
void exibirMapa(Territorio *mapa, int n) {
    if (!mapa) return;
    printf("=== Mapa (Territorios: %d) ===\n", n);
    for (int i = 0; i < n; ++i) {
        printf("[%d] %s | Cor: %s | Tropas: %d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/* Exibe a missão apenas uma vez (chame no inicio) */
void exibirMissaoUmaVez(char *missao) {
    if (!missao) return;
    // missão está no formato "Cor|Texto", exibir somente o texto
    char *sep = strchr(missao, '|');
    if (!sep) {
        printf("Missao: %s\n", missao);
    } else {
        printf("Sua Missao: %s\n", sep + 1);
    }
}

/* Liberar memoria das missões alocadas e do mapa (mapa deve ter sido alocado com malloc/calloc) */
void liberarMemoria(Territorio *mapa, int n, char **missoesJogadores, int numJogadores) {
    if (mapa) free(mapa);
    if (missoesJogadores) {
        for (int i = 0; i < numJogadores; ++i) {
            free(missoesJogadores[i]);
        }
        free(missoesJogadores);
    }
}

/* --- Programa principal --- */
int main(void) {
    srand((unsigned) time(NULL));

    /* --- Configuração inicial --- */
    const int numTerritorios = 6; // mapa pequeno para demo
    Territorio *mapa = criarMapa(numTerritorios);
    if (!mapa) {
        fprintf(stderr, "Erro: nao foi possivel alocar mapa\n");
        return 1;
    }

    /* popular nomes, cores e tropas iniciais */
    strncpy(mapa[0].nome, "Brasil", sizeof(mapa[0].nome)-1);
    strncpy(mapa[0].cor, "Azul", sizeof(mapa[0].cor)-1);
    mapa[0].tropas = 4;

    strncpy(mapa[1].nome, "Argentina", sizeof(mapa[1].nome)-1);
    strncpy(mapa[1].cor, "Vermelho", sizeof(mapa[1].cor)-1);
    mapa[1].tropas = 3;

    strncpy(mapa[2].nome, "Chile", sizeof(mapa[2].nome)-1);
    strncpy(mapa[2].cor, "Neutro", sizeof(mapa[2].cor)-1);
    mapa[2].tropas = 2;

    strncpy(mapa[3].nome, "Peru", sizeof(mapa[3].nome)-1);
    strncpy(mapa[3].cor, "Azul", sizeof(mapa[3].cor)-1);
    mapa[3].tropas = 2;

    strncpy(mapa[4].nome, "Colombia", sizeof(mapa[4].nome)-1);
    strncpy(mapa[4].cor, "Vermelho", sizeof(mapa[4].cor)-1);
    mapa[4].tropas = 3;

    strncpy(mapa[5].nome, "Venezuela", sizeof(mapa[5].nome)-1);
    strncpy(mapa[5].cor, "Neutro", sizeof(mapa[5].cor)-1);
    mapa[5].tropas = 1;

    /* jogadores (cores) */
    const int numJogadores = 2;
    const char *playersColors[numJogadores];
    playersColors[0] = "Azul";
    playersColors[1] = "Vermelho";

    /* criar vetor de descrições de missões */
    char *descrMissoesStatic[10];
    int totalDescr = 0;
    criarVetorMissoes(descrMissoesStatic, &totalDescr);

    /* alocar array de char* para missões dos jogadores */
    char **missoesJogadores = (char **) calloc(numJogadores, sizeof(char *));
    if (!missoesJogadores) {
        fprintf(stderr, "Erro: nao foi possivel alocar missoesJogadores\n");
        free(mapa);
        return 1;
    }

    /* atribuir missão a cada jogador e exibir apenas uma vez */
    for (int i = 0; i < numJogadores; ++i) {
        atribuirMissao(&missoesJogadores[i], descrMissoesStatic, totalDescr, playersColors[i]);
        printf("Jogador %d (%s) recebeu sua missao:\n", i+1, playersColors[i]);
        exibirMissaoUmaVez(missoesJogadores[i]);
        printf("\n");
    }

    /* --- Loop do jogo (simples demo) --- */
    int turno = 0;
    int vencedor = -1;
    const int maxTurnos = 50; // para evitar loop infinito em demo

    while (turno < maxTurnos && vencedor == -1) {
        printf("=== Turno %d ===\n", turno+1);
        exibirMapa(mapa, numTerritorios);

        // Para demo, cada jogador faz um ataque aleatório válido (se existir um alvo inimigo)
        for (int pj = 0; pj < numJogadores; ++pj) {
            const char *cor = playersColors[pj];
            // encontrar um atacante (território do jogador com tropas > 0)
            int atacadorIdx = -1;
            for (int i = 0; i < numTerritorios; ++i) {
                if (strcmp(mapa[i].cor, cor) == 0 && mapa[i].tropas > 0) { atacadorIdx = i; break; }
            }
            if (atacadorIdx == -1) {
                printf("Jogador %s: sem territorios/tropas para atacar.\n", cor);
                continue;
            }

            // encontrar alvo inimigo aleatório
            int alvoIdx = -1;
            int start = rand() % numTerritorios;
            for (int offs = 0; offs < numTerritorios; ++offs) {
                int idx = (start + offs) % numTerritorios;
                if (strcmp(mapa[idx].cor, cor) != 0) {
                    alvoIdx = idx;
                    break;
                }
            }
            if (alvoIdx == -1) {
                printf("Jogador %s: nenhum alvo inimigo encontrado.\n", cor);
                continue;
            }

            printf("\nJogador %s ataca: %s -> %s\n", cor, mapa[atacadorIdx].nome, mapa[alvoIdx].nome);
            atacar(&mapa[atacadorIdx], &mapa[alvoIdx]);

            // Após cada ataque, verificar silenciosamente se algum jogador cumpriu a missão
            for (int check = 0; check < numJogadores; ++check) {
                if (verificarMissao(missoesJogadores[check], mapa, numTerritorios)) {
                    vencedor = check;
                    break;
                }
            }
            if (vencedor != -1) break;
        }

        if (vencedor != -1) break;

        turno++;
        printf("\n");
    }

    /* Resultado final e exibição de quem venceu (se houve) */
    printf("\n===== Fim do Jogo (turnos: %d) =====\n", turno+1);
    exibirMapa(mapa, numTerritorios);
    if (vencedor != -1) {
        printf(">>> Vencedor: Jogador %d (cor %s)\n", vencedor+1, playersColors[vencedor]);
        printf("Missao cumprida: ");
        char *sep = strchr(missoesJogadores[vencedor], '|');
        if (sep) printf("%s\n", sep+1); else printf("%s\n", missoesJogadores[vencedor]);
    } else {
        printf("Nenhum vencedor apos %d turnos.\n", maxTurnos);
    }

    /* Liberar memoria */
    liberarMemoria(mapa, numTerritorios, missoesJogadores, numJogadores);

    return 0;
}
