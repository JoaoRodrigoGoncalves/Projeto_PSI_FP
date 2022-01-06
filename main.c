/**
 *  Aplicação que permite efetuar a gestão das transações
 *  do sistema SIGA nos bares e cantinas do Politécnico de Leiria
 *  Autor:  - João Gonçalves <2211870@my.ipleiria.pt>
 *          - Tiago Amaro <2211862@my.ipleiria.pt>
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX_ESCOLAS 5
#define MAX_UTILIZADORES 200
#define MAX_TRANSACOES 5000

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

// utils

int ler_inteiro(char [], int, int);
float ler_real(char [], float, float);
void ler_string(char [], char [], int, int);
void aplicar_tabs(char [], int);

// menus

char menu();
char menu_escolas();
char confirmar_saida();

// outras funções

void carregar_informacao(t_escola [], t_utilizador [], t_transacao []);
void registar_escola(t_escola [], int *);
void consultar_escolas(t_escola [], int *);

int main()
{
    t_escola escolas[MAX_ESCOLAS];
    //t_utilizador utilizadores[MAX_UTILIZADORES];
    //t_transacao transacoes[MAX_TRANSACOES];
    int escolas_registadas = 0;
    //int utilizadores_registados = 0, transacoes_registadas = 0;
    char selecao_saida = 'n', selecao_saida_escolas = 'n';
    // TODO: carregar informação dos ficheiros

    do{
        switch(menu())
        {
            case 'e':
                do{
                    switch(menu_escolas())
                    {
                        case 'r':
                            registar_escola(escolas, &escolas_registadas);
                            break;

                        case 'c':
                            consultar_escolas(escolas, &escolas_registadas);
                            break;

                        case 'v':
                            selecao_saida_escolas = 's';
                            break;
                    }
                }while(selecao_saida_escolas != 's');
                break;

            case 'u':
                // menu utilizadores
                break;

            case 't':
                // menu transações
                break;

            case 's':
                selecao_saida = confirmar_saida();
                break;
        }
    }while(selecao_saida != 's');

    // Saída confirmada
    // TODO: guardar informação nos ficheiros
    return 0;
}

/**
 * Lê um número inteiro com base nos valores mínimos
 * e máximos definidos.
 * @param mensagem Mensagem customizada a apresentar ao utilizador
 * @param min Valor minimo aceitável
 * @param max Valor máximo aceitável
 * @return int
*/
int ler_numero(char mensagem[], int min, int max)
{
    int numero = 0;

    do{
        printf("%s: ", mensagem);
        scanf("%d", &numero);

        if(numero < min || numero > max)
            printf("\n\nValor Invalido. Tente Novamente.\n\n");

    }while(numero < min || numero > max);
    return numero;
}

/**
 * Lê um número real com base nos valores mínimos
 * e máximos definidos.
 * @param mensagem Mensagem customizada a apresentar ao utilizador
 * @param min Valor minimo aceitável
 * @param max Valor máximo aceitável
 * @return float
*/
float ler_real(char mensagem[], float min, float max)
{
    float numero = 0;
    do{
        printf("%s: ", mensagem);
        scanf("%f", &numero);

        if(numero < min || numero > max)
            printf("\n\nValor Invalido. Tente Novamente.\n\n");

    }while(numero < min || numero > max);
    return numero;
}

