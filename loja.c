#include "loja.h"

//Variaveis globais ================================================================================================================> 

no_pilha_de_caixas* inicio_pilhas = NULL; //Lista de pilha de caixas
no_pilha_de_caixas* final_pilhas = NULL;
int tamanho_pilhas = 0;

no_pp* inicio_pp = NULL; //Lista de ponteiros
no_pp* final_pp = NULL;
int tamanho_pp = 0;

no_pedido_entrega* inicio_fila_entregas = NULL; //Fila de pedidos de entrega
no_pedido_entrega* fim_fila_entregas = NULL;
int tamanho_fila_entregas = 0;

// Funcoes auxiliares ==============================================================================================================>

char* gerar_codigo(const char* prefixo) { 
    int numero_aleatorio = 1000 + (rand() % 9000);
    int tamanho_necessario = strlen(prefixo) + 1 + 4 + 1;
    char* codigo_final = malloc(tamanho_necessario);
    sprintf(codigo_final, "%s-%d", prefixo, numero_aleatorio);
    return codigo_final;
}

char* identificar_tipo_pelo_codigo(char* codigo_produto){ 
    if (strncmp(codigo_produto, "PRD-PAR", 7) == 0) {
        return "parafina";
    } else if (strncmp(codigo_produto, "PRD-LSH", 7) == 0) {
        return "leash";
    } else if (strncmp(codigo_produto, "PRD-QHA", 7) == 0) {
        return "quilha";
    } else if (strncmp(codigo_produto, "PRD-DCK", 7) == 0) {
        return "deck";
    }
    
    // Se não for nenhum dos tipos conhecidos, retorna NULL
    return NULL; 
}

// Funcoes de manipulacao de pilhas, listas e filas ================================================================================>

no_caixa* criar_nova_caixa(no_produto* novo_produto, const char* tipo){ 
    no_caixa* nova_caixa = malloc(sizeof(no_caixa));

    //Atualiza os campos da nova caixa
    nova_caixa->qntd_itens = 1; 
    nova_caixa->inicio = novo_produto;
    nova_caixa->final = novo_produto;
    nova_caixa->prox = NULL; 
    
    //Define a capacidade e o codigo da caixa de acordo com o tipo
    if (strcmp(tipo, "parafina") == 0) {
        nova_caixa->capacidade = CAPACIDADE_CAIXA_PARAFINA;
        nova_caixa->codigo =(gerar_codigo("CX-PAR"));
    } else if (strcmp(tipo, "leash") == 0) {
        nova_caixa->capacidade = CAPACIDADE_CAIXA_LEASH;
        nova_caixa->codigo =(gerar_codigo("CX-LSH"));
    } else if (strcmp(tipo, "quilha") == 0) {
        nova_caixa->capacidade = CAPACIDADE_CAIXA_QUILHA;
        nova_caixa->codigo =(gerar_codigo("CX-QHA"));
    } else if (strcmp(tipo, "deck") == 0) {
        nova_caixa->capacidade = CAPACIDADE_CAIXA_DECK;
        nova_caixa->codigo =(gerar_codigo("CX-DCK"));
    }
    return nova_caixa;
}

void inserir_produto_ordenado_na_caixa(no_produto* novo_produto, no_produto** inicio_caixa, no_produto** final_caixa){ //Funcao para inserir no_produto ordenado por preco dentro de uma caixas
    //Produto com menor preco
    if (novo_produto->preco < (*inicio_caixa)->preco) {
        novo_produto->prox = *inicio_caixa;
        (*inicio_caixa)->ant = novo_produto;
        novo_produto->ant = NULL;
        *inicio_caixa = novo_produto;

    //Produto com maior preco
    }else if(novo_produto->preco >= (*final_caixa)->preco) {
        novo_produto->ant = *final_caixa;
        (*final_caixa)->prox = novo_produto;
        novo_produto->prox = NULL;
        *final_caixa = novo_produto;

    //Produto com preco no meio
    }else{
        if ((novo_produto->preco - (*inicio_caixa)->preco) < ((*final_caixa)->preco - novo_produto->preco)){
            no_produto* aux = *inicio_caixa;
            while((aux->prox->preco < novo_produto->preco)){
                aux=aux->prox;
            }
            aux->prox->ant=novo_produto;
            novo_produto->prox=aux->prox;
            aux->prox=novo_produto;
            novo_produto->ant=aux;
        }else{
            no_produto* aux = *final_caixa;
            while((aux->ant->preco > novo_produto->preco)){
                aux=aux->ant;
            }
            aux->ant->prox=novo_produto;
            novo_produto->ant=aux->ant;
            aux->ant=novo_produto;
            novo_produto->prox=aux;
        }
    }
}

