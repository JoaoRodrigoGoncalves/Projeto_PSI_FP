/**
 *  Aplicação que permite efetuar a gestão das transações
 *  do sistema SIGA nos bares e cantinas do Politécnico de Leiria
 *  Autor:  - João Gonçalves <2211870@my.ipleiria.pt>
 *          - Tiago Amaro <2211862@my.ipleiria.pt>
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#define MAX_ESCOLAS 5
#define MAX_UTILIZADORES 200
#define MAX_TRANSACOES 5000
#define MAX_LINHAS_TABELA 10

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
    char tipo[15]; // tipo de estudante (Estudante/Docente/Funcionário)
    char email[100];
    float saldo;
} t_utilizador;

typedef struct
{
    int identificador;
    int utilizador; //indentificador do utilizador
    char tipo[30];
    float valor;
    time_t tempo_registo;
} t_transacao;

// utils

int ler_inteiro(char [], int, int);
float ler_real(char [], float, float);
void ler_string(char [], char [], int, int);
void aplicar_tabs(char [], int);
char selecionar_opcao(char [], char []);
int opcao_na_array(char [], char *);
void calcular_navegacao_tabela(char *, int *, int *, int *, int *);
int processar_movimento(t_utilizador [], int *, int *, float *);
int validar_NIF(t_utilizador [], int *);
void validar_email(char []);
int validar_id_utilizador(t_utilizador [], int *, int *);
int validar_identificador_escola(t_escola[], int *);
time_t obter_data_timestamp(char[], int, int, int);

// menus

char menu(float [], int *, t_escola []);
char menu_escolas();
char menu_transacoes();
char menu_utilizadores();
char menu_estatistica();
char confirmar_saida();

// outras funções

void registar_escola(t_escola [], int *);
void registar_utilizadores(t_utilizador [],int *, t_escola [], int *);
void registar_transacao(t_transacao [], int *, t_utilizador [], int *, t_escola [], int *);
void consultar_escolas(t_escola [], int *);
void consultar_utilizadores(t_utilizador [], int *, t_escola []);
void consultar_transacoes(t_transacao [], int *, t_utilizador []);
void total_faturado_por_escola(t_transacao [], int *, int *, t_utilizador [], float []);
void percentagem_faturacao_por_escola(t_escola [], float [], int *, int *);
void pesquisa_horizonte_temporal(t_transacao [], int *, t_utilizador []);

// funções de dados

void carregar_escolas(t_escola [], int *);
void carregar_utilizadores(t_utilizador [], int *);
void carregar_transacoes(t_transacao [], int *);
void guardar_escolas(t_escola [], int *);
void guardar_utilizadores(t_utilizador [], int *);
void guardar_transacoes(t_transacao [], int *);

int main()
{
    t_escola escolas[MAX_ESCOLAS];
    t_utilizador utilizadores[MAX_UTILIZADORES];
    t_transacao transacoes[MAX_TRANSACOES];
    int escolas_registadas = 0, utilizadores_registados = 0, transacoes_registadas = 0;
    char selecao_saida = 'n', selecao_saida_escolas = 'n', selecao_saida_transacoes = 'n', selecao_saida_utilizadores = 'n', selecao_saida_estatistica = 'n';
    float faturado_escola[MAX_ESCOLAS];
    system("cls"); // limpamos aqui para poder mostrar possíveis erros com o carregamento de dados para a memória
    carregar_escolas(escolas, &escolas_registadas);
    carregar_utilizadores(utilizadores, &utilizadores_registados);
    carregar_transacoes(transacoes, &transacoes_registadas);

    do
    {
        total_faturado_por_escola(transacoes, &transacoes_registadas, &escolas_registadas, utilizadores, faturado_escola);
        switch(menu(faturado_escola, &escolas_registadas, escolas))
        {
            case 'e':
                system("cls");
                do
                {
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
                }
                while(selecao_saida_escolas != 's');
                break;

            case 'u':
                system("cls");
                do
                {
                    switch(menu_utilizadores())
                    {
                    case 'r':
                        registar_utilizadores(utilizadores, &utilizadores_registados, escolas, &escolas_registadas);
                        break;

                    case 'c':
                        consultar_utilizadores(utilizadores, &utilizadores_registados, escolas);
                        break;

                    case 'v':
                        system("cls");
                        selecao_saida_utilizadores = 's';
                        break;
                    }
                }
                while(selecao_saida_utilizadores != 's');
                break;

            case 't':
                system("cls");
                do
                {
                    switch(menu_transacoes())
                    {
                    case 'r':
                        registar_transacao(transacoes, &transacoes_registadas, utilizadores, &utilizadores_registados, escolas, &escolas_registadas);
                        break;

                    case 'c':
                        consultar_transacoes(transacoes, &transacoes_registadas, utilizadores);
                        break;

                    case 'v':
                        system("cls");
                        selecao_saida_transacoes = 's';
                        break;
                    }
                }
                while(selecao_saida_transacoes != 's');
                break;

            case 'a':
                system("cls");
                do{
                    switch(menu_estatistica())
                    {
                        case 'p':
                            percentagem_faturacao_por_escola(escolas, faturado_escola, &escolas_registadas, &transacoes_registadas);
                            break;

                        case 'd':
                            pesquisa_horizonte_temporal(transacoes, &transacoes_registadas, utilizadores);
                            break;

                        case 'v':
                            system("cls");
                            selecao_saida_estatistica = 's';
                            break;
                    }
                }while(selecao_saida_estatistica != 's');
                break;

            case 'g':
                guardar_escolas(escolas, &escolas_registadas);
                guardar_utilizadores(utilizadores, &utilizadores_registados);
                guardar_transacoes(transacoes, &transacoes_registadas);
                system("cls");
                printf("Operacao completa.");
                break;

            case 's':
                selecao_saida = confirmar_saida();
                break;
        }
    }
    while(selecao_saida != 's');

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

    do
    {
        printf("%s: ", mensagem);
        scanf("%d", &numero);

        if(numero < min || numero > max)
            printf("\n\nValor Invalido. Tente Novamente.\n\n");

    }
    while(numero < min || numero > max);
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
    do
    {
        printf("%s: ", mensagem);
        scanf("%f", &numero);

        if(numero < min || numero > max)
            printf("\n\nValor Invalido. Tente Novamente.\n\n");

    }
    while(numero < min || numero > max);
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
        printf("%s: ", texto_questao);
        scanf(" %99[^\n]", temp_string); // limitamos a entrada a 99 caracteres para permitir o '\0' e para impedir buffer overflow
        fflush(stdin);
        tamanho = strlen(temp_string);

        if(tamanho < min)
        {
            printf("\n\nO valor indicado e demasiado curto. Tente novamente!\n\n");
        }
        else if (tamanho > max)
        {
            printf("\n\nO valor indicado e demasiado comprido. Tente novamente!\n\n");
        }

    }
    while (tamanho < min || tamanho > max);
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

    do
    {
        printf("\n%s ", mensagem);
        scanf(" %c", &selecao);
        fflush(stdin);
        selecao = tolower(selecao);

        if(!opcao_na_array(opcoes, &selecao))
            printf("\nOpcao Invalida. Tente Novamente.");
    }
    while(!opcao_na_array(opcoes, &selecao));

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
    int posicao, resultado = 0;

    for(posicao = 0; posicao < (int)strlen(opcoes); posicao++) // usamos strlen para obter o mesmo objetivo que sizeof visto que este ia devolver o tamanho do ponteiro, não o tamanho do vetor.
    {
        if(opcoes[posicao] == *selecao)
        {
            resultado = 1;
        }
    }
    return resultado;
}

/**
 * @brief Calcula e atribui os devidos valores ao offset e pagina a serem aplicados à tabela
 * @param selecao A opção escolhida pelo utilizador (a - anterior, p - próxima)
 * @param offset Variável utilizada pelo for loop para ler pelo vetor
 * @param num_registo Quantidade de registos no vetor
 * @param pagina Pagina atual da tabela
 * @param linhas_mostradas Quantidade de linhas mostradas na página atual
 */
void calcular_navegacao_tabela(char *selecao, int *offset, int *num_registo, int *pagina, int *linhas_mostradas)
{
    *offset = *offset - 1; // reduzir 1 por causa da secção de incrementação do for loop no final
    switch(*selecao)
    {
    case 'p':
        // não se usa <= porque offset é baseado em 0 e queremos o
        // imediatamente anterior
        if((*offset + MAX_LINHAS_TABELA) < *num_registo)
        {
            *offset = *offset + 1; // restaurar offset para continuar na condição falhada
            *pagina = *pagina + 1; // aumentar a pagina para a condição ser válida
        }
        else
        {
            // não é suficiente para encher uma página só por si mesmo
            // é necessário calcular se sequer existem mais dados para
            // serem mostrados

            if(*num_registo - (*offset + 1) > 0)
            {
                // adicionamos 1 ao offset na conta para que a condição
                // fosse avaliada por números "verdadeiros" e não contagens
                // baseadas em 0.

                // Aqui podemos confirmar que ainda existem linhas de informação
                // a serem mostradas, por isso vamos dar indicação de que se pode
                // incrementar a página e restaurar o offset para continuar a
                // iterar pelo vetor.

                *offset = *offset + 1;
                *pagina = *pagina + 1;
            }
            else
            {
                // verifica-se que não exite mais informação a ser apresentada
                // por isso volta-se a apresentar a página atual.
                // para voltar a apresentar a página atual é necessário
                // retirar o número de linhas apresentadas na página atual

                // linhas_mostradas vs. MAX_LINHAS_TABELA
                // linhas_mostradas vai apresentar a quantidade de linhas
                // que foram escritas, enquanto que MAX_LINHAS_TABELA vai
                // apenas devolver o valor máximo de linhas que uma página
                // pode ter. Isto iria causar problemas caso a página a ser
                // mostrada no momento tivesse menos de 10 linhas.

                *offset = *offset - (*linhas_mostradas - 1);

                // Visto que linhas_mostradas é incrementado na secção
                // pós execução do for loop, é necessário subtrair 1 a este
                // valor de forma a que este fique correto.

            }
        }
        break;

    case 'a':

        if(*offset - *linhas_mostradas - MAX_LINHAS_TABELA > 0)
        {
            // se é possível mover o offset para o inicio da pagina atual
            // e depois move-lo para o inicio de uma suposta pagina anterior
            // então é isso que vamos fazer

            *offset = *offset - *linhas_mostradas - MAX_LINHAS_TABELA;
            *pagina = *pagina - 1;

        }
        else
        {
            // não é possível andar assim tanto para trás o que significa
            // que ou estamos na primeira ou segunda página

            *offset = 0; // dar reset ao offset para mostrar a primeira página

            if(*pagina == 2)
            {
                // se a página é a 2, então trocamos para a 1
                // se já estamos na 1, não é preciso mudar nada
                *pagina = 1;
            }
        }
        break;
    }
}

/**
 * @brief Processa as transações na estrutura t_utilizador para o utilizador indicado.
 * @param utilizadores Vetor de estruturas do tipo t_utilizador.
 * @param identificador_utilizador Indentificador do utilizador associado à transação
 * @param tipo_transacao O tipo de transação que vai ser executado: 1 - Pagamento, 2 - Carregamento
 * @return int 1 - sucesso, 0 - sem sucesso
 */
int processar_movimento(t_utilizador utilizadores[], int *identificador_utilizador, int *tipo_transacao, float *valor_movimento)
{
    int resultado = 0;
    if(*tipo_transacao == 1)
    {
        // Pagamento
        if(utilizadores[*identificador_utilizador - 1].saldo >= *valor_movimento)
        {
            utilizadores[*identificador_utilizador - 1].saldo -= *valor_movimento;
            resultado = 1;
        }
        else
        {
            printf("Erro: O utilizador nao tem saldo suficiente para concluir o pagamento. Saldo atual: %.2f EUR.\n", utilizadores[*identificador_utilizador - 1].saldo);
            resultado =  0;
        }
    }
    else
    {
        // Carregamento
        utilizadores[*identificador_utilizador - 1].saldo += *valor_movimento;
        resultado = 1;
    }
    return resultado;
}

/**
 * @brief Pede um NIF ao utilizador e confirma que não está registado noutro utilizador.
 * @param utilizadores Vetor do tipo t_utilizador com os utilizadores.
 * @param num_registos (Ponteiro) Total de utilizadores já registados.
 * @return int NIF
 */
int validar_NIF(t_utilizador utilizadores[], int *num_registos)
{
    int nif, posicao, nif_posicao = -1;

    do
    {
        if(nif_posicao != -1)
        {
            printf("\nO NIF indicado ja se encontra registado no utilizador \"%s\". Tente Novamente.\n", utilizadores[nif_posicao].nome);
            nif_posicao = -1; // reiniciar a posição do NIF
        }

        nif = ler_inteiro("Indique o numero de identificacao fiscal", 100000000, 299999999);

        for(posicao = 0; posicao < *num_registos; posicao++)
        {
            if(utilizadores[posicao].NIF == nif)
            {
                nif_posicao = posicao;
            }
        }

    }
    while(nif_posicao != -1);
    return nif;
}

/**
 * @brief Pede um email e confirma que é um válido caso contenha o caracter '@'
 * @param email Variável onde o email será guardado
 */
void validar_email(char email[])
{
    int posicao, resultado = 0;
    char email_temp[100];

    do
    {
        ler_string(email_temp, "Indique o email", 6, 100);

        for(posicao = 0; posicao < (int)strlen(email_temp); posicao++)
        {
            if(email_temp[posicao] == '@')
            {
                resultado = 1;
            }
        }

        if(resultado == 0)
            printf("Email invalido. Tente Novamente.\n");

    }
    while(resultado == 0);

    strcpy(email, email_temp);
}

/**
 * @brief Verifica que um id de utilizador é válido
 * @param utilizadores Vetor do tipo t_utilizador com os utilizadores registados.
 * @param quantidade_registos (Ponteiro) Quantidade de utilizadores registados.
 * @param id_a_testar (Ponteiro) ID a verificar.
 * @return int 1 - id válido, 0 - id inválido
 */
int validar_id_utilizador(t_utilizador utilizadores[], int *quantidade_registos, int *id_a_testar)
{
    int posicao, resultado = 0;

    for(posicao = 0; posicao < *quantidade_registos; posicao++)
    {
        if(utilizadores[posicao].identificador == *id_a_testar)
        {
            resultado = 1;
        }
    }
    return resultado;
}

/**
 * @brief Pede um identificador único de uma escola ao utilizador e devolve-o caso a escola exista
 * @param escolas Vetor do tipo t_escola com as escolas registadas
 * @param quantidade_escolas (Ponteiro) Quantidade de escolas registadas.
 * @return int o ID único válido indicado pelo utilizador
 */
int validar_identificador_escola(t_escola escolas[], int *quantidade_escolas)
{
    int id_escola, posicao, posicao_escola = -1;

    do
    {
        id_escola = ler_inteiro("\nIndique o identificador da escola", 1, *quantidade_escolas);
        for(posicao = 0; posicao < *quantidade_escolas; posicao++)
        {
            if(escolas[posicao].identificador == id_escola)
            {
                posicao_escola = posicao; // alterar o valor
                posicao = *quantidade_escolas; // forçar saída do loop
            }
        }

        if(posicao_escola == -1)
        {
            printf("Identificador invalido. Tente Novamente.");
        }
    }
    while(posicao_escola == -1);
    return id_escola;
}

/**
 * @brief Lê, valida e converte uma data indicada pelo utilizador
 * @param mensagem A mensagem a mostrar ao utilizador
 * @param valor_padrao_hora O valor que o campo da hora deverá ter
 * @param valor_padrao_minutos O valor que o campo dos minutos deverá ter
 * @param valor_padrao_segundos O valor que o campo dos minutos deverá ter
 */
time_t obter_data_timestamp(char mensagem[], int valor_padrao_hora, int valor_padrao_minutos, int valor_padrao_segundos)
{
    int entradas = 0, dia = 0, mes = 0, ano = 0;
    struct tm temp_time = {0};

    do{
        printf("\n%s: ", mensagem);
        // https://stackoverflow.com/a/34422936/10935376
        entradas = scanf("%d/%d/%d", &dia, &mes, &ano);
        if(entradas != 3 || dia > 31 || dia < 1 || mes > 12 || mes < 1 || ano < 1900)
        {
            printf("\nErro: Data/hora indicada invalida. Tente novamente.\n");
        }
        fflush(stdin); // ter a certeza que não fica nada do buffer depois desta operação
    }while(entradas != 3 || dia > 31 || dia < 1 || mes > 12 || mes < 1 || ano < 1900);

    temp_time.tm_hour = valor_padrao_hora;
    temp_time.tm_min = valor_padrao_minutos;
    temp_time.tm_sec = valor_padrao_segundos;
    temp_time.tm_mday = dia;
    temp_time.tm_mon = mes - 1; // meses começam em 0
    temp_time.tm_year = ano - 1900; // é necessário remover 1900 por causa do epoch do NTP

    return mktime(&temp_time);
}

/**
 * @brief Desenha o menu e pede uma opção ao utilizador
 * @return char A escolha do utilizador (minuscula)
 */
char menu(float faturado_escola[], int *quantidade_escolas, t_escola escolas[])
{
    int indice;
    float total = 0;
    char escolha;
    if(*quantidade_escolas > 0)
    {
        printf("\n========= Estatisticas =========");
        for(indice = 0; indice < *quantidade_escolas; indice++)
        {
            printf("\n%s: %.2f Euros", escolas[indice].abreviatura, faturado_escola[indice]);
            total += faturado_escola[indice];
        }
        printf("\nTotal Faturado em todos os campi: %.2f Euros", total);
    }
    printf("\n============ Menus =============");
    printf("\n[E] Menu Escolas");
    printf("\n[U] Menu Utilizadores");
    printf("\n[T] Menu Transacoes");
    printf("\n[A] Menu de Estatisticas");
    printf("\n[G] Guardar Dados");
    printf("\n[S] Sair");
    printf("\n================================");
    fflush(stdin);
    escolha = selecionar_opcao("Opcao:", (char[6]){'e', 'u', 't', 'a', 'g', 's'});
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
    fflush(stdin);
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
    fflush(stdin);
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
    fflush(stdin);
    escolha = selecionar_opcao("Opcao:", (char[3]){'r', 'c', 'v',});
    return escolha;
}

/**
 * @brief Desenha o menu de opções relacionadas com estatistica
 * @return char A escolha do utilizador (minuscula)
 */
