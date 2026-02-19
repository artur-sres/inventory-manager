//Header guard
#ifndef LOJA_H
#define LOJA_H

//Bibliotecas utilizadas
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

//Definicao das capacidaddes das caixas para cada tipo de produto
#define CAPACIDADE_CAIXA_PARAFINA 50
#define CAPACIDADE_CAIXA_LEASH 25
#define CAPACIDADE_CAIXA_QUILHA 10
#define CAPACIDADE_CAIXA_DECK 5

//Structs utilizadas 
typedef struct no_produto{ 
    char* codigo;
    char* tipo;
    char* descricao;
    float preco;
    struct no_produto* prox;
    struct no_produto* ant;
}no_produto;

typedef struct no_caixa{ 
    char* codigo;
    int capacidade;
    int qntd_itens;
    no_produto* inicio;
    no_produto* final;
    struct no_caixa* prox;
}no_caixa;

typedef struct no_pilha_de_caixas{ 
    char* tipo;
    no_caixa* topo;
    int qntd_caixas;
    int qntd_itens;
    struct no_pilha_de_caixas* prox;
    struct no_pilha_de_caixas* ant;
}no_pilha_de_caixas;

typedef struct no_pp{ 
    no_produto* produto; 
    struct no_pp* prox;
    struct no_pp* ant;
}no_pp;

typedef struct no_pedido_entrega{
    no_produto* produto_entregue; 
    char* nome_cliente;
    char* cpf;
    char* cep;
    char* rua;
    char* numero_casa;
    char* complemento;
    struct no_pedido_entrega* prox; 
}no_pedido_entrega;

//Prototipos das funcoes
no_pilha_de_caixas* achar_tipo(no_pilha_de_caixas* inicio_pilha, char* tipo); 
char* gerar_codigo(const char* prefixo); 
no_caixa* criar_nova_caixa(no_produto* novo_produto, const char* tipo); 
void inserir_produto_ordenado_na_caixa(no_produto* novo_produto, no_produto** inicio_caixa, no_produto** final_caixa); 
void inserir_produto_ordenado_na_lista_de_vendas(no_produto* novo_produto); 
void add(char* tipo, char* descricao, float preco); 
void imprimir_estoque_completo(); 
void imprimir_lista_venda(); 
void imprimir_por_tipo(char* tipo); 
void imprimir_por_preco(float min, float max); 
char* identificar_tipo_pelo_codigo(char* codigo_produto); 
no_produto* remover_produto(char* codigo);
void adicionar_na_fila_de_entrega(no_produto* produto, char* nome, char* cpf, char* cep, char* rua, char* numero, char* complemento); 
void preparar_pedido(no_produto* produto_comprado); 
void processar_entrega(); 
void imprimir_fila_de_entregas(); 

#endif