no_pilha_de_caixas* achar_tipo(no_pilha_de_caixas* inicio_pilha, char* tipo) { 
    no_pilha_de_caixas* aux = inicio_pilha;
    while (aux != NULL) {
        if (strcmp(aux->tipo, tipo) == 0) {
            return aux;
        }
        aux = aux->prox;
    }
    return NULL;
}

void inserir_produto_ordenado_na_lista_de_vendas(no_produto* novo_produto){ 
    //Aloca memoria
    no_pp* novo_no_pp = (no_pp*) malloc(sizeof(no_pp));
    novo_no_pp->produto = novo_produto;
    novo_no_pp->prox = NULL;
    novo_no_pp->ant = NULL;

    //A lista de ponteiros está vazia
    if (inicio_pp == NULL) {
        inicio_pp = novo_no_pp;
        final_pp = novo_no_pp;
    
    //O produto deve ser inserido no início (menor preço)
    } else if (novo_produto->preco < inicio_pp->produto->preco) {
        novo_no_pp->prox = inicio_pp;
        inicio_pp->ant = novo_no_pp;
        inicio_pp = novo_no_pp;

    //O produto deve ser inserido no final (maior ou igual preço)
    } else if (novo_produto->preco >= final_pp->produto->preco) {
        novo_no_pp->ant = final_pp;
        final_pp->prox = novo_no_pp;
        final_pp = novo_no_pp;

    //O produto deve ser inserido no meio da lista
    } else {
        if ((novo_produto->preco - inicio_pp->produto->preco) < (final_pp->produto->preco - novo_produto->preco)) {
            no_pp* aux = inicio_pp;
            while (aux->prox != NULL && aux->prox->produto->preco < novo_produto->preco) {
                aux = aux->prox;
            }
            novo_no_pp->prox = aux->prox;
            novo_no_pp->ant = aux;
            if (aux->prox != NULL) {
                aux->prox->ant = novo_no_pp;
            }
            aux->prox = novo_no_pp;
        } else {
            no_pp* aux = final_pp;
            while (aux->ant != NULL && aux->ant->produto->preco > novo_produto->preco) {
                aux = aux->ant;
            }
            novo_no_pp->ant = aux->ant;
            novo_no_pp->prox = aux;
            if (aux->ant != NULL) {
                aux->ant->prox = novo_no_pp;
            }
            aux->ant = novo_no_pp;
        }
    }
    tamanho_pp++;
}

void adicionar_na_fila_de_entrega(no_produto* produto, char* nome, char* cpf, char* cep, char* rua, char* numero, char* complemento) { 
    //Aloca memoria para o novo no do pedido
    no_pedido_entrega* novo_pedido = (no_pedido_entrega*) malloc(sizeof(no_pedido_entrega));

    //Preenche os dados do pedido
    novo_pedido->produto_entregue = produto;
    novo_pedido->nome_cliente = strdup(nome);
    novo_pedido->cpf = strdup(cpf);
    novo_pedido->cep = strdup(cep);
    novo_pedido->rua = strdup(rua);
    novo_pedido->numero_casa = strdup(numero);
    novo_pedido->complemento = strdup(complemento);
    novo_pedido->prox = NULL; 

    //Adiciona o no no final da fila
    if(inicio_fila_entregas == NULL){ // Se a fila estava vazia
        inicio_fila_entregas = novo_pedido; 
        fim_fila_entregas = novo_pedido;    

    }else{ // Se a fila ja tinha itens
        fim_fila_entregas->prox = novo_pedido; 
        fim_fila_entregas = novo_pedido;       
    }
    printf("\n\033[32mPedido para '%s' foi adicionado a fila de entregas!\033[0m\n", nome);
}

