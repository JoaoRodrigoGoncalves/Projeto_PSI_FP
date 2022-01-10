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
#define MAX_LINHAS_TABELA 10

typedef struct
{
    int dia;
    int mes;
    int ano;
} t_data;

typedef struct
{
    int hora;
    int minuto;
    int segundo;
} t_hora;

typedef struct
{
    int identificador;
    char nome[100];
    char abreviatura[10];
    char campus[20];
    char localidade[20];
} t_escola;

typedef struct
{
    int identificador;
    int escola;
    char nome[100];
    int NIF;
    char tipo[15]; // tipo de estudante (Estudante-1/Docente-2/Funcionário-3)
    char email[100];
    float saldo; // isto ainda me vai dar problemas. eu metia um inteiro e fazer uma divisão por 100 para encontrar as casas decimais. só não faço já porque tenho medo que o stor me dê porrada
} t_utilizador;

typedef struct
{
    int identificador;
    int utilizador; //indentificador do utilizador
    char tipo[30];
    float valor; // mesma cena do que o salto do t_utilizador;
    t_data data;
    t_hora hora;
} t_transacao;

// utils

int ler_inteiro(char [], int, int);
float ler_real(char [], float, float);
void ler_string(char [], char [], int, int);
void aplicar_tabs(char [], int);
char selecionar_opcao(char [], char []);
void listar_utilizadores_paginado(t_utilizador [], int *, t_escola [], int *);
char tratador_erros(char []);
int opcao_na_array(char [], char *);
void calcular_navegacao_tabela(char *, int *, int *, int *);

// menus

char menu();
char menu_escolas();
char menu_transacoes();
char menu_utilizadores();
char confirmar_saida();

// funções de dados

void carregar_escolas(t_escola [], int *);
void carregar_utilizadores(t_utilizador [], int *);
void carregar_transacoes(t_transacao [], int *);
void guardar_escolas(t_escola [], int *);
void guardar_utilizadores(t_utilizador [], int *);
void guardar_transacoes(t_transacao [], int *);

// outras funções

void registar_escola(t_escola [], int *);
void consultar_escolas(t_escola [], int *);
void registar_transacao(t_transacao [], int *, t_utilizador [], int *, t_escola [], int *);
void consultar_transacoes(t_transacao [], int *);
void registar_utilizadores(t_utilizador [],int *, t_escola [], int *);
void consultar_utilizadores(t_utilizador [], int *, t_escola [], int *);

int main()
{
    t_escola escolas[MAX_ESCOLAS];
    t_utilizador utilizadores[MAX_UTILIZADORES];
    t_transacao transacoes[MAX_TRANSACOES];
    int escolas_registadas = 0, utilizadores_registados = 0, transacoes_registadas = 0;
    char selecao_saida = 'n', selecao_saida_escolas = 'n', selecao_saida_transacoes = 'n', selecao_saida_utilizadores = 'n';
    
    carregar_escolas(escolas, &escolas_registadas);
    carregar_utilizadores(utilizadores, &utilizadores_registados);
    carregar_transacoes(transacoes, &transacoes_registadas);

    do{
        switch(menu())
        {
            case 'e':
                system("cls");
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
                            system("cls");
                            selecao_saida_escolas = 's';
                            break;
                    }
                }while(selecao_saida_escolas != 's');
                break;

            case 'u':
                system("cls");
                do{
                    switch(menu_utilizadores())
                    {
                        case 'r':
                            registar_utilizadores(utilizadores, &utilizadores_registados, escolas, &escolas_registadas);
                            break;

                        case 'c':
                            listar_utilizadores_paginado(utilizadores, &utilizadores_registados, escolas, &escolas_registadas);
                            break;

                        case 'v':
                            system("cls");
                            selecao_saida_utilizadores = 's';
                            break;
                    }
                }while(selecao_saida_utilizadores != 's');
                break;

            case 't':
                system("cls");
                do{
                    switch(menu_transacoes())
                    {
                        case 'r':
                            registar_transacao(transacoes, &transacoes_registadas, utilizadores, &utilizadores_registados, escolas, &escolas_registadas);
                            break;
                        
                        case 'c':
                            consultar_transacoes(transacoes, &transacoes_registadas);
                            break;

                        case 'v':
                            system("cls");
                            selecao_saida_transacoes = 's';
                            break;
                    }
                }while(selecao_saida_transacoes != 's');
                break;

            case 's':
                selecao_saida = confirmar_saida();
                break;
        }
    }while(selecao_saida != 's');

    guardar_escolas(escolas, &escolas_registadas);
    guardar_utilizadores(utilizadores, &utilizadores_registados);
    guardar_transacoes(transacoes, &transacoes_registadas);
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
int ler_inteiro(char mensagem[], int min, int max)
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

    do{
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

    }while (tamanho < min || tamanho > max);
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
 * @brief Permite ao utilizador selecionar uma opção da array opcoes. ** Não contém nenhum simbolo no final da mensagem de forma a permitir o uso de ? e : . Contém espaço**
 * @param mensagem Mensagem a ser apresentada ao utilizador
 * @param opcoes Array de opcoes que o utilizador pode escolher
 * @return char Opção selecionada pelo utilizador
 */
