# Gerenciador de Estoque - Loja de Surf

![C](https://img.shields.io/badge/C-Standard-blue?logo=c&logoColor=white)
![Data Structures](https://img.shields.io/badge/Estrutura%20de%20Dados-UFC-orange)

## Sobre o Projeto

O **Gerenciador de Estoque - Loja de Surf** é um projeto acadêmico desenvolvido em C, focado na aplicação prática de Estruturas de Dados avançadas. Ele simula o ciclo de vida completo dos produtos de uma loja especializada, gerenciando desde o recebimento e armazenamento até a venda e despacho para entrega.

Este trabalho foi realizado no **2º semestre** do curso de **Engenharia de Software** da **Universidade Federal do Ceará (UFC) - Campus Russas**, para a disciplina de Estrutura de Dados, sob orientação da **Profa. Dra. Tatiane Fernandes Figueiredo**.

## Funcionalidades Principais

O sistema oferece um menu interativo no console que permite realizar as seguintes operações:

* **Gerenciamento de Estoque e Catálogo:**
    * **Recebimento:** Adicionar novos produtos (Parafina, Leash, Quilha, Deck). O sistema organiza automaticamente o item na pilha correta e na lista de vendas.
    * **Visualização Completa:** Exibir a estrutura física do estoque, mostrando a organização interna das caixas e pilhas.
    * **Listagem de Vendas:** Ver o catálogo completo ordenado por preço (do menor para o maior).

* **Consultas e Filtros:**
    * Filtrar produtos por **faixa de preço** (Mínimo e Máximo).
    * Filtrar produtos por **tipo/categoria** específica.

* **Operações Comerciais:**
    * **Comprar:** Realizar a venda de um produto através do seu código único, removendo-o das pilhas de estoque e da lista de vendas.
    * **Fila de Entregas:** Ao finalizar uma compra, o pedido é adicionado a uma fila, contendo os dados do cliente e do produto.
    * **Processar Entrega:** Simular o despacho do pedido mais antigo da fila (FIFO - First In, First Out).

## Estrutura do Projeto

O código utiliza estruturas de dados específicas para resolver diferentes problemas de negócio:

* **Lista Duplamente Encadeada Ordenada:** Mantém o catálogo de vendas sempre ordenado por preço.
* **Pilhas (Stacks):** Simulam o estoque físico. Existem 4 pilhas (uma para cada tipo de produto), onde cada nó é uma caixa com capacidade limitada.
* **Fila (Queue):** Gerencia a ordem de saída das entregas.

Os arquivos estão organizados da seguinte forma:

* `main.c`: Arquivo principal que contém o menu interativo e a chamada para as funções do sistema.
* `loja.c`: Implementação de toda a lógica do sistema, manipulação das estruturas de dados (pilhas, listas e filas) e regras de negócio.
* `loja.h`: Arquivo de cabeçalho (header) contendo as definições das `structs`, constantes e protótipos das funções.

## Como Executar

1.  Certifique-se de ter um compilador C (como o GCC) instalado.
2.  Compile os arquivos `.c` gerando o executável.
    ```bash
    gcc -o loja main.c loja.c -Wall
    ```
3.  Execute o programa compilado.
    * No Linux/Mac:
        ```bash
        ./loja
        ```
    * No Windows:
        ```bash
        loja.exe
        ```

## Autor

Feito por **[Artur Saraiva Rabelo](https://github.com/artur-sres)**.

---
*Obs: A documentação deste projeto (README) foi gerada com auxílio de Inteligência Artificial.*