// Funcoes que o usuario utiliza no menu ===========================================================================================>

void add(char* tipo, char* descricao, float preco){ 
    //Verifica se o tipo e valido
    if (strcmp(tipo, "parafina") != 0 && strcmp(tipo, "leash") != 0 && strcmp(tipo, "quilha") != 0 && strcmp(tipo, "deck") != 0) {
        printf("\n\033[31mERRO! O tipo de produto '\033[4m%s\033[0;31m' nao e valido.\033[0m\nTipos validos: \033[32;4mparafina\033[0m, \033[32;4mleash\033[0m, \033[32;4mquilha\033[0m, \033[32;4mdeck\033[0m\n\n", tipo);
        return;
    }

    //Criacao do no_produto
    no_produto* novo_produto = malloc(sizeof(no_produto)); 
    novo_produto->tipo=strdup(tipo);
    novo_produto->descricao=strdup(descricao);
    novo_produto->preco=preco;
    novo_produto->prox=NULL;
    novo_produto->ant=NULL;
    if(strcmp(tipo, "parafina") == 0){
        novo_produto->codigo=(gerar_codigo("PRD-PAR"));
    }else if(strcmp(tipo, "leash") == 0){
        novo_produto->codigo=(gerar_codigo("PRD-LSH"));
    }else if(strcmp(tipo, "quilha") == 0){
        novo_produto->codigo=(gerar_codigo("PRD-QHA"));
    }else if(strcmp(tipo, "deck") == 0){
        novo_produto->codigo=(gerar_codigo("PRD-DCK"));
    }
    
    //Adiciona o produto na lista de vendas
    inserir_produto_ordenado_na_lista_de_vendas(novo_produto);
    
    //Voltando para o estoque

    //Caso nao haja nenhuma pilha de caixas
    if(inicio_pilhas==NULL){
        //Cria umm novo no_caixa
        no_caixa* nova_caixa=criar_nova_caixa(novo_produto, tipo);

        //Adiciona uma nova pilha no no_pilha_de_caixas
        no_pilha_de_caixas* novo_tipo_pilha = malloc(sizeof(no_pilha_de_caixas));
        novo_tipo_pilha->tipo=strdup(tipo);
        novo_tipo_pilha->topo=nova_caixa;
        novo_tipo_pilha->qntd_caixas=1;
        novo_tipo_pilha->qntd_itens=1;
        novo_tipo_pilha->prox=NULL;
        novo_tipo_pilha->ant=NULL;
        
        //Inicializa a lista de no_pilha_de_caixas
        inicio_pilhas = novo_tipo_pilha;
        final_pilhas = novo_tipo_pilha;
        tamanho_pilhas++;
        
    //Caso a lista de no_pilha_de_caixas não esteja vazia
    }else{
        no_pilha_de_caixas* aux_tipo_pilha = achar_tipo(inicio_pilhas, tipo);

        //Caso ainda nao haja uma pilha para aquele tipo
        if (aux_tipo_pilha == NULL){
            //Cria uma nova no_caixa
            no_caixa* nova_caixa=criar_nova_caixa(novo_produto, tipo);
            
            //Adiciona uma nova pilha na lista de no_pilha_de_caixas
            no_pilha_de_caixas* novo_tipo_pilha = malloc(sizeof(no_pilha_de_caixas));
            novo_tipo_pilha->tipo=strdup(tipo);
            novo_tipo_pilha->topo=nova_caixa;
            novo_tipo_pilha->qntd_caixas=1;
            novo_tipo_pilha->qntd_itens=1;
            novo_tipo_pilha->prox=NULL;
            novo_tipo_pilha->ant=NULL;
        
            tamanho_pilhas++;
            final_pilhas->prox=novo_tipo_pilha;
            novo_tipo_pilha->ant=final_pilhas;
            final_pilhas=novo_tipo_pilha;
            
        //Caso haja uma pilha de caixa para aquele no_produto
        }else{
            //Ha espaco par aum novo item na no_caixa do topo
            if(aux_tipo_pilha->topo->qntd_itens < aux_tipo_pilha->topo->capacidade){
                inserir_produto_ordenado_na_caixa(novo_produto, &aux_tipo_pilha->topo->inicio, &aux_tipo_pilha->topo->final);
                aux_tipo_pilha->topo->qntd_itens++;
                
            //Nao ha espaco na no_caixa topo, deve ser criada uma nova no_caixa
            }else{
                no_caixa* nova_caixa=criar_nova_caixa(novo_produto, tipo);

                //Atualiza informacoes sobre aquela pilha de caixas
                nova_caixa->prox=aux_tipo_pilha->topo;
                aux_tipo_pilha->topo=nova_caixa;
                aux_tipo_pilha->qntd_caixas++;
            }
            aux_tipo_pilha->qntd_itens++;
        }
    }
}

