#include <stdio.h>
#include <string.h>

#define NUM_TERRITORIOS 5

typedef struct {
    char nome[30];  
    char cor[10];   
    int tropas; 
} Territorio;


// ----------------------------------------------------
// 2. FUNÇÃO PRINCIPAL
// ----------------------------------------------------

int main() {
    
    // 3. DECLARAÇÃO DE VETOR DE STRUCTS
    // Cria um vetor com capacidade para 5 estruturas do tipo Territorio.
    Territorio mapa[NUM_TERRITORIOS];
    
    printf("==========================================\n");
    printf("       Sistema de Cadastro de Territórios (WAR)\n");
    printf("==========================================\n");
    
    // 4. ENTRADA DOS DADOS (Cadastro dos 5 territórios)
    printf(">> Realizando o cadastro de %d territórios:\n\n", NUM_TERRITORIOS);
    
    // Utiliza um laço 'for' para percorrer o vetor e preencher os dados.
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("--- Território #%d ---\n", i + 1);
        
        // Entrada do Nome (Usando scanf com cuidado para evitar whitespaces)
        printf("Nome do Território (sem espaços): ");
        scanf("%s", mapa[i].nome); 
        
        // Entrada da Cor
        printf("Cor do Exército (ex: Vermelho): ");
        scanf("%s", mapa[i].cor);
        
        // Entrada da Quantidade de Tropas
        printf("Quantidade de Tropas: ");
        // Uso de &mapa[i].tropas para passar o endereço do campo 'tropas'
        scanf("%d", &mapa[i].tropas); 
        
        printf("\n");
    }
    
    // ----------------------------------------------------
    // 5. EXIBIÇÃO DOS DADOS
    // ----------------------------------------------------
    
    printf("==========================================\n");
    printf("   RELATÓRIO DE TERRITÓRIOS CADASTRADOS\n");
    printf("==========================================\n");
    printf("| NÚMERO | NOME               | COR        | TROPAS |\n");
    printf("|--------|--------------------|------------|--------|\n");
    
    // Percorre o vetor de structs para exibir os dados registrados
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("| %-6d | %-18s | %-10s | %-6d |\n", 
               i + 1,
               mapa[i].nome, 
               mapa[i].cor, 
               mapa[i].tropas);
    }
    printf("==========================================\n");

    return 0;
}