/**
 * Lê um vetor de chars (string).
 * Recebe um vetor de chars que é utilizado como forma de
 * pedir informação ao utlizador. Recebe ainda um valor
 * mínimo e máximo de caracteres a serem introduzidos pelo utilizador.
 * Para de registar quando o utilizador prime a tecla ENTER.
 * @param string Vetor para armazenar os dados inseridos pelo utilizador.
 * @param texto_questao O texto a ser mostrado ao utilizador.
 * @param min O mínimo de caracteres a serem inseridos pelo utilizador.
 * @param max O máximo de caracteres a serem inseridos pelo utilizador.
*/
void ler_string(char string[], char texto_questao[], int min, int max)
{
    int tamanho = 0;
    /**
     * damos 100 caracteres visto que o máximo alocado para string[] em todo o código será de 100 para os nomes.
     */
    char temp_string[100]; 

    do
    {
        printf("\n%s: ", texto_questao);
        scanf(" %99[^\n]", temp_string); // limitamos a entrada a 99 caracteres para permitir o '\0' e para impedir buffer overflow
        tamanho = strlen(temp_string);

        if(tamanho < min)
        {
            printf("\n\nO valor indicado e demasiado curto. Tente novamente!\n\n");
        }
        else if (tamanho > max)
        {
            printf("\n\nO valor indicado e demasiado comprido. Tente novamente!\n\n");
        }

    } while (tamanho < min || tamanho > max);
    strcpy(string, temp_string); // copia a string temporária correta para a string final
}

/**
 * @brief Utiliza o texto a ser escrito e o número de caracteres
 * que a string mais longa contém para determinar e escrever quantas
 * tabulações são necessárias para formatar a tabela corretamente.
 * Assume-se que uma tabulação é equivalente a 8 caracteres.
 * @param texto Texto a ser escrito
 * @param chars_da_string_longa O número de caracteres que a string mais comprida tem.
 */
void aplicar_tabs(char texto[], int chars_da_string_longa)
{
    int max_tabs = ceil(chars_da_string_longa/8);
    int tamanho_em_tabs = ceil(strlen(texto)/8); // obtém o tamanho de uma string em tabs (conjuntos de 8), arredondado por excesso.
    int tabs_string, contador;

    tabs_string = (max_tabs - tamanho_em_tabs)+1; // +1 para contar com o texto já escrito no cabeçalho da coluna.

    printf("%s", texto);
    for(contador = 0; contador < tabs_string; contador++)
    {
        printf("\t");
    }
}

/**
 * @brief Desenha o menu e pede uma opção ao utilizador
 * @return char A escolha do utilizador (minuscula)
 */
char menu()
{
    char escolha;
    do{
        printf("\n========= Estatisticas =========");
        printf("\nTotal Faturado: 0000.00 euros");
        printf("\n============ Menus =============");
        printf("\n[E] Menu Escolas");
        printf("\n[U] Menu Utilizadores");
        printf("\n[T] Menu Transacoes");
        printf("\n[S] Sair");
        printf("\n================================");

        printf("\nOpcao: ");
        scanf(" %c", &escolha);
        escolha = tolower(escolha);
        if(escolha != 's' && escolha != 'e' && escolha != 'u' && escolha != 't')
            printf("\n\nOpcao Desconhecida. Tente Novamente.\n");
    }while(escolha != 's' && escolha != 'e' && escolha != 'u' && escolha != 't');
    return escolha;
}

/**
 * @brief Desenha o menu de opções relacionadas com escolas
 * @return char A escolha do utilizador (minuscula)
 */
char menu_escolas()
{
    char escolha;
    do{
        printf("\n======== Escolas =========");
        printf("\n[R] Registar Escola");
        printf("\n[C] Consultar Lista");
        printf("\n[V] Voltar ao Menu Anterior");
        printf("\n==========================");

        printf("\nOpcao: ");
        scanf(" %c", &escolha);
        escolha = tolower(escolha);
        if(escolha != 'r' && escolha != 'c' && escolha != 'v')
            printf("\n\nOpcao Desconhecida. Tente Novamente.\n");
    }while(escolha != 'r' && escolha != 'c' && escolha != 'v');
    return escolha;
}

/**
 * @brief
 * Pede as informações necessárias e regista numa escola no vetor de estruturas t_escola
 * @param escolas Vetor de estruturas do tipo t_escola onde a informação será guardada.
 * @param num_registos (Ponteiro) Quantidade de registos já realizados. Utilizado para determinar
 * o índice do vetor onde a informação será guardada.
 */