no_produto* remover_produto(char* codigo){ 

    //Valida o produto e a pilha pelo tipo, caso não encontre, retorna NULL e informa ao usuario
    char* tipo = identificar_tipo_pelo_codigo(codigo);
    if (tipo == NULL) {
        printf("\n\033[31mERRO: Codigo de produto '%s' e invalido ou desconhecido!\033[0m\n", codigo);
        return NULL;
    }
    no_pilha_de_caixas* pilha_tipo = achar_tipo(inicio_pilhas, tipo); //Encontra a pilha correspondente ao tipo
    if (pilha_tipo == NULL) {
        printf("\n\033[33mAVISO: Nao ha estoque para produtos do tipo '%s.\033[0m\n", tipo);
        return NULL;
    }

    //Encontrar o produto
    no_caixa* pilha_auxiliar = NULL; 
    no_caixa* caixa_alvo = NULL; 
    no_produto* produto_encontrado = NULL;

    while (pilha_tipo->topo != NULL) { //Loop para procurar nas caixas 'manualmente' 
        no_caixa* caixa_atual = pilha_tipo->topo;
        pilha_tipo->topo = pilha_tipo->topo->prox; //Retira a caixa topo da pilha e transforma seu prox no novo topo
        caixa_atual->prox = NULL; 

        no_produto* produto_atual = caixa_atual->inicio;
        while (produto_atual != NULL) { //Loop para procurar o produto na caixa pelo codigo
            if (strcmp(produto_atual->codigo, codigo) == 0) {
                produto_encontrado = produto_atual;
                caixa_alvo = caixa_atual; 
                break; //Sai do loop de procurar na caixa atual
            }
            produto_atual = produto_atual->prox;
        }
        if (produto_encontrado != NULL) {
            break; //Sai do loop de desenpilhar caixas
        } else {
            caixa_atual->prox = pilha_auxiliar;
            pilha_auxiliar = caixa_atual;
        }
    }

    //Tratamento para caso o produto tenha sido encontrado ou nao
    if (produto_encontrado == NULL) { //Caso nao foi encontrado
        printf("\n\033[31mERRO: Produto com codigo '%s' nao encontrado no estoque.\033[0m\n", codigo);
        
        while (pilha_auxiliar != NULL) { // Devolve tudo da pilha auxiliar para a principal antes de sair
            no_caixa* caixa_a_devolver = pilha_auxiliar;
            pilha_auxiliar = pilha_auxiliar->prox;
            caixa_a_devolver->prox = pilha_tipo->topo;
            pilha_tipo->topo = caixa_a_devolver;
        }
        return NULL;
    }
    //Produto foi encontrado
    //Primeiro, removemos ele da lista de vendas
    //Logica para diminuir o numero de operacoes para saber se vai procurar a partir do inicio do final 
    if ((produto_encontrado->preco - inicio_pp->produto->preco) < (final_pp->produto->preco - produto_encontrado->preco)) { //Preco menor
        no_pp* no_pp_atual = inicio_pp;
        while (no_pp_atual != NULL) {
            if (no_pp_atual->produto == produto_encontrado) {
                if (tamanho_pp == 1) { inicio_pp = final_pp = NULL; }
                else if (no_pp_atual == inicio_pp) { inicio_pp = inicio_pp->prox; inicio_pp->ant = NULL; }
                else if (no_pp_atual == final_pp) { final_pp = final_pp->ant; final_pp->prox = NULL; }
                else { no_pp_atual->ant->prox = no_pp_atual->prox; no_pp_atual->prox->ant = no_pp_atual->ant; }
                free(no_pp_atual); tamanho_pp--; break;
            }
            no_pp_atual = no_pp_atual->prox;
        }
    } else { //Preco maior
        no_pp* no_pp_atual = final_pp;
        while (no_pp_atual != NULL) {
            if (no_pp_atual->produto == produto_encontrado) {
                if (tamanho_pp == 1) { inicio_pp = final_pp = NULL; }
                else if (no_pp_atual == inicio_pp) { inicio_pp = inicio_pp->prox; inicio_pp->ant = NULL; }
                else if (no_pp_atual == final_pp) { final_pp = final_pp->ant; final_pp->prox = NULL; }
                else { no_pp_atual->ant->prox = no_pp_atual->prox; no_pp_atual->prox->ant = no_pp_atual->ant; }
                free(no_pp_atual); tamanho_pp--; break;
            }
            no_pp_atual = no_pp_atual->ant;
        }
    }

    //Agora precisamos remover do estoque, retornar o produto removido, e fazer o balanceamento das caixas
    //Primeiro, removemos o produto da caixa
    if(caixa_alvo->qntd_itens == 1){ //Caso a caixa tenha apenas um produto
        caixa_alvo->inicio = NULL;
        caixa_alvo->final = NULL; 

    }else if(produto_encontrado == caixa_alvo->inicio){ //Produto era o primeiro da caixa
        caixa_alvo->inicio = produto_encontrado->prox; 
        caixa_alvo->inicio->ant = NULL; 

    }else if(produto_encontrado == caixa_alvo->final){ //Produto era o ultimo da caixa
        caixa_alvo->final = produto_encontrado->ant; 
        caixa_alvo->final->prox = NULL; 

    }else{ //Produto nao era o primeiro ou o ultimo
        produto_encontrado->ant->prox = produto_encontrado->prox; 
        produto_encontrado->prox->ant = produto_encontrado->ant; 
    }

    //Atualizamos as informacoes da pilha e isolamos o produto
    caixa_alvo->qntd_itens--;
    pilha_tipo->qntd_itens--;
    produto_encontrado->prox = NULL;
    produto_encontrado->ant = NULL;

    //Se a caixa-alvo ficou vazia
    if (caixa_alvo->qntd_itens == 0) {
        free(caixa_alvo->codigo);
        free(caixa_alvo);
        caixa_alvo = NULL; //Marcamos como nula para a próxima etapa
        pilha_tipo->qntd_caixas--;
    }

    //Reconstruir a pilha de caixas na ordem correta
    //Coloca a caixa_alvo (se ela ainda existir) de volta na pilha principal
    if (caixa_alvo != NULL) {
        caixa_alvo->prox = pilha_tipo->topo;
        pilha_tipo->topo = caixa_alvo;
    }
    //Depois, desempilha da pilha auxiliar para a pilha principal, colocando as caixas de volta no topo
    while (pilha_auxiliar != NULL) {
        no_caixa* caixa_a_devolver = pilha_auxiliar;
        pilha_auxiliar = pilha_auxiliar->prox;
        caixa_a_devolver->prox = pilha_tipo->topo;
        pilha_tipo->topo = caixa_a_devolver;
    }

    //Fazer o rebalanceamento
    //A logica e verificar se a caixa do topo pode receber um item da de baixo
    if (pilha_tipo->topo != NULL && pilha_tipo->topo->prox != NULL) {
        no_caixa* caixa_de_cima = pilha_tipo->topo;
        no_caixa* caixa_de_baixo = pilha_tipo->topo->prox;

        if (caixa_de_cima->qntd_itens < caixa_de_cima->capacidade && caixa_de_baixo->qntd_itens > 0) {

            //Pega o produto mais barato da caixa de baixo.
            no_produto* produto_movido = caixa_de_baixo->inicio;
            caixa_de_baixo->inicio = caixa_de_baixo->inicio->prox;

            if (caixa_de_baixo->inicio != NULL) {
                caixa_de_baixo->inicio->ant = NULL;

            }else{
                caixa_de_baixo->final = NULL;
            }

            caixa_de_baixo->qntd_itens--;
            produto_movido->prox = NULL; 
            produto_movido->ant = NULL;

            inserir_produto_ordenado_na_caixa(produto_movido, &caixa_de_cima->inicio, &caixa_de_cima->final);
            caixa_de_cima->qntd_itens++;

            //Se a caixa de baixo ficou vazia apos o balanceamento
            if (caixa_de_baixo->qntd_itens == 0) {
                caixa_de_cima->prox = caixa_de_baixo->prox; 
                free(caixa_de_baixo->codigo);
                free(caixa_de_baixo);
              pilha_tipo->qntd_caixas--;
            }
        }
    }
    return produto_encontrado;
}

