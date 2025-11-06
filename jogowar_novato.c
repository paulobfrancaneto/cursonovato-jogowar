#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>   

// Constantes
#define MAX_MISSAO_LEN 100 // Tamanho máximo para a string da missão
#define NUM_MISSOES 5

// Estrutura que representa um território no jogo
typedef struct {
    char nome[30]; 
    char cor[10];  // Dono do território (cor do exército)
    int tropas;    // Quantidade de tropas estacionadas
} Territorio;

// Estrutura para o Jogador, incluindo a Missão alocada dinamicamente
typedef struct {
    char cor[10];
    char *missao; // Ponteiro para a missão alocada dinamicamente (malloc)
} Jogador;


// Vetor estático de missões (Requisito: vetor de strings com 5+ missões)
const char *MISSOES[] = {
    "Conquistar 3 territórios seguidos.",
    "Eliminar todas as tropas da cor Azul.",
    "Conquistar 4 territórios que tenham 5 ou mais tropas.",
    "Eliminar todas as tropas da cor Vermelha.",
    "Conquistar no mínimo 6 territórios no total."
};


// ----------------------------------------------------
// PROTÓTIPOS DAS FUNÇÕES
// ----------------------------------------------------
Territorio* alocarTerritorios(int *tamanho);
void cadastrarTerritorios(Territorio *mapa, int tamanho, Jogador *j1, Jogador *j2);
void exibirTerritorios(Territorio *mapa, int tamanho);
void atribuirMissao(Jogador *jogador);
int verificarMissao(char *missao, Territorio *mapa, int tamanho, char *cor_jogador);
void atacar(Territorio *atacante, Territorio *defensor);
void exibirMissao(char *missao);
void liberarMemoria(Territorio *mapa, Jogador *j1, Jogador *j2);


// ----------------------------------------------------
// IMPLEMENTAÇÃO DAS FUNÇÕES MODULARES
// ----------------------------------------------------

Territorio* alocarTerritorios(int *tamanho) {
    int n;
    printf("Quantos territórios serão cadastrados? ");
    if (scanf("%d", &n) != 1 || n < 2) {
        printf("Número inválido ou insuficiente. Usando o padrão de 5 territórios.\n");
        n = 5;
    }
    *tamanho = n;
    Territorio *mapa = (Territorio*) calloc(n, sizeof(Territorio));
    if (mapa == NULL) {
        perror("Erro ao alocar memória para o mapa");
        exit(EXIT_FAILURE);
    }
    printf("Memória alocada dinamicamente para %d territórios.\n", n);
    return mapa;
}

void atribuirMissao(Jogador *jogador) {
    // 1. Sorteia um índice aleatório
    int indice_sorteado = rand() % NUM_MISSOES;

    // 2. Aloca dinamicamente o espaço para a string da missão (Requisito: malloc)
    // +1 para o terminador nulo '\0'
    jogador->missao = (char*) malloc(MAX_MISSAO_LEN * sizeof(char)); 
    
    if (jogador->missao == NULL) {
        perror("Erro ao alocar memória para a missão");
        exit(EXIT_FAILURE);
    }
    
    // 3. Copia a missão sorteada para a memória alocada (Requisito: strcpy)
    strcpy(jogador->missao, MISSOES[indice_sorteado]);
}

void exibirMissao(char *missao) {
    printf("🎯 Sua Missão Estratégica é: %s\n", missao);
}

int verificarMissao(char *missao, Territorio *mapa, int tamanho, char *cor_jogador) {
    // Lógica SIMPLES de verificação (Requisito funcional)
    
    // Contar territórios do jogador e do inimigo
    int territorios_jogador = 0;
    int tropas_inimigo = 0;
    
    // Encontrar a cor inimiga (assume-se apenas 2 cores neste exemplo)
    char cor_inimigo[10] = "";
    if (strcmp(cor_jogador, "Vermelho") == 0) {
        strcpy(cor_inimigo, "Azul");
    } else if (strcmp(cor_jogador, "Azul") == 0) {
        strcpy(cor_inimigo, "Vermelho");
    }

    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, cor_jogador) == 0) {
            territorios_jogador++;
        }
        if (strcmp(mapa[i].cor, cor_inimigo) == 0) {
            tropas_inimigo += mapa[i].tropas;
        }
    }

    // Lógica para as missões predefinidas:
    
    // Missão 2 ou 4: Eliminar todas as tropas de uma cor (condição de eliminação total)
    if (strstr(missao, "Eliminar todas as tropas da cor Azul") != NULL) {
        int tropas_restantes = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Azul") == 0) tropas_restantes += mapa[i].tropas;
        }
        return tropas_restantes == 0;
    }
     if (strstr(missao, "Eliminar todas as tropas da cor Vermelha") != NULL) {
        int tropas_restantes = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0) tropas_restantes += mapa[i].tropas;
        }
        return tropas_restantes == 0;
    }

    // Missão 5: Conquistar no mínimo 6 territórios no total
    if (strstr(missao, "Conquistar no mínimo 6 territórios") != NULL) {
        return territorios_jogador >= 6;
    }
    
    // As missões 1 e 3 (Conquistar X seguidos/com X tropas) exigiriam um rastreamento mais complexo 
    // com variáveis globais/struct Jogador, mas para simplificar:
    
    // Se não for nenhuma das anteriores, apenas vence se dominar a maioria
    if (territorios_jogador > tamanho / 2) {
        printf("[DEBUG] Missão padrão: Conquistou a maioria dos territórios.\n");
        return 1;
    }

    return 0; // Missão não cumprida
}

