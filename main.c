#include "loja.h"

int main(){
    //Preencher o estoque com alguns produtos de teste
    srand(time(NULL));
    add("parafina", "OceanSoul Base Coat - Longa duracao", 22.50);
    add("parafina", "WaveRider Base Coat - Longa duracao", 22.50);

    add("leash", "OceanSoul Comp 6' - Leve e Rapido", 119.90);
    add("leash", "WaveRider Regular 7' - Dia a Dia", 110.00);
    
    add("quilha", "OceanSoul Performer M - Tri-fin FCS II", 450.00);
    add("quilha", "WaveRider Reactor S - Tri-fin Futures", 430.50);

    add("deck", "OceanSoul Pro Model 3 Pecas - Arch Bar", 190.00);
    add("deck", "WaveRider Flat Pad 2 Pecas - Sensibilidade", 175.50);

    //Loop de testes
    int opcao;
    do {
        printf("\n============================================ MENU PRINCIPAL - TATI SURF.CO ==========================+==================\n");
        printf(" [ 1 ] Visualizar Estoque Completo (organizacao por pilhas e caixas)\n");
        printf(" [ 2 ] Visualizar Lista de Venda (todos os produtos, ordenados por preco)\n");
        printf(" [ 3 ] Filtrar Lista de Venda por Faixa de Preco\n");
        printf(" [ 4 ] Filtrar Lista de Venda por Tipo de Produto\n");
        printf(" [ 5 ] Comprar Produto (remover do estoque e da lista de vendas)\n");
        printf(" [ 6 ] Processar Entrega\n");
        printf(" [ 7 ] Imprimir Fila de Entregas\n");
        printf(" [ 0 ] Sair do Programa\n");
        printf("=======================================================================================================+=================\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao); 
        float minimo, maximo;
        char codigo_digitado[20]; 
        
        switch (opcao) {
            case 1:
                imprimir_estoque_completo();
            break;
            
            case 2:
                imprimir_lista_venda();
            break;
            
            case 3:
                printf("\nDigite o PRECO minimo: ");
                scanf("%f", &minimo);
                printf("Digite o PRECO maximo: ");
                scanf("%f", &maximo);
                imprimir_por_preco(minimo, maximo);
            break;
            
            case 4:
                printf("\nDigite o TIPO de produto (parafina, leash, quilha, deck): ");
                scanf("%19s", codigo_digitado); 
                imprimir_por_tipo(codigo_digitado);
            break;
            
            case 5:
                printf("\nDigite o CODIGO do produto que deseja comprar: ");
                scanf("%19s", codigo_digitado); 

                no_produto* produto_comprado = remover_produto(codigo_digitado);

                if (produto_comprado != NULL) {
                    printf("\n\033[32mCOMPRA REALIZADA COM SUCESSO!\033[0m\n");
                    printf("  - Produto: %s\n", produto_comprado->descricao);
                    printf("  - Codigo: %s\n", produto_comprado->codigo);
                    printf("  - Preco: R$%.2f\n", produto_comprado->preco);
                    preparar_pedido(produto_comprado);

                }else{
                    printf("\033[31mA compra nao pode ser concluida.\033[0m\n");
                }
                break;

            case 6:
                processar_entrega();
                break;

            case 7:
                imprimir_fila_de_entregas();
                break;

            case 0:
                printf("\nSaindo do programa... Ate mais!\n");
                break;

            default:
                printf("\n\033[31mOpcao invalida! Tente novamente.\033[0m\n");
                break;
        }

    }while (opcao != 0);    

    return 0;
}