void processar_entrega(){ 

    //Verifica se a fila esta vazia
    if (inicio_fila_entregas == NULL) {
        printf("\n\033[33mNao ha pedidos na fila de entrega.\033[0m\n");
        return;
    }

    //Guarda o pedido do inicio da fila em um ponteiro temporario
    no_pedido_entrega* pedido_a_remover = inicio_fila_entregas;

    //Imprime a mensagem e os dados do pedido que esta saindo
    printf("\n\033[32m--- Produto saindo para entrega... ---\033[0m\n");
    printf("Cliente: %s (CPF: %s)\n", pedido_a_remover->nome_cliente, pedido_a_remover->cpf);
    printf("Endereco: %s, %s - CEP: %s\n", pedido_a_remover->rua, pedido_a_remover->numero_casa, pedido_a_remover->cep);
    printf("Complemento: %s\n", pedido_a_remover->complemento);
    printf("Produto: %s (Codigo: %s)\n", pedido_a_remover->produto_entregue->tipo, pedido_a_remover->produto_entregue->codigo);
    printf("--------------------------------------\n");

    //Avanca o inicio da fila para o proximo item
    inicio_fila_entregas = inicio_fila_entregas->prox;

    //Se a fila ficou vazia apos a remocao, atualiza tambem o ponteiro do fim
    if (inicio_fila_entregas == NULL) {
        fim_fila_entregas = NULL;
    }

    //Libera toda a memoria que foi alocada para o pedido removido
    free(pedido_a_remover->nome_cliente);
    free(pedido_a_remover->cpf);
    free(pedido_a_remover->cep);
    free(pedido_a_remover->rua);
    free(pedido_a_remover->numero_casa);
    free(pedido_a_remover->complemento);
    free(pedido_a_remover->produto_entregue->codigo);
    free(pedido_a_remover->produto_entregue->descricao);
    free(pedido_a_remover->produto_entregue->tipo);
    free(pedido_a_remover->produto_entregue);
    free(pedido_a_remover);
}