void cadastrarTerritorios(Territorio *mapa, int tamanho, Jogador *j1, Jogador *j2) {
    printf("\n--- CONFIGURAÇÃO INICIAL ---\n");
    // Configura os jogadores
    strcpy(j1->cor, "Vermelho");
    strcpy(j2->cor, "Azul");
    
    printf("Jogador 1 (Cor: %s) e Jogador 2 (Cor: %s).\n\n", j1->cor, j2->cor);
    
    // Distribui as cores e pede os dados
    for (int i = 0; i < tamanho; i++) {
        Territorio *t = &mapa[i]; 
        
        // Distribuição inicial de cores
        if (i % 2 == 0) {
            strcpy(t->cor, j1->cor); // Jogador 1 (Vermelho)
        } else {
            strcpy(t->cor, j2->cor); // Jogador 2 (Azul)
        }

        printf("--- Território #%d (Dono: %s) ---\n", i + 1, t->cor);
        printf("Nome do Território (sem espaços): ");
        scanf("%29s", t->nome); 
        
        do {
            printf("Quantidade de Tropas (mínimo 1): ");
            scanf("%d", &t->tropas);
        } while (t->tropas < 1);

        printf("\n");
    }
}

void exibirTerritorios(Territorio *mapa, int tamanho) {
    printf("\n==========================================\n");
    printf("      RELATÓRIO DE TERRITÓRIOS\n");
    printf("==========================================\n");
    printf("| N. | NOME                 | COR        | TROPAS |\n");
    printf("|----|----------------------|------------|--------|\n");
    
    for (int i = 0; i < tamanho; i++) {
        Territorio *t = (mapa + i); 
        printf("| %-2d | %-20s | %-10s | %-6d |\n", 
               i + 1,
               t->nome, 
               t->cor, 
               t->tropas);
    }
    printf("==========================================\n");
}