void registar_escola(t_escola escolas[], int *num_registos)
{
    char nome[100];
    char abreviatura[10];
    char campus[20];
    char localidade[20];

    ler_string(nome, "Indique o nome da escola", 2, 99);
    ler_string(abreviatura, "Indique uma abreviatura para a escola", 1, 9);
    ler_string(campus, "Indique o nome do campus", 1, 19);
    ler_string(localidade, "Indique a localidade da escola", 1, 19);

    *num_registos = *num_registos + 1; //incrementa o número de registos existentes
    escolas[*num_registos-1].identificador = *num_registos; // o identificador único da escola é equivalente ao número de registos
    strcpy(escolas[*num_registos-1].nome, nome);
    strcpy(escolas[*num_registos-1].abreviatura, abreviatura);
    strcpy(escolas[*num_registos-1].campus, campus);
    strcpy(escolas[*num_registos-1].localidade, localidade);
}

/**
 * @brief Lista todas as escolas registadas
 * @param escolas Vetor de estruturas do tipo t_escola onde a informação está guardada.
 * @param num_registos (Ponteiro) Quantidade de registos já realizados. Utilizado para iterar pelo vetor.
 */
void consultar_escolas(t_escola escolas[], int *num_registos)
{
    int posicao, auxiliar, contador, max_char_nome = 0, max_char_abreviatura = 0, max_char_campus = 0, max_char_localidade = 0;
    if(*num_registos > 0)
    {
        for(posicao = 0; posicao < *num_registos; posicao++)
        {
            max_char_nome = (strlen(escolas[posicao].nome) > max_char_nome ? strlen(escolas[posicao].nome) : max_char_nome);
            max_char_abreviatura = (strlen(escolas[posicao].abreviatura) > max_char_abreviatura ? strlen(escolas[posicao].abreviatura) : max_char_abreviatura);
            max_char_campus = (strlen(escolas[posicao].campus) > max_char_campus ? strlen(escolas[posicao].campus) : max_char_campus);
            //a última coluna (localidade) não precisa de tabs
        }

        max_char_abreviatura = (strlen("Abreviatura") > max_char_abreviatura ? strlen("Abreviatura") : max_char_abreviatura); // normalmente a palavra "Abreviatura" é maior que as abreviaturas. Isto repara os tabs
        // a coluna dos campi não precisa de um calculo para a palavra "Campus" pois a função aplicar_tabs já conta com 1 tab a mais por causa do texto de cabeçalho

        printf("\n\nID\t");
        aplicar_tabs("Nome", max_char_nome);
        aplicar_tabs("Abreviatura", max_char_abreviatura);
        aplicar_tabs("Campus", max_char_campus);
        aplicar_tabs("Localidade", max_char_localidade);

        for(posicao = 0; posicao < *num_registos; posicao++)
        {
            printf("\n%d\t", escolas[posicao].identificador);
            aplicar_tabs(escolas[posicao].nome, max_char_nome);
            aplicar_tabs(escolas[posicao].abreviatura, max_char_abreviatura);
            aplicar_tabs(escolas[posicao].campus, max_char_campus);
            printf("%s", escolas[posicao].localidade); // a última coluna não precisa de tabs
        }
    }
    else
    {
        printf("\n\nNao existem escolas registadas\n\n");
    }
}

/**
 * @brief
 * Pergunta ao utilizador se tem a certeza que quer sair da aplicação
 * @return char 's' se confirma a saída, 'n' se não confirma a saída
 */
char confirmar_saida()
{
    char resposta;
    do{
        printf("\nTem a certeza que pretende sair (s/n)? ");
        scanf(" %c", &resposta);
        resposta = tolower(resposta);

        if(resposta != 's' && resposta != 'n')
            printf("\n\nValor Invalido. Tente Novamente!\n\n");

    }while(resposta != 's' && resposta != 'n');
    return resposta;
}

/**
 * Carrega a informação dos ficheiros binários para a memória
 * da aplicação
 * NOT IMPLEMENTED
*/
void carregar_informacao(t_escola escolas[], t_utilizador utilizadores[], t_transacao transacoes[])
{
    // TODO: Implementar função, criar verificação se já existem ficheiros gravados e função de criação de ficheiros
}