// Interface =======================================================================================================================>

void imprimir_estoque_completo(){ 

    //A logica e bem simples, ele vai entrar em uma pilha, entrar na caixa topo e imprimir todos os produtos (que ja estao ordenados)
    //Depois ele passa para a proxima caixa e repete o processo ate nao ter mais caixas
    //E por ultima, passa para proxima pilha e faz todo o processo ate que nao hajam mais pilhas

    printf("=================================================== ESTOQUE COMPLETO ===================================================\n");
    if(inicio_pilhas==NULL){
        printf("\033[31mEstoque vazio!\033[0m\n");
        printf("========================================================================================================================\n");
        return;
    }
    printf("Temos %i tipos de produtos disponiveis:\n", tamanho_pilhas);
    
    no_pilha_de_caixas* aux_tipo_pilha=inicio_pilhas;
    while(aux_tipo_pilha!=NULL){
        printf("------------------------------------------------------------------------------------------------------------------------\n");
        printf("\033[34mTemos %i produto(s) do tipo \033[4m%s\033[0m\033[34m, distribuidos em uma pilha com %d caixa(s)\033[0m\n\n", aux_tipo_pilha->qntd_itens, aux_tipo_pilha->tipo, aux_tipo_pilha->qntd_caixas);
        no_caixa* caixa_atual=aux_tipo_pilha->topo;
        int n_caixa=aux_tipo_pilha->qntd_caixas;
        while(caixa_atual!=NULL){
            printf("\033[032mCaixa %i\033[0m(\033[033m%s\033[0m): %i produto(s)\n\n", n_caixa, caixa_atual->codigo, caixa_atual->qntd_itens);
            no_produto* produto_atual=caixa_atual->inicio;
            while(produto_atual!=NULL){
                printf("\033[4m%s\033[0m(\033[033m%s\033[0m)\nDescricao:%s\nPreco: R$%.2f\n\n", produto_atual->tipo, produto_atual->codigo, produto_atual->descricao, produto_atual->preco);
                produto_atual=produto_atual->prox;
            }
            if(caixa_atual->prox!=NULL){
                printf("........................................................................................................................\n\n");
            }
            caixa_atual=caixa_atual->prox;
            n_caixa--;
        }
        aux_tipo_pilha=aux_tipo_pilha->prox;
        if(aux_tipo_pilha != NULL) {
            printf("========================================================================================================================\n");
        }
    }
    printf("========================================================================================================================\n");
}