void atacar(Territorio *atacante, Territorio *defensor) {
    printf("\n--- SIMULAÇÃO DE ATAQUE ---\n");
    printf("ATACANTE: %s (%s, %d tropas) vs. DEFENSOR: %s (%s, %d tropas)\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);

    int dado_atacante = rand() % 6 + 1;
    int dado_defensor = rand() % 6 + 1;

    printf("Rolagem de Dados: Atacante (%d) | Defensor (%d)\n", dado_atacante, dado_defensor);

    if (dado_atacante > dado_defensor) {
        printf("RESULTADO: O ATACANTE %s VENCEU a rodada!\n", atacante->nome);
        defensor->tropas--; 

        if (defensor->tropas <= 0) {
            // CONQUISTA
            printf("\n!!! CONQUISTA DE TERRITÓRIO !!!\n");
            
            strcpy(defensor->cor, atacante->cor);

            int tropas_movidas = atacante->tropas / 2;
            if (tropas_movidas == 0 && atacante->tropas > 1) {
                tropas_movidas = 1;
            } else if (atacante->tropas == 1) {
                 tropas_movidas = 1; 
            }

            defensor->tropas = tropas_movidas;
            atacante->tropas -= tropas_movidas;
            
            printf("O território %s agora é controlado por %s com %d tropas iniciais.\n", 
                   defensor->nome, defensor->cor, defensor->tropas);
        }
        
    } else {
        printf("RESULTADO: O ATACANTE %s PERDEU a rodada! Atacante perde 1 tropa.\n", atacante->nome);
        atacante->tropas--;
        
        if (atacante->tropas <= 0) {
            printf("O território atacante %s ficou sem tropas!\n", atacante->nome);
        }
    }
}


void liberarMemoria(Territorio *mapa, Jogador *j1, Jogador *j2) {
    printf("\n--- Gerenciamento de Memória ---\n");
    
    // 1. Libera a memória da Missão do Jogador 1
    if (j1->missao != NULL) {
        free(j1->missao);
        j1->missao = NULL;
        printf("Memória da Missão do Jogador 1 liberada.\n");
    }

    // 2. Libera a memória da Missão do Jogador 2
    if (j2->missao != NULL) {
        free(j2->missao);
        j2->missao = NULL;
        printf("Memória da Missão do Jogador 2 liberada.\n");
    }

    // 3. Libera a memória do vetor de Territórios
    if (mapa != NULL) {
        free(mapa);
        printf("Memória alocada para os territórios liberada.\n");
    }
}

// ----------------------------------------------------
// FUNÇÃO PRINCIPAL
// ----------------------------------------------------

int main() {
    // Requisito: Inicializar o gerador de números aleatórios
    srand(time(NULL));

    // Variáveis de controle
    int num_territorios = 0;
    Territorio *mapa = NULL;
    char continuar_ataque;
    
    // Cria os jogadores (alocação estática para simplificar)
    Jogador jogador1;
    Jogador jogador2;
    int vencedor = 0;

    printf("==========================================\n");
    printf("     WAR Estruturado - Missões Estratégicas\n");
    printf("==========================================\n");

    // 1. Alocação Dinâmica do Mapa
    mapa = alocarTerritorios(&num_territorios);
    
    // 2. Cadastro dos Territórios (Inicializa cores dos jogadores)
    cadastrarTerritorios(mapa, num_territorios, &jogador1, &jogador2);
    
    // 3. Atribuição Dinâmica das Missões
    atribuirMissao(&jogador1); // Passagem por referência para atribuição
    atribuirMissao(&jogador2); // Passagem por referência para atribuição

    // 4. Exibição Inicial das Missões
    printf("\n--- MISSÕES ATRIBUÍDAS ---\n");
    printf("Jogador 1 (%s):\n", jogador1.cor);
    exibirMissao(jogador1.missao); // Passagem por valor (do ponteiro) para exibição
    printf("\nJogador 2 (%s):\n", jogador2.cor);
    exibirMissao(jogador2.missao);
    printf("--------------------------\n");

    // 5. Laço principal de Ataque
    do {
        exibirTerritorios(mapa, num_territorios);

        int idx_atacante, idx_defensor;
        
        printf("\n=====================\n");
        printf("   RODADA DE ATAQUE\n");
        printf("=====================\n");
        
        printf("Selecione o número do território ATACANTE (1 a %d): ", num_territorios);
        scanf("%d", &idx_atacante);

        printf("Selecione o número do território DEFENSOR (1 a %d): ", num_territorios);
        scanf("%d", &idx_defensor);
        
        // Validações
        if (idx_atacante < 1 || idx_atacante > num_territorios || 
            idx_defensor < 1 || idx_defensor > num_territorios || 
            idx_atacante == idx_defensor) {
            printf("Seleção de território inválida. Tente novamente.\n");
            continue;
        }
        
        Territorio *atacante = &mapa[idx_atacante - 1];
        Territorio *defensor = &mapa[idx_defensor - 1];

        // Requisito: Validar as escolhas para que o jogador não ataque um território da própria cor.
        if (strcmp(atacante->cor, defensor->cor) == 0) {
            printf("Você não pode atacar um território da sua própria cor (%s).\n", atacante->cor);
            continue;
        }

        if (atacante->tropas < 2) {
            printf("O território atacante precisa de no mínimo 2 tropas para iniciar um ataque.\n");
            continue;
        }

        // Simulação do Ataque
        atacar(atacante, defensor);

        // 6. Verificação de Missão (Requisito: Exibição condicional)
        // Passagem por referência para verificação (do ponteiro da missão)
        if (verificarMissao(jogador1.missao, mapa, num_territorios, jogador1.cor)) {
            vencedor = 1;
            break;
        }
        if (verificarMissao(jogador2.missao, mapa, num_territorios, jogador2.cor)) {
            vencedor = 2;
            break;
        }

        // Opção para continuar
        printf("\nDeseja realizar outro ataque? (s/n): ");
        scanf(" %c", &continuar_ataque);
        
    } while (continuar_ataque == 's' || continuar_ataque == 'S');
    
    // 7. Declaração do Vencedor
    printf("\n\n##########################################\n");
    if (vencedor > 0) {
        printf("!!! JOGADOR %d VENCEU O JOGO CUMPRINDO SUA MISSÃO!!!\n", vencedor);
    } else {
        printf("O jogo terminou sem um vencedor por missão.\n");
    }
    printf("##########################################\n");
    
    // 8. Liberação de toda a Memória Alocada Dinamicamente
    liberarMemoria(mapa, &jogador1, &jogador2);

    return 0;
}