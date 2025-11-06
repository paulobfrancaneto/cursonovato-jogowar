#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h> 

// Estrutura que representa um território no jogo
typedef struct {
    char nome[30]; 
    char cor[10];  
    int tropas;   
} Territorio;

Territorio* alocarTerritorios(int *tamanho);
void cadastrarTerritorios(Territorio *mapa, int tamanho);
void exibirTerritorios(Territorio *mapa, int tamanho);
void atacar(Territorio *atacante, Territorio *defensor);
void liberarMemoria(Territorio *mapa);

// ----------------------------------------------------
// IMPLEMENTAÇÃO DAS FUNÇÕES MODULARES
// ----------------------------------------------------

Territorio* alocarTerritorios(int *tamanho) {
    int n;
    printf("Quantos territórios serão cadastrados? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Número inválido. Usando o padrão de 5 territórios.\n");
        n = 5;
    }
    *tamanho = n;

    // Uso de calloc para alocação dinâmica e inicialização com zeros
    Territorio *mapa = (Territorio*) calloc(n, sizeof(Territorio));

    if (mapa == NULL) {
        // Se a alocação falhar, informa e encerra o programa
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }
    printf("Memória alocada dinamicamente para %d territórios.\n", n);
    return mapa;
}

void cadastrarTerritorios(Territorio *mapa, int tamanho) {
    printf(">> Realizando o cadastro de %d territórios:\n\n", tamanho);

    for (int i = 0; i < tamanho; i++) {
        // Uso de aritmética de ponteiro para obter o endereço do struct atual
        Territorio *t = &mapa[i]; 

        printf("--- Território #%d ---\n", i + 1);

        printf("Nome do Território (sem espaços): ");
        scanf("%29s", t->nome); 

        printf("Cor do Exército (ex: Vermelho): ");
        scanf("%9s", t->cor);
        
        // Garante que o número de tropas seja no mínimo 1
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
        // Acesso aos campos via notação de ponteiro (->)
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

    // Simulação de rolagem de dados (rand() % 6 + 1 -> gera números de 1 a 6)
    int dado_atacante = rand() % 6 + 1;
    int dado_defensor = rand() % 6 + 1;

    printf("Rolagem de Dados: Atacante (%d) | Defensor (%d)\n", dado_atacante, dado_defensor);

    if (dado_atacante > dado_defensor) {
        // -------------------------
        // ATACANTE VENCEU O COMBATE
        // -------------------------
        printf("RESULTADO: O ATACANTE %s VENCEU a rodada!\n", atacante->nome);
        defensor->tropas--; // Defensor perde 1 tropa.

        if (defensor->tropas <= 0) {
            // CONQUISTA
            printf("\n!!! CONQUISTA DE TERRITÓRIO !!!\n");
            
            // 1. Mudar de dono (Cor)
            // Uso de strcpy para copiar a cor do atacante para o defensor
            strcpy(defensor->cor, atacante->cor);

            // 2. Transferir metade das tropas do ATACANTE para o DEFENSOR
            // A transferência é baseada nas tropas restantes do atacante.
            int tropas_movidas = atacante->tropas / 2;
            
            // Garantir que o atacante mova pelo menos 1 tropa para o território recém-conquistado
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
        // -------------------------
        // DEFENSOR VENCEU ou EMPATE
        // -------------------------
        printf("RESULTADO: O ATACANTE %s PERDEU a rodada! Atacante perde 1 tropa.\n", atacante->nome);
        atacante->tropas--; // Atacante perde 1 tropa.
        
        if (atacante->tropas <= 0) {
            printf("O território atacante %s ficou sem tropas!\n", atacante->nome);
        }
    }
}

void liberarMemoria(Territorio *mapa) {
    printf("\n--- Gerenciamento de Memória ---\n");
    // Uso obrigatório de free() no ponteiro base alocado
    if (mapa != NULL) {
        free(mapa);
        printf("Memória alocada para os territórios liberada com sucesso.\n");
    } else {
        printf("Nenhuma memória para liberar (ponteiro NULL).\n");
    }
}

// ----------------------------------------------------
// FUNÇÃO PRINCIPAL
// ----------------------------------------------------

int main() {
    // Requisito: Inicializar o gerador de números aleatórios para dados
    srand(time(NULL));

    // Variáveis de controle
    int num_territorios = 0;
    Territorio *mapa = NULL;
    char continuar_ataque;

    printf("==========================================\n");
    printf("     WAR Estruturado - Simulação de Batalha\n");
    printf("==========================================\n");

    // 1. Alocação Dinâmica e Obtenção do Tamanho
    mapa = alocarTerritorios(&num_territorios);
    
    // 2. Cadastro dos Territórios
    cadastrarTerritorios(mapa, num_territorios);
    
    // 3. Laço principal de Ataque
    do {
        exibirTerritorios(mapa, num_territorios);

        int idx_atacante, idx_defensor;
        
        printf("\n=====================\n");
        printf("   RODADA DE ATAQUE\n");
        printf("=====================\n");
        
        // Seleção do Atacante
        printf("Selecione o número do território ATACANTE (1 a %d): ", num_territorios);
        scanf("%d", &idx_atacante);

        // Seleção do Defensor
        printf("Selecione o número do território DEFENSOR (1 a %d): ", num_territorios);
        scanf("%d", &idx_defensor);
        
        // Validações
        if (idx_atacante < 1 || idx_atacante > num_territorios || 
            idx_defensor < 1 || idx_defensor > num_territorios) {
            printf("Seleção de território inválida. Tente novamente.\n");
            continue;
        }
        
        Territorio *atacante = &mapa[idx_atacante - 1];
        Territorio *defensor = &mapa[idx_defensor - 1];

        // Requisito: Não pode atacar a si mesmo
        if (idx_atacante == idx_defensor) {
            printf("Você não pode atacar seu próprio território.\n");
            continue;
        }

        // Requisito: Validar as escolhas para que o jogador não ataque um território da própria cor.
        if (strcmp(atacante->cor, defensor->cor) == 0) {
            printf("Você não pode atacar um território da sua própria cor (%s).\n", atacante->cor);
            continue;
        }

        // Requisito: Atacante deve ter pelo menos 2 tropas para atacar (1 de ataque + 1 de defesa)
        if (atacante->tropas < 2) {
            printf("O território atacante precisa de no mínimo 2 tropas para iniciar um ataque.\n");
            continue;
        }

        // 4. Simulação do Ataque
        atacar(atacante, defensor);

        // Opção para continuar
        printf("\nDeseja realizar outro ataque? (s/n): ");
        // O espaço antes do %c é importante para consumir possíveis caracteres de nova linha pendentes
        scanf(" %c", &continuar_ataque);
        
    } while (continuar_ataque == 's' || continuar_ataque == 'S');
    
    // 5. Liberação de Memória (Requisito fundamental)
    liberarMemoria(mapa);

    return 0;
}