void imprimir_lista_venda(){ 
    //Uma simples impressao de uma lista encadeada
    printf("\n============================================= LISTA DE PRODUTOS PARA VENDA =============================================\n");
    if(inicio_pp == NULL){
        printf("\033[31mNenhum produto disponivel para venda!\033[0m\n");
        printf("========================================================================================================================\n");
        return;
    }
    
    no_pp* ponteiro_atual = inicio_pp;
    while(ponteiro_atual != NULL){
        printf("\033[4m%s\033[0m (\033[033m%s\033[0m)\n", ponteiro_atual->produto->tipo, ponteiro_atual->produto->codigo);
        printf("Descricao: %s\n", ponteiro_atual->produto->descricao);
        printf("Preco: R$%.2f\n\n", ponteiro_atual->produto->preco);
        ponteiro_atual = ponteiro_atual->prox;
    }
    printf("========================================================================================================================\n");
}

void imprimir_por_tipo(char* tipo){ 
    //Validação do tipo de entrada
    if (strcmp(tipo, "parafina") != 0 && strcmp(tipo, "leash") != 0 && strcmp(tipo, "quilha") != 0 && strcmp(tipo, "deck") != 0) {
        printf("\n\033[31mERRO! O tipo de produto '\033[4m%s\033[0;31m' nao e valido.\033[0m\nTipos validos: \033[32;4mparafina\033[0m, \033[32;4mleash\033[0m, \033[32;4mquilha\033[0m, \033[32;4mdeck\033[0m\n\n", tipo);
        return;
    }

    printf("\n============================================ CONSULTA DE PRODUTOS POR TIPO =============================================\n");
    printf("Categoria selecionada: \033[4m%s\033[0m\n\n", tipo);
    
    //Verifica se a lista principal está vazia
    if(inicio_pp == NULL){
        printf("\033[31mNenhum produto disponivel para venda!\033[0m\n");
        printf("========================================================================================================================\n");
        return;
    }
    
    no_pp* ponteiro_atual = inicio_pp;
    int encontrou_produto = 0; //Flag

    //Percorre a lista de produtos
    while(ponteiro_atual != NULL){
        //Se o tipo do produto for o desejado imprime
        if(strcmp(ponteiro_atual->produto->tipo, tipo) == 0){
            printf("\033[4m%s\033[0m (\033[033m%s\033[0m)\n", ponteiro_atual->produto->tipo, ponteiro_atual->produto->codigo);
            printf("Descricao: %s\n", ponteiro_atual->produto->descricao);
            printf("Preco: R$%.2f\n\n", ponteiro_atual->produto->preco);
            encontrou_produto = 1; //Marca que pelo menos um foi encontrado
        }
        ponteiro_atual = ponteiro_atual->prox;
    }

    //Se nenhum produto foi encontrado, avisa o usuário
    if(encontrou_produto == 0){
        printf("\033[33mNenhum produto encontrado para a categoria '%s'.\033[0m\n\n", tipo);
    }

    printf("========================================================================================================================\n");
}