char selecionar_opcao(char mensagem[], char opcoes[])
{
    char selecao;

    do{
        printf("\n%s ", mensagem);
        scanf(" %c", &selecao);
        selecao = tolower(selecao);

        if(!opcao_na_array(opcoes, &selecao))
            printf("\nOpcao Invalida. Tente Novamente.");
    }while(!opcao_na_array(opcoes, &selecao));

    return selecao;
}

/**
 * @brief Verifica se um dado char (selecao) se encontra num dado vetor (opcoes)
 * @param opcoes array de opcoes
 * @param selecao opcao selecionada
 * @return int 1 - verdadeiro, 0 - falso
 */
int opcao_na_array(char opcoes[], char *selecao)
{
    int posicao;

    for(posicao = 0; posicao < sizeof(opcoes); posicao++)
    {
        if(opcoes[posicao] == *selecao)
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Calcula e atribui os devidos valores ao offset e pagina a serem aplicados à tabela
 * @param selecao A opção escolhida pelo utilizador (a - anterior, p - próxima)
 * @param offset Variável utilizada pelo for loop para ler pelo vetor
 * @param num_registo Quantidade de registos no vetor
 */
void calcular_navegacao_tabela(char *selecao, int *offset, int *num_registo, int *pagina)
{
    *offset = *offset - 1; // reduzir 1 por causa do for loop
    switch(*selecao)
    {
        case 'p':
            if((*offset + MAX_LINHAS_TABELA) < *num_registo)
            {
                *offset = *offset + 1;
                *pagina = *pagina + 1;
            }
            else
            {
                if((*offset + 1) == *num_registo)
                {
                    // offset = offset - ((linhas mostradas na página) - 1 linha da diferença do for loop)
                    *offset = *offset - (floor((double)*offset/MAX_LINHAS_TABELA)); //bug aqui
                }
                else
                {
                    *offset = *offset + 1;
                    *pagina = *pagina + 1;
                }
            }
            break;

        case 'a':
            if((*offset - (MAX_LINHAS_TABELA*2)) > 0)
            {
                if(*num_registo - *offset > MAX_LINHAS_TABELA)
                {
                    *offset = *offset - (MAX_LINHAS_TABELA*2);
                }
                else
                {
                    *offset = *offset - (MAX_LINHAS_TABELA + (*num_registo - *offset));
                }
                *pagina = *pagina - 1;
            }
            else
            {
                *offset = 0;
                if(*pagina == 2)
                    *pagina = 1;
            }
            break;
    }
}

/**
 * @brief Desenha o menu e pede uma opção ao utilizador
 * @return char A escolha do utilizador (minuscula)
 */
char menu()
{
    char escolha;
    printf("\n\n========= Estatisticas =========");
    printf("\nTotal Faturado: 0000.00 euros");
    printf("\n============ Menus =============");
    printf("\n[E] Menu Escolas");
    printf("\n[U] Menu Utilizadores");
    printf("\n[T] Menu Transacoes");
    printf("\n[S] Sair");
    printf("\n================================");
    escolha = selecionar_opcao("Opcao:", (char[4]){'e', 'u', 't', 's'});
    return escolha;
}

/**
 * @brief Desenha o menu de opções relacionadas com escolas
 * @return char A escolha do utilizador (minuscula)
 */
char menu_escolas()
{
    // Não é utilizado um system("cls") aqui de forma a que mensagens de erro possam ser apresentadas ao utilizador
    char escolha;
    printf("\n======== Escolas =========");
    printf("\n[R] Registar Escola");
    printf("\n[C] Consultar Lista");
    printf("\n[V] Voltar ao Menu Anterior");
    printf("\n==========================");
    escolha = selecionar_opcao("Opcao:", (char[3]){'r', 'c', 'v'});
    return escolha;
}

/**
 * @brief Desenha o menu de opções relacionadas aos utilizadores
 * @return char A escolha do utilizador converte a letra para minuscula
 */
char menu_utilizadores()
{
    char escolha;
    printf("\n======== Utilizadores =========");
    printf("\n[R] Registar Utilizador");
    printf("\n[C] Consultar Utilizadores");
    printf("\n[V] Voltar ao Menu Anterior");
    printf("\n==========================");
    escolha = selecionar_opcao("Opcao:", (char[3]){'r', 'c', 'v'});
    return escolha;
}

/**
 * @brief Desenha o menu de opções relacionadas com transações
 * @return char A escolha do utilizador (minuscula)
 */
char menu_transacoes()
{
    char escolha;
    printf("\n====== Transacoes =======");
    printf("\n[R] Registar Transacao");
    printf("\n[C] Consultar Lista");
    printf("\n[V] Voltar ao Menu Anterior");
    printf("\n=========================");
    escolha = selecionar_opcao("Opcao:", (char[3]){'r', 'c', 'v'});
    return escolha;
}

/**
 * @brief Pede as informações necessárias e regista uma escola no vetor de estruturas t_escola
 * @param escolas Vetor de estruturas do tipo t_escola onde a informação será guardada.
 * @param num_registos (Ponteiro) Quantidade de registos já realizados. Utilizado para determinar o índice do vetor onde a informação será guardada.
 */
void registar_escola(t_escola escolas[], int *num_registos)
{
    if(*num_registos < MAX_ESCOLAS)
    {
        printf("\n=== Registar Escola ===");
        ler_string(escolas[*num_registos].nome, "Indique o nome da escola", 2, 99);
        ler_string(escolas[*num_registos].abreviatura, "Indique uma abreviatura para a escola", 1, 9);
        ler_string(escolas[*num_registos].campus, "Indique o nome do campus", 1, 19);
        ler_string(escolas[*num_registos].localidade, "Indique a localidade da escola", 1, 19);

        escolas[*num_registos].identificador = *num_registos + 1 ; // o identificador único da escola é equivalente ao número de registos
        *num_registos = *num_registos + 1; //incrementa o número de registos existentes
        system("cls");
    }
    else
    {
        system("cls");
        printf("\nO limite de %d escolas foi excedido. Nao e possivel registar mais escolas.", MAX_ESCOLAS);
    }
}

/**
 * @brief Pede as informações necessárias e regista um utilizador no vetor de estruturas t_utilizador
 * @param utilizadores Vetor de estruturas do tipo t_utilizador onde a informação será guardada.
 * @param num_registos (Ponteiro) Quantidade de registos já realizados. Utilizado para determinar o índicedo vetor onde a informação será guardada.
 * @param lista_escolas Vetor de estruturas do tipo t_escola que é utilizado para apresentar a lista de escolas ao utilizador
 * @param registos_escolas (Ponteiro) Quantidade de escolas registadas
 * o índice do vetor onde a informação será guardada.
 */
void registar_utilizadores(t_utilizador utilizadores[], int *num_registos, t_escola lista_escolas[], int *registos_escolas)
{
    int selecao_tipo;
    if(*num_registos < MAX_UTILIZADORES && *registos_escolas > 0)
    {
        utilizadores[*num_registos].identificador = *num_registos + 1;
        consultar_escolas(lista_escolas, registos_escolas); // mostrar lista de escolas antes de pedir o ID da escola
        utilizadores[*num_registos].escola = ler_inteiro("\n=== Registar Utilizador ===\nInsira o identificador da escola", 1, 5); // passamos um pouco mais de texto aqui visto que a função consultar_escolas() limpa a consola visto que normalmente esta é chamada nos menus
        ler_string(utilizadores[*num_registos].nome, "Indique o nome", 3, 100);
        utilizadores[*num_registos].NIF  = ler_inteiro("Indique o NIF", 100000000, 299999999);
        selecao_tipo = ler_inteiro("Escolha o tipo de utilizador (Estudante - 1 | Docente - 2 | Funcionario - 3)", 1, 3);
        
        switch(selecao_tipo)
        {
            case 1: //estudante
                strcpy(utilizadores[*num_registos].tipo, "Estudante");
                break;

            case 2: //docente
                strcpy(utilizadores[*num_registos].tipo, "Docente");
                break;

            case 3: //funcionario
                strcpy(utilizadores[*num_registos].tipo, "Funcionario");
                break;
        }
        
        ler_string(utilizadores[*num_registos].email,"Indique o email", 6, 100);
        utilizadores[*num_registos].saldo = 0;
        *num_registos = *num_registos + 1;
        system("cls");
    }
    else
    {
        system("cls");
        printf("\nO limite de %d utilizadores foi excedido ou nao existem escolas registadas. Existem %d utilizadores registados.\nNao e possivel registar utilizador.", MAX_UTILIZADORES, (*num_registos + 1));
    }
}

/**
 * @brief Pede as informações necessárias e regista uma transação no vetor de estruturas t_transacao
 * @param transacoes Vetor de estruturas do tipo t_transacao onde a informação será guardada.
 * @param quantidade_registos_transacoes (Ponteiro) Quantidade de registos já realizados. Utilizado para determinar o índice do vetor onde a informação será guardada.
 * @param utilizadores Vetor de estruturas do tipo t_utilizador.
 * @param quantidade_registos_utilizadores (Ponteiro) Quantidade de registos já realizados.
 * @param escolas Vetor de estruturas do tipo t_escola.
 * @param quantidade_registos_escolas (Ponteiro) Quantidade de registos já realizados.
 */
void registar_transacao(t_transacao transacoes[], int *quantidade_registos_transacoes, t_utilizador utilizadores[], int *quantidade_registos_utilizadores, t_escola escolas[], int *quantidade_registos_escolas)
{
    t_transacao temp;
    int identificador_utilizador, selecao_tipo;
    if(*quantidade_registos_transacoes < MAX_TRANSACOES)
    {
        if(*quantidade_registos_escolas && *quantidade_registos_utilizadores) // se qualquer uma das variáveis for 0, a condição falha
        {
            printf("\n=== Registar Transacao ===\n");
            *quantidade_registos_transacoes = *quantidade_registos_transacoes + 1;
            temp.identificador = *quantidade_registos_transacoes;
            do{
                identificador_utilizador = ler_inteiro("Indique o identificador unico do utilizador (0 para lista)", 0, MAX_UTILIZADORES);
                if(identificador_utilizador == 0)
                    listar_utilizadores_paginado(utilizadores, quantidade_registos_utilizadores, escolas, quantidade_registos_escolas); // não é necessário utilizar * na varável pois já é o endereço do ponteiro
            }while(identificador_utilizador == 0);
            temp.utilizador = identificador_utilizador;
            selecao_tipo = ler_inteiro("Indique o tipo de transacao (1 - Pagameto, 2 - Carregamento)", 1, 2);

            switch(selecao_tipo)
            {
                case 1:
                    strcpy(temp.tipo, "Pagamento");
                    break;

                case 2:
                    strcpy(temp.tipo, "Carregamento");
                    break;
            }

            
            
        }
        else
        {
            system("cls");
            printf("\nNao e possivel registar transacoes. Confirme que existem utilizadores e/ou escolas registadas.\n");
        }
    }
    else
    {
        system("cls");
        printf("\nO limite de %d transacoes foi excedido. Nao e possivel registar mais transacoes.", MAX_TRANSACOES);
    }
}

/**
 * @brief Lista todas as escolas registadas
 * @param escolas Vetor de estruturas do tipo t_escola onde a informação está guardada.
 * @param num_registos (Ponteiro) Quantidade de registos já realizados. Utilizado para iterar pelo vetor.
 */
void consultar_escolas(t_escola escolas[], int *num_registos)
{
    system("cls");
    int posicao, max_char_nome = 0, max_char_abreviatura = 0, max_char_campus = 0, max_char_localidade = 0;
    if(*num_registos > 0)
    {
        for(posicao = 0; posicao < *num_registos; posicao++)
        {
            max_char_nome = ((int)strlen(escolas[posicao].nome) > max_char_nome ? (int)strlen(escolas[posicao].nome) : max_char_nome);
            max_char_abreviatura = ((int)strlen(escolas[posicao].abreviatura) > max_char_abreviatura ? (int)strlen(escolas[posicao].abreviatura) : max_char_abreviatura);
            max_char_campus = ((int)strlen(escolas[posicao].campus) > max_char_campus ? (int)strlen(escolas[posicao].campus) : max_char_campus);
            //a última coluna (localidade) não precisa de tabs
        }

        max_char_abreviatura = ((int)strlen("Abreviatura") > max_char_abreviatura ? (int)strlen("Abreviatura") : max_char_abreviatura); // normalmente a palavra "Abreviatura" é maior que as abreviaturas. Isto repara os tabs
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
 * @brief Lista todos os utilizadores numa tabela paginada
 * @param utilizadores Vetor de utilizadores do tipo t_utilizador
 * @param registos_utilizadores (Ponteiro) Quantidade de utilizadores registados
 * @param escolas Vetor de escolas do tipo t_escola. Utilizado para obter a abreviatura da escola do utilizador
 * @param registos_escolas (Ponteiro) Quantidade de escolas registadas
 */
void listar_utilizadores_paginado(t_utilizador utilizadores[], int *registos_utilizadores, t_escola escolas[], int *registos_escolas)
{
    char selecao = '0', erro; // v = voltar, a = anterior, p = proxima
    int posicao, offset = 0, pagina = 1, max_char_nome = 0, max_char_escola = 0, max_char_email = 0; // id, NIF e id da escola não precisam de ser calculados visto que têm caracteres fixos
    
    if(*registos_utilizadores > 0)
    {
        for(posicao = 0; posicao < *registos_utilizadores; posicao++)
        {
            max_char_nome = ((int)strlen(utilizadores[posicao].nome) > max_char_nome ? (int)strlen(utilizadores[posicao].nome) : max_char_nome);
            max_char_escola = ((int)strlen(escolas[(utilizadores[posicao].escola - 1)].abreviatura) > max_char_escola ? (int)strlen(escolas[(utilizadores[posicao].escola - 1)].abreviatura) : max_char_escola);
            max_char_email = ((int)strlen(utilizadores[posicao].email) > max_char_email ? (int)strlen(utilizadores[posicao].email) : max_char_email);
        }
        
        do{
            system("cls");
            printf("#\tTipo\t\t");
            aplicar_tabs("Escola", max_char_escola);
            aplicar_tabs("Nome", max_char_nome);
            aplicar_tabs("Email", max_char_email);
            printf("NIF\t\tSaldo");

            for(/* offset */; offset < (MAX_LINHAS_TABELA * pagina) && offset < *registos_utilizadores; offset++)
            {
                printf("\n%d\t%s \t", utilizadores[offset].identificador, utilizadores[offset].tipo);
                aplicar_tabs(escolas[(utilizadores[offset].escola-1)].abreviatura, max_char_escola);
                aplicar_tabs(utilizadores[offset].nome, max_char_nome);
                aplicar_tabs(utilizadores[offset].email, max_char_email);
                printf("%d\t%3.2f$", utilizadores[offset].NIF, utilizadores[offset].saldo);
            }
            printf("\nPagina %d de %.0f. Existem %d utilizadores registados.", pagina, ceil(((double)*registos_utilizadores + 1)/MAX_LINHAS_TABELA), *registos_utilizadores+1);
            
            selecao = selecionar_opcao("(v)oltar, (a)nterior, (p)roxima?", (char[3]){'v', 'a', 'p'}); // Vetor diretamente na chamada da função: https://stackoverflow.com/a/27281507/10935376
            calcular_navegacao_tabela(&selecao, &offset, registos_utilizadores, &pagina);
        }while(selecao != 'v');
    }
    else
    {
        printf("\n Sem registos a mostrar.\n");
    }
}

void consultar_transacoes(t_transacao transacoes[], int* registos)
{

}

/**
 * @brief Pergunta ao utilizador se tem a certeza que quer sair da aplicação
 * @return char 's' se confirma a saída, 'n' se não confirma a saída
 */
char confirmar_saida()
{
    char resposta;
    do
    {
        printf("\nTem a certeza que pretende sair (s/n)? ");
        scanf(" %c", &resposta);
        resposta = tolower(resposta);

        if(resposta != 's' && resposta != 'n')
            printf("\n\nValor Invalido. Tente Novamente!\n\n");

    }
    while(resposta != 's' && resposta != 'n');
    return resposta;
}

/**
 * @brief Carrega a informação do ficheiro de escolas para a memória
 * @param escolas 
 * @param registos_escolas 
 */
void carregar_escolas(t_escola escolas[], int *registos_escolas)
{
    char opcao = 'n';
    FILE *ficheiro;
    int verificacao_leitura;

    do{
        ficheiro = fopen("dados_escolas.dat", "rb");
        if(ficheiro == NULL)
        {
            printf("\nAVISO: Ficheiro de escolas nao encontrado."); // aparece na primeira utilização
        }
        else
        {
            verificacao_leitura = fread(escolas, sizeof(t_escola), MAX_ESCOLAS, ficheiro);
            if(verificacao_leitura == 0)
            {
                opcao = tratador_erros("Ocorreu um erro ao ler as escolas");
            }
            else
            {
                *registos_escolas = verificacao_leitura;
            }
            fclose(ficheiro);
        }
    }while(opcao != 'n');
}

/**
 * @brief Carrega a informação do ficheiro de utilizadores para a memória
 * @param utilizadores 
 * @param registos_utilizadores 
 */
void carregar_utilizadores(t_utilizador utilizadores[], int *registos_utilizadores)
{
    char opcao = 'n';
    FILE *ficheiro;
    int verificacao_leitura;
    
    do{
        ficheiro = fopen("dados_utilizadores.dat", "rb");
        if(ficheiro == NULL)
        {
            printf("\nAVISO: Ficheiro de utilizadores nao encontrado."); // aparece na primeira utilização
        }
        else
        {
            verificacao_leitura = fread(utilizadores, sizeof(t_utilizador), MAX_UTILIZADORES, ficheiro);
            if(verificacao_leitura == 0)
            {
                opcao = tratador_erros("Ocorreu um erro ao ler os utilizadores");
            }
            else
            {
                *registos_utilizadores = verificacao_leitura;
            }
            fclose(ficheiro);
        }
    }while(opcao != 'n');
}

/**
 * @brief Carrega a informação do ficheiro de transacoes para a memória
 * @param transacoes 
 * @param registos_transacoes 
 */
void carregar_transacoes(t_transacao transacoes[], int *registos_transacoes)
{
    char opcao = 'n';
    FILE *ficheiro;
    int verificacao_leitura;

    do{
        ficheiro = fopen("dados_transacoes.dat", "rb");
        if(ficheiro == NULL)
        {
            printf("\nAVISO: Ficheiro de transacoes nao encontrado."); // aparece na primeira utilização
        }
        else
        {
            verificacao_leitura = fread(transacoes, sizeof(t_transacao), MAX_TRANSACOES, ficheiro);
            if(verificacao_leitura == 0)
            {
                opcao = tratador_erros("Ocorreu um erro ao ler as transacoes");
            }
            else
            {
                *registos_transacoes = verificacao_leitura;
            }
            fclose(ficheiro);
        }
    }while(opcao != 'n');
}

/**
 * @brief Guarda no ficheiro de escolas a informação encontrada na memória
 * @param escolas 
 * @param registos_escolas 
 */
void guardar_escolas(t_escola escolas[], int *registos_escolas)
{
    char opcao = 'n';
    FILE *ficheiro;
    int verificacao_escrita;

    if(*registos_escolas > 0)
    {
        do{
            ficheiro = fopen("dados_escolas.dat", "wb");
            if(ficheiro == NULL)
            {
                opcao = tratador_erros("Ocorreu um erro ao tentar abrir o ficheiro de escolas para escrita");
            }
            else
            {
                verificacao_escrita = fwrite(escolas, sizeof(t_escola), *registos_escolas, ficheiro);
                if(verificacao_escrita != *registos_escolas)
                {
                    opcao = tratador_erros("Ocorreu um erro ao tentar guardar o ficheiro de escolas");
                }
                fclose(ficheiro);
            }
        }while(opcao != 'n');
    }
}

/**
 * @brief Guarda no ficheiro de utilizadores a informação encontrada na memória
 * @param utilizadores 
 * @param registos_utilizadores 
 */
void guardar_utilizadores(t_utilizador utilizadores[], int *registos_utilizadores)
{
    char opcao = 'n';
    FILE *ficheiro;
    int verificacao_escrita;

    if(*registos_utilizadores > 0)
    {
        do{
            ficheiro = fopen("dados_utilizadores.dat", "wb");
            if(ficheiro == NULL)
            {
                opcao = tratador_erros("Ocorreu um erro ao tentar abrir o ficheiro de utilizadores para escrita");
            }
            else
            {
                verificacao_escrita = fwrite(utilizadores, sizeof(t_utilizador), *registos_utilizadores, ficheiro);
                if(verificacao_escrita != *registos_utilizadores)
                {
                    opcao = tratador_erros("Ocorreu um erro ao tentar guardar o ficheiro de utilizadores");
                }
                fclose(ficheiro);
            }
        }while(opcao != 'n');
    }
}

/**
 * @brief Guarda no ficheiro de transacoes a informação encontrada na memória
 * @param transacoes 
 * @param registo_transacoes 
 */
void guardar_transacoes(t_transacao transacoes[], int *registo_transacoes)
{
    char opcao = 'n';
    FILE *ficheiro;
    int verificacao_escrita;

    if(*registo_transacoes > 0)
    {
        do{
            ficheiro = fopen("dados_transacoes.dat", "wb");
            if(ficheiro == NULL)
            {
                opcao = tratador_erros("Ocorreu um erro ao tentar abrir o ficheiro de transacoes para escrita");
            }
            else
            {
                verificacao_escrita = fwrite(transacoes, sizeof(t_transacao), *registo_transacoes, ficheiro);
                if(verificacao_escrita != *registo_transacoes)
                {
                    opcao = tratador_erros("Ocorreu um erro ao tentar guardar o ficheiro de transacoes");
                }
                fclose(ficheiro);
            }
        }while(opcao != 'n');
    }
}

/**
 * @brief Apresenta ao utilizador a escolha para tentar refazer a última operação
 * @param mensagem A mensagem de erro a apresentar (já incluí questão de tentar novamente)
 * @return char A escolha o utilizador (s/n)
 */
char tratador_erros(char mensagem[])
{
    char opcao = 'n';
    do{
        printf("\n%s. Tentar Novamente? (s/n) ", mensagem);
        scanf(" %c", &opcao);
        opcao = tolower(opcao);
        if(opcao != 'n' && opcao != 's')
            printf("\nOpcao Invalida. Tente Novamente.\n");
    }while(opcao != 'n' && opcao != 's');
    return opcao;
}