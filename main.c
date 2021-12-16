/**
 *  Aplicação que permite efetuar a gestão das transações
 *  do sistema SIGA nos bares e cantinas do Politécnico de Leiria
 *  Autor:  - João Gonçalves <2211870@my.ipleiria.pt>
 *          - Tiago Amaro <2211862@my.ipleiria.pt>
*/
#include <stdio.h>
#include <stdlib.h>

#define MAX_ESCOLAS 5
#define MAX_UTILIZADORES 200
#define MAX_TRANSACOES 5000

typedef struct {
    int identificador;
    char nome[100];
    char abreviatura[10];
    char campus[20];
    char localidade[20];
} t_escola;

typedef struct {
    int identificador;
    int escola; //identificador da escola
    char nome[100];
    int NIF;
    char tipo[30]; // tipo de estudante (Estudante/Docente/Funcionário) uma estrutura?
    char email[100];
    float saldo; // isto ainda me vai dar problemas. eu metia um inteiro e fazer uma divisão por 100 para encontrar as casas decimais. só não faço já porque tenho medo que o stor me dê porrada
} t_utilizador;

typedef struct {
    int identificador;
    int utilizador; //indentificador do utilizador
    char tipo[30]; // tipo de transação (Carregamento/Pagamento) uma estrutura?
    float valor; // mesma cena do que o salto do t_utilizador;
    t_data data;
    t_hora hora;
} t_transacao;

typedef struct {
    int dia;
    int mes;
    int ano;
} t_data;

typedef struct {
    int hora;
    int minuto;
    int segundo;
} t_hora;

int ler_inteiro(int, int);
float ler_real(float, float);
void carregar_informacao(t_escola [], t_utilizador [], t_transacao []);

int main()
{
    t_escola escolas[MAX_ESCOLAS];
    t_utilizador utilizadores[MAX_UTILIZADORES];
    t_transacao transacoes[MAX_TRANSACOES];

    return 0;
}

/**
 * Lê um número inteiro com base nos valores mínimos
 * e máximos definidos.
 * TODO: Adicionar uma forma de passar uma mensagem costumizada
*/
int ler_numero(int min, int max)
{
    int numero = 0;

    do{
        printf("Indique um número: ");
        scanf("%d", &numero);

        if(numero < min || numero > max)
            printf("\n\nValor Inválido. Tente Novamente.\n\n");

    }while(numero < min || numero > max);
    return numero;
}

/**
 * Lê um número real com base nos valores mínimos
 * e máximos definidos.
 * TODO: Adicionar uma forma de passar uma mensagem costumizada
*/
float ler_real(float min, float max)
{
    float numero = 0;

    do{
        printf("Indique um número real: ");
        scanf("%f", &numero);

        if(numero < min || numero > max)
            printf("\n\nValor Inválido. Tente Novamente.\n\n");

    }while(numero < min || numero > max);
    return numero;
}

/**
 * Carrega a informação dos ficheiros binários para a memória
 * da aplicação
 * NOT IMPLEMENTED
*/
void carregar_informacao(t_escola escolas[], t_utilizador utilizadores[], t_transacao transacoes[])
{
    // TODO: Implementar função
}