void imprimir_por_preco(float min, float max){ 

    //Validação para garantir que o preço mínimo não é maior que o máximo
    if (min > max) {
        printf("\n\033[31mERRO! O preco minimo (R$%.2f) nao pode ser maior que o preco maximo (R$%.2f).\033[0m\n", min, max);
        return;
    }

    printf("\n=========================================== CONSULTA DE PRODUTOS POR PREÇO ===========================================\n");
    printf("Exibindo produtos na faixa de R$%.2f a R$%.2f\n\n", min, max);

    //Verifica se a lista de vendas está vazia
    if(inicio_pp == NULL){
        printf("\033[31mNenhum produto disponivel para venda!\033[0m\n");
        printf("========================================================================================================================\n");
        return;
    }
    
    no_pp* ponteiro_atual = inicio_pp;
    int encontrou_produto = 0; //Flag 

    //Percorre a lista de produtos
    while(ponteiro_atual != NULL){
        float preco_atual = ponteiro_atual->produto->preco;

        //Se o preço atual já é maior que o máximo, para o loop
        if (preco_atual > max) {
            break; 
        }

        //Imprime se estiver dentro do intervalo
        if(preco_atual >= min){
            printf("\033[4m%s\033[0m (\033[033m%s\033[0m)\n", ponteiro_atual->produto->tipo, ponteiro_atual->produto->codigo);
            printf("Descricao: %s\n", ponteiro_atual->produto->descricao);
            printf("Preco: R$%.2f\n\n", ponteiro_atual->produto->preco);
            encontrou_produto = 1; 
        }

        ponteiro_atual = ponteiro_atual->prox;
    }

    //Se nenhum foi encontrado
    if(encontrou_produto == 0){
        printf("\033[33mNenhum produto encontrado na faixa de preco de R$%.2f a R$%.2f.\033[0m\n\n", min, max);
    }

    printf("========================================================================================================================\n");
}

void imprimir_fila_de_entregas(){ 

    printf("\n============================================ FILA DE ENTREGAS PENDENTES ============================================\n");

    //Verifica se a fila esta vazia
    if (inicio_fila_entregas == NULL) {
        printf("A fila de entregas esta vazia.\n");
        printf("========================================================================================================================\n");
        return;
    }

    no_pedido_entrega* pedido_atual = inicio_fila_entregas;
    int posicao = 1;

    while (pedido_atual != NULL) {
        printf("--- Posicao na Fila: %d ---\n", posicao);
        printf("Cliente:     %s (CPF: %s)\n", pedido_atual->nome_cliente, pedido_atual->cpf);
        printf("Endereco:    %s, %s - CEP: %s\n", pedido_atual->rua, pedido_atual->numero_casa, pedido_atual->cep);
        printf("Produto:     %s (%s)\n", pedido_atual->produto_entregue->tipo, pedido_atual->produto_entregue->codigo);
        
        if(pedido_atual->prox != NULL){
            printf("........................................................................................................................\n");
        }

        pedido_atual = pedido_atual->prox;
        posicao++;
    }
    printf("========================================================================================================================\n");
}

void preparar_pedido(no_produto* produto_comprado){ 
    //Variaveis temporarias para armazenar os dados digitados
    char nome[100], cpf[15], cep[10], rua[100], numero[10], complemento[50];

    printf("\n--- PREPARANDO PEDIDO PARA ENTREGA ---\n");
    printf("Produto: %s (Codigo: %s)\n", produto_comprado->descricao, produto_comprado->codigo);
    printf("--------------------------------------\n");
    printf("Por favor, insira os dados do cliente para a entrega:\n\n");

    printf("Nome completo: ");
    scanf(" %[^\n]", nome);

    printf("CPF (apenas numeros): ");
    scanf("%14s", cpf);

    printf("CEP (apenas numeros): ");
    scanf("%9s", cep);

    printf("Nome da Rua: ");
    scanf(" %[^\n]", rua);

    printf("Numero da casa/apto: ");
    scanf("%9s", numero);

    printf("Complemento (se nao houver, digite 'nenhum'): ");
    scanf(" %[^\n]", complemento);

    adicionar_na_fila_de_entrega(produto_comprado, nome, cpf, cep, rua, numero, complemento);
}