char menu_estatistica()
{
    char escolha;
    printf("\n============ Estatistica =============");
    printf("\n[P] Percentagem transacoes por escola");
    printf("\n[D] Estatiscas entre duas datas");
    printf("\n[V] Voltar ao Menu Anterior");
    printf("\n======================================");
    fflush(stdin);
    escolha = selecionar_opcao("Opcao:", (char[3]){'p', 'd', 'v',});
    return escolha;
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
 * @brief Pede as informações necessárias e regista uma escola no vetor de estruturas t_escola
 * @param escolas Vetor de estruturas do tipo t_escola onde a informação será guardada.
 * @param num_registos (Ponteiro) Quantidade de registos já realizados. Utilizado para determinar o índice do vetor onde a informação será guardada.
 */
void registar_escola(t_escola escolas[], int *num_registos)
{
    system("cls");
    if(*num_registos < MAX_ESCOLAS)
    {
        printf("\n=== Registar Escola ===\n");
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
    system("cls");
    if(*num_registos < MAX_UTILIZADORES && *registos_escolas > 0)
    {
        utilizadores[*num_registos].identificador = *num_registos + 1;
        consultar_escolas(lista_escolas, registos_escolas); // mostrar lista de escolas antes de pedir o ID da escola
        printf("\n=== Registar Utilizador ==="); // mostramos isto aqui visto que a função consultar_escolas() limpa o ecrã
        utilizadores[*num_registos].escola = validar_identificador_escola(lista_escolas, registos_escolas);
        ler_string(utilizadores[*num_registos].nome, "Indique o nome", 3, 100);
        utilizadores[*num_registos].NIF  = validar_NIF(utilizadores, num_registos);
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

        validar_email(utilizadores[*num_registos].email);
        utilizadores[*num_registos].saldo = 0;
        *num_registos = *num_registos + 1;
        system("cls");
    }
    else
    {
        printf("\nO limite de %d utilizadores foi excedido ou nao existem escolas registadas. Existem %d utilizadores registados.\nNao e possivel registar utilizador.", MAX_UTILIZADORES, *num_registos);
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
    time_t data_hora_atuais;
    int selecao_tipo;
    system("cls");
    if(*quantidade_registos_transacoes < MAX_TRANSACOES && (*quantidade_registos_escolas && *quantidade_registos_utilizadores)) // se uma das variáveis finais for 0, a condição falha
    {
        printf("\n=== Registar Transacao ===\n");
        temp.identificador = *quantidade_registos_transacoes + 1;
        do
        {
            temp.utilizador = ler_inteiro("Indique o identificador unico do utilizador (0 para lista)", 0, MAX_UTILIZADORES);
            if(temp.utilizador == 0)
                consultar_utilizadores(utilizadores, quantidade_registos_utilizadores, escolas); // não é necessário utilizar * na varável pois já é o endereço do ponteiro

            if(temp.utilizador != 0 && !validar_id_utilizador(utilizadores, quantidade_registos_utilizadores, &temp.utilizador))
                printf("Identificador invalido. Tente Novamente.\n");
        }
        while(temp.utilizador == 0 || !validar_id_utilizador(utilizadores, quantidade_registos_utilizadores, &temp.utilizador));
        selecao_tipo = ler_inteiro("Indique o tipo de transacao (1 - Pagameto, 2 - Carregamento)", 1, 2);

        selecao_tipo == 1 ? strcpy(temp.tipo, "Pagamento") : strcpy(temp.tipo, "Carregamento");

        temp.valor = ler_real("Indique o valor da transacao (Formato [0.00])", 0.01, 100.00);

        /**
         * Recursos utilizados para trabalhar com a biblioteca <time.h>
         * https://pubs.opengroup.org/onlinepubs/7908799/xsh/localtime.html
         * https://www.youtube.com/watch?v=4j0pRe8rxhs
         */

        time(&data_hora_atuais);
        temp.tempo_registo = data_hora_atuais; // passa a estrutura de tempo para a estrutura dos registos

        if(processar_movimento(utilizadores, &temp.utilizador, &selecao_tipo, &temp.valor))
        {
            transacoes[*quantidade_registos_transacoes] = temp;
            *quantidade_registos_transacoes = *quantidade_registos_transacoes + 1;
            system("cls");
        }
    }
    else
    {
        printf("\nO limite de %d transacoes foi excedido ou nao existem escolas/utilizadores registados. Existem %d transacoes registadas.\nNao e possivel registar transacao.", MAX_TRANSACOES, *quantidade_registos_transacoes);
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

        printf("ID\t");
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
void consultar_utilizadores(t_utilizador utilizadores[], int *registos_utilizadores, t_escola escolas[])
{
    char selecao = '0';
    int posicao, offset = 0, linhas_na_pagina, pagina = 1, max_char_nome = 0, max_char_escola = 0, max_char_email = 0;

    /**
     * O ponteiro registos utilizadores é utilizado no resto do programa como forma de indicar o índice onde o
     * próximo registo deve ser indicado. Visto isto, e apesar de os vetores começarem em 0, esta variável
     * continua a funcionar como forma de ilustrar a quantidade de, neste caso, utilizadores registados.
     */

    if(*registos_utilizadores > 0)
    {
        for(posicao = 0; posicao < *registos_utilizadores; posicao++) // aqui a posicao começa em 0, sendo assim, a condição verifica ATÉ ao número imediatamente anterior porque 0 também conta
        {
            max_char_nome = ((int)strlen(utilizadores[posicao].nome) > max_char_nome ? (int)strlen(utilizadores[posicao].nome) : max_char_nome);
            max_char_escola = ((int)strlen(escolas[(utilizadores[posicao].escola - 1)].abreviatura) > max_char_escola ? (int)strlen(escolas[(utilizadores[posicao].escola - 1)].abreviatura) : max_char_escola);
            max_char_email = ((int)strlen(utilizadores[posicao].email) > max_char_email ? (int)strlen(utilizadores[posicao].email) : max_char_email);
        }

        do
        {
            system("cls");
            printf("#\tTipo\t\t");
            aplicar_tabs("Escola", max_char_escola);
            aplicar_tabs("Nome", max_char_nome);
            aplicar_tabs("Email", max_char_email);
            printf("NIF\t\tSaldo");

            for(linhas_na_pagina = 0 /*, offset */; offset < (MAX_LINHAS_TABELA * pagina) && offset < *registos_utilizadores; offset++, linhas_na_pagina++)
            {
                printf("\n%d\t%s \t", utilizadores[offset].identificador, utilizadores[offset].tipo);
                aplicar_tabs(escolas[(utilizadores[offset].escola-1)].abreviatura, max_char_escola);
                aplicar_tabs(utilizadores[offset].nome, max_char_nome);
                aplicar_tabs(utilizadores[offset].email, max_char_email);
                printf("%d\t%6.2f EUR", utilizadores[offset].NIF, utilizadores[offset].saldo);
            }
            printf("\nPagina %d de %.0f. Existem %d utilizadores registados.", pagina, ceil(((double)*registos_utilizadores)/MAX_LINHAS_TABELA), *registos_utilizadores);

            selecao = selecionar_opcao("(V)oltar, (A)nterior, (P)roxima?", (char[3]){'v', 'a', 'p'}); // Vetor diretamente na chamada da função: https://stackoverflow.com/a/27281507/10935376
            calcular_navegacao_tabela(&selecao, &offset, registos_utilizadores, &pagina, &linhas_na_pagina);
        }
        while(selecao != 'v');
        system("cls");
    }
    else
    {
        system("cls");
        printf("\n Sem registos a mostrar.\n");
    }
}

/**
 * @brief Lista de todas as transacoes numa tabela paginada
 * @param transacoes Vetor de estruturas do tipo t_transaco
 * @param registos_transacoes (Ponteiro) Quantidade de transacoes registadas
 * @param utilizadores Vetor de estruturas do tipo t_utilizador
 */
void consultar_transacoes(t_transacao transacoes[], int* registos_transacoes, t_utilizador utilizadores[])
{
    char selecao = '0'; // v = voltar, a = anterior, p = proxima
    int posicao, offset = 0, linhas_na_pagina, pagina = 1, max_char_nome = 0;

    if(*registos_transacoes > 0)
    {
        for(posicao = 0; posicao < *registos_transacoes; posicao++)
        {
            max_char_nome = ((int)strlen(utilizadores[transacoes[posicao].utilizador - 1].nome) > max_char_nome ? (int)strlen(utilizadores[transacoes[posicao].utilizador - 1].nome) : max_char_nome);
        }

        do
        {
            system("cls");
            printf("#\tData/Hora\t\t");
            aplicar_tabs("Nome", max_char_nome);
            printf("Tipo\t\tValor");

            for(linhas_na_pagina = 0 /*, offset */; offset < (MAX_LINHAS_TABELA * pagina) && offset < *registos_transacoes; offset++, linhas_na_pagina++)
            {
                struct tm *tempo = localtime(&transacoes[offset].tempo_registo);
                printf("\n%d\t%02d/%02d/%d %02d:%02d:%02d\t", transacoes[offset].identificador, tempo->tm_mday, tempo->tm_mon + 1, tempo->tm_year + 1900, tempo->tm_hour, tempo->tm_min, tempo->tm_sec);
                aplicar_tabs(utilizadores[transacoes[offset].utilizador - 1].nome, max_char_nome);
                aplicar_tabs(transacoes[offset].tipo, 12); // 12 é o máximo de caracteres
                printf("%6.2f EUR", transacoes[offset].valor);
            }
            printf("\nPagina %d de %.0f. Existem %d transacoes registadas.", pagina, ceil(((double)*registos_transacoes)/MAX_LINHAS_TABELA), *registos_transacoes);

            selecao = selecionar_opcao("(V)oltar, (A)nterior, (P)roxima?", (char[3]){'v', 'a', 'p'}); // Vetor diretamente na chamada da função: https://stackoverflow.com/a/27281507/10935376
            calcular_navegacao_tabela(&selecao, &offset, registos_transacoes, &pagina, &linhas_na_pagina);
        }
        while(selecao != 'v');
    }
    else
    {
        system("cls");
        printf("\n Sem registos a mostrar.\n");
    }
}

/**
 * @brief Calcula o total faturado por cada escola registada.
 * @param transacoes Vetor do tipo t_transacao.
 * @param registos_transacoes (Ponteiro) Quantidade de transacoes registadas.
 * @param registos_escolas (Ponteiro) Quantidade de escolas registadas.
 * @param utilizadores Vetor do tipo t_utilizador.
 * @param total_transacoes Vetor do tipo float onde são guardados os totais faturados por cada escola.
 */
void total_faturado_por_escola(t_transacao transacoes[], int *registos_transacoes, int *registos_escolas, t_utilizador utilizadores[], float total_transacoes[])
{
    int indice_transacoes, indice_escolas, user_transacao, escola_user;

    if (*registos_transacoes > 0) // transacoes persupõe que já existem escolas
    {
        for (indice_escolas = 0; indice_escolas < *registos_escolas; indice_escolas++)
        {
            total_transacoes[indice_escolas] = 0; // colocar os valores das escolas a 0 porque podem já estar preenchidos com um valor anterior
        }

        for (indice_transacoes = 0; indice_transacoes < *registos_transacoes; indice_transacoes++)
        {
            if (strcmp(transacoes[indice_transacoes].tipo, "Pagamento") == 0)
            {
                user_transacao = transacoes[indice_transacoes].utilizador - 1; // índice do utilizador no vetor de utilizadores
                escola_user = utilizadores[user_transacao].escola - 1; // índice da escola no vetor de escolas
                total_transacoes[escola_user] += transacoes[indice_transacoes].valor;
            }
        }
    }
}

/**
 * @brief Apresenta a precentagem e valor faturado individualmente por cada escola
 * @param escolas Vetor do tipo t_escola com as escolas registadas.
 * @param transacoes_escolas Vetor do tipo float com as transações das escolas em que cada indice corresponde a uma escola.
 * @param registo_escolas (Ponteiro) Quantidade de escolas registadas.
 */
void percentagem_faturacao_por_escola(t_escola escolas[], float transacoes_escolas[], int *registo_escolas, int *registos_transacoes)
{
    int indice;
    float total = 0;

    system("cls");
    if(*registos_transacoes > 0){
        for (indice = 0; indice < *registo_escolas; indice++)
        {
            total += transacoes_escolas[indice];
        }

        printf("\n============ Prcentagem por Escola ============");
        for (indice = 0; indice < *registo_escolas; indice++)
        {
            printf("\n%s:\t %.2f%% (%.2f EUR)", escolas[indice].abreviatura, (transacoes_escolas[indice] / total) * 100, transacoes_escolas[indice]);
        }
        printf("\n===============================================");
    }
    else
    {
        printf("\nNao e possivel mostrar as percentagens pois nao existem transacoes registadas.");
    }

}

/**
 * @brief Pesquisa e apresenta o total de pagamentos efetuados por cada tipo de utilizador no periodo indicado pelo mesmo
 * @param transacoes Vetor do tipo t_transacao com.
 * @param registos_transacoes (Ponteiro) Quantidade de transacoes registadas
 * @param utilizadores Vetor do tipo t_utilizadores.
 */
void pesquisa_horizonte_temporal(t_transacao transacoes[], int *registos_transacoes, t_utilizador utilizadores[])
{
    int posicao, total_transacoes_a_mostrar = 0, contador_transacoes[3] = {0};
    float total_tipo[3] = {0};
    time_t timestamp_inicio, timestamp_fim;

    system("cls");
    timestamp_inicio = obter_data_timestamp("Indique a data de inicio de pesquisa (dia/mes/ano)", 0, 0, 0);
    timestamp_fim = obter_data_timestamp("Indique a data de fim de pesquisa (dia/mes/ano)", 23, 59, 59);

    for(posicao = 0; posicao < *registos_transacoes; posicao++)
    {
        if(strcmp(transacoes[posicao].tipo, "Pagamento") == 0 && transacoes[posicao].tempo_registo >= timestamp_inicio && transacoes[posicao].tempo_registo <= timestamp_fim)
        {
            // Estudante 0 / Docente 1 / Funcionario 2

            if(strcmp(utilizadores[transacoes[posicao].utilizador - 1].tipo, "Estudante") == 0)
            {
                total_tipo[0] += transacoes[posicao].valor;
                contador_transacoes[0] += 1;
            }

            if(strcmp(utilizadores[transacoes[posicao].utilizador - 1].tipo, "Docente") == 0)
            {
                total_tipo[1] += transacoes[posicao].valor;
                contador_transacoes[1] += 1;
            }

            if(strcmp(utilizadores[transacoes[posicao].utilizador - 1].tipo, "Funcionario") == 0)
            {
                total_tipo[2] += transacoes[posicao].valor;
                contador_transacoes[2] += 1;
            }
            total_transacoes_a_mostrar++;
        }
    }

    if(total_transacoes_a_mostrar > 0)
    {
        printf("\n=== Resultados Da Pesquisa Horizontal ===");
        printf("\nEstudantes: \t%.2fEUR (%d transacoes)", total_tipo[0], contador_transacoes[0]);
        printf("\nDocente: \t%.2fEUR (%d transacoes)", total_tipo[1], contador_transacoes[1]);
        printf("\nFuncionarios: \t%.2fEUR (%d transacoes)", total_tipo[2], contador_transacoes[2]);
        printf("\n=========================================\n");
    }
    else
    {
        printf("\nNao existem transacoes de pagamentos no periodo indicado.\n\n");
    }
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
    int verificacao_leitura, bytes, elementos;

    do
    {
        ficheiro = fopen("dados_escolas.dat", "rb");
        if(ficheiro == NULL)
        {
            printf("\nAVISO: Ficheiro de escolas nao encontrado."); // aparece na primeira utilização
        }
        else
        {
            fseek(ficheiro, 0L, SEEK_END);
            bytes = ftell(ficheiro);
            elementos = bytes/sizeof(t_escola);
            fseek(ficheiro, 0L, SEEK_SET);

            verificacao_leitura = fread(escolas, sizeof(t_escola), elementos, ficheiro);
            if(verificacao_leitura == 0)
            {
                opcao = selecionar_opcao("Ocorreu um erro ao ler as escolas. Tentar novamente? (s/n)", (char[2]){'s', 'n'});
            }
            else
            {
                *registos_escolas = elementos;
            }
            fclose(ficheiro);
        }
    }
    while(opcao != 'n');
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
    int verificacao_leitura, bytes, elementos;

    do
    {
        ficheiro = fopen("dados_utilizadores.dat", "rb");
        if(ficheiro == NULL)
        {
            printf("\nAVISO: Ficheiro de utilizadores nao encontrado."); // aparece na primeira utilização
        }
        else
        {
            fseek(ficheiro, 0L, SEEK_END);
            bytes = ftell(ficheiro);
            elementos = bytes/sizeof(t_escola);
            fseek(ficheiro, 0L, SEEK_SET);
            verificacao_leitura = fread(utilizadores, sizeof(t_utilizador), elementos, ficheiro);
            if(verificacao_leitura == 0)
            {
                opcao = selecionar_opcao("Ocorreu um erro ao ler os utilizadores. Tentar novamente? (s/n)", (char[2]){'s', 'n'});
            }
            else
            {
                *registos_utilizadores = verificacao_leitura;
            }
            fclose(ficheiro);
        }
    }
    while(opcao != 'n');
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
    int verificacao_leitura, bytes, elementos;

    do
    {
        ficheiro = fopen("dados_transacoes.dat", "rb");
        if(ficheiro == NULL)
        {
            printf("\nAVISO: Ficheiro de transacoes nao encontrado."); // aparece na primeira utilização
        }
        else
        {
            fseek(ficheiro, 0L, SEEK_END);
            bytes = ftell(ficheiro);
            elementos = bytes/sizeof(t_transacao);
            fseek(ficheiro, 0L, SEEK_SET);
            verificacao_leitura = fread(transacoes, sizeof(t_transacao), elementos, ficheiro);
            if(verificacao_leitura == 0)
            {
                opcao = selecionar_opcao("Ocorreu um erro ao ler as transacoes. Tentar novamente? (s/n)", (char[2]){'s', 'n'});
            }
            else
            {
                *registos_transacoes = verificacao_leitura;
            }
            fclose(ficheiro);
        }
    }
    while(opcao != 'n');
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
        do
        {
            ficheiro = fopen("dados_escolas.dat", "wb");
            if(ficheiro == NULL)
            {
                opcao = selecionar_opcao("Ocorreu um erro ao tentar abrir o ficheiro de escolas para escrita. Tentar novamente? (s/n)", (char[2]){'s', 'n'});
            }
            else
            {
                verificacao_escrita = fwrite(escolas, sizeof(t_escola), *registos_escolas, ficheiro);
                if(verificacao_escrita != *registos_escolas)
                {
                    opcao = selecionar_opcao("Ocorreu um erro ao tentar guardar o ficheiro de escolas. Tentar novamente? (s/n)", (char[2]){'s', 'n'});
                }
                fclose(ficheiro);
            }
        }
        while(opcao != 'n');
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
        do
        {
            ficheiro = fopen("dados_utilizadores.dat", "wb");
            if(ficheiro == NULL)
            {
                opcao = selecionar_opcao("Ocorreu um erro ao tentar abrir o ficheiro de utilizadores para escrita. Tentar novamente? (s/n)", (char[2]){'s', 'n'});
            }
            else
            {
                verificacao_escrita = fwrite(utilizadores, sizeof(t_utilizador), *registos_utilizadores, ficheiro);
                if(verificacao_escrita != *registos_utilizadores)
                {
                    opcao = selecionar_opcao("Ocorreu um erro ao tentar guardar o ficheiro de utilizadores. Tentar novamente? (s/n)", (char[2]){'s', 'n'});
                }
                fclose(ficheiro);
            }
        }
        while(opcao != 'n');
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
        do
        {
            ficheiro = fopen("dados_transacoes.dat", "wb");
            if(ficheiro == NULL)
            {
                opcao = selecionar_opcao("Ocorreu um erro ao tentar abrir o ficheiro de transacoes para escrita. Tentar novamente? (s/n)", (char[2]){'s', 'n'});
            }
            else
            {
                verificacao_escrita = fwrite(transacoes, sizeof(t_transacao), *registo_transacoes, ficheiro);
                if(verificacao_escrita != *registo_transacoes)
                {
                    opcao = selecionar_opcao("Ocorreu um erro ao tentar guardar o ficheiro de transacoes. Tentar novamente? (s/n)", (char[2]){'s', 'n'});
                }
                fclose(ficheiro);
            }
        }
        while(opcao != 'n');
    }
}
