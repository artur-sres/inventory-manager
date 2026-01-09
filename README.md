# Gerenciador-Estoque-Loja-de-Surf

![C](https://img.shields.io/badge/Standart-00599C?logo=c&logoColor=white)
![Estrutura de Dados](https://img.shields.io/badge/Estrutura%20de%20Dados-orange)
![Status](https://img.shields.io/badge/Status-Completo-brightgreen)
![UFC](https://img.shields.io/badge/Universidade%20Federal%20do%20Cear%C3%A1-blue)

## Sobre o Projeto

O projeto foi o trabalho final para a disciplina de Estrutura de Dados, ministrada pela Profa. Dra. Tatiane Fernandes Figueiredo.

Implementa um sistema em C para gerenciar o estoque, vendas e entregas de uma loja de surf. O sistema lida com o ciclo de vida completo dos produtos, desde o recebimento até a saída para entrega.


## Estruturas de Dados Utilizadas

O núcleo do projeto é a aplicação prática das seguintes estruturas de dados para resolver os desafios de gerenciamento:
* **Lista Duplamente Encadeada Ordenada:** Utilizada para manter o catálogo de produtos sempre organizado por preço em ordem crescente. Toda nova inserção mantém a ordenação da lista.

* **Pilhas:** Quatro pilhas são usadas para simular o estoque físico da loja, com uma pilha para cada categoria de produto (Parafina, Leash, Quilhas, Decks). A lógica de empilhar e desempilhar gerencia caixas de produtos com capacidades específicas.

* **Filas:** Utilizada para criar e gerenciar a ordem dos pedidos que foram vendidos e aguardam para sair para entrega, garantindo que o primeiro a ser vendido é o primeiro a ser entregue.


## Funcionalidades

* **Recebimento de Produtos:** Adiciona novos itens ao catálogo e ao estoque de forma organizada.

* **Gestão de Estoque:** Gerencia a capacidade das caixas no topo de cada pilha, criando novas caixas quando as atuais atingem o limite.

* **Consulta de Produtos:** Permite visualizar os produtos disponíveis por categoria ou por uma faixa de preço específica.

* **Processo de Venda:** Remove um produto do estoque (pilhas) e do catálogo de vendas (lista) através de seu código único.

* **Fila de Entrega:** Adiciona pedidos vendidos a uma fila de entrega com os dados do cliente para despacho.


## Como Executar o Projeto

**Pré-requisitos:**
* Compilador GCC (ou compatível). 
<br/><br/>  

1. **Clone o repositório:**
```bash
git clone https://github.com/artur-sres/Gerenciador-Estoque-Loja-de-Surf
```
2. **Navegue até a pasta do projeto:**
```bash
cd Gerenciador-Estoque-Loja-de-Surf
```
3. **Compile o programa:**
```bash
gcc -o executavel main.c loja.c -W all
```
4. **Execute o programa:**
```bash
./executavel
```
Ao executar, o programa exibirá um menu interativo no console com as opções disponíveis.

## Autor


Feito por **[Artur Saraiva Rabelo](https://github.com/artur-sres)**.
