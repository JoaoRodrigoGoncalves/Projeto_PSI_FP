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
    int tipo; // tipo de estudante (Estudante-0/Docente-1/Funcionário-2) uma estrutura?
    char email[100];
    float saldo; // isto ainda me vai dar problemas. eu metia um inteiro e fazer uma divisão por 100 para encontrar as casas decimais. só não faço já porque tenho medo que o stor me dê porrada
} t_utilizador;

typedef struct
{
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
void listar_utilizadores_paginado(t_utilizador [], int *);
char tratador_erros(char []);

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
void registar_transacao(t_transacao [], int *, int *, t_utilizador [], int *);
void registar_utilizadores(t_utilizador [],int *);
void consultar_utilizadores(t_utilizador [], int *);

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

    do
    {
        switch(menu())
        {
            case 'e':
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
                            selecao_saida_escolas = 's';
                            break;
                    }
                }
                while(selecao_saida_escolas != 's');
                break;

            case 'u':
                do
                {
                    switch(menu_utilizadores())
                    {
                        case 'r':
                            registar_utilizadores(utilizadores, &utilizadores_registados);
                            break;

                        case 'c':
                            consultar_utilizadores(utilizadores, &utilizadores_registados);
                            break;

                        case 'v':
                            selecao_saida_utilizadores = 's';
                            break;
                    }
                }
                while(selecao_saida_utilizadores != 's');
                break;

            case 't':
                do{
                    switch(menu_transacoes())
                    {
                        case 'r':
                            registar_transacao(transacoes, &transacoes_registadas, &escolas_registadas, utilizadores, &utilizadores_registados);
                            break;
                        
                        case 'c':
                            break;

                        case 'v':
                            selecao_saida_transacoes = 's';
                            break;
                    }
                }while(selecao_saida_transacoes != 's');
                break;

            case 's':
                selecao_saida = confirmar_saida();
                break;
        }
    }
    while(selecao_saida != 's');

    printf("\n A guardar informacao...");
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
 * @brief Desenha o menu e pede uma opção ao utilizador
 * @return char A escolha do utilizador (minuscula)
 */
char menu()
{
    char escolha;
    do
    {
        printf("\n\n========= Estatisticas =========");
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
    }
    while(escolha != 's' && escolha != 'e' && escolha != 'u' && escolha != 't');
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
    do
    {
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
    }
    while(escolha != 'r' && escolha != 'c' && escolha != 'v');
    return escolha;
}

/**
 * @brief Desenha o menu de opções relacionadas aos utilizadores
 * @return char A escolha do utilizador converte a letra para minuscula
 */
char menu_utilizadores()
{
    char escolha;
    do
    {
        printf("\n======== Utilizadores =========");
        printf("\n[R] Registar Utilizador");
        printf("\n[C] Consultar Utilizadores");
        printf("\n[V] Voltar ao Menu Anterior");
        printf("\n==========================");

        printf("\nOpcao: ");
        scanf(" %c", &escolha);
        escolha = tolower(escolha);
        if(escolha != 'r' && escolha != 'c' && escolha != 'v')
            printf("\n\nOpcao Desconhecida. Tente Novamente.\n");
    }
    while(escolha != 'r' && escolha != 'c' && escolha != 'v');
    return escolha;
}

/**
 * @brief Desenha o menu de opções relacionadas com transações
 * @return char A escolha do utilizador (minuscula)
 */
char menu_transacoes()
{
    char escolha;
    do{
        printf("\n====== Transacoes =======");
        printf("\n[R] Registar Transacao");
        printf("\n[C] Consultar Lista");
        printf("\n[V] Voltar ao Menu Anterior");
        printf("\n=========================");

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
    if(*num_registos < MAX_ESCOLAS)
    {
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
 * @brief 
 * 
 * @param transacoes 
 * @param num_registos 
 */
void registar_transacao(t_transacao transacoes[], int *quantidade_registos_transacoes, int *quantidade_registos_escolas, t_utilizador utilizadores[], int *quantidade_registos_utilizadores)
{
    int teste;
    if(*quantidade_registos_escolas && *quantidade_registos_utilizadores) // se qualquer uma das variáveis for 0, a condição falha
    {
        t_transacao temp;
        *quantidade_registos_transacoes = *quantidade_registos_transacoes + 1;
        temp.identificador = *quantidade_registos_transacoes;
        do{
            teste = ler_inteiro("Indique o identificador unico do utilizador", 0, MAX_UTILIZADORES);
            if(teste == 0)
                listar_utilizadores_paginado(utilizadores, &quantidade_registos_utilizadores);
        }while(teste == 0);
        temp.utilizador = teste;
    }
    else
    {
        system("cls");
        printf("\nNao e possivel registar transacoes. Confirme que existem utilizadores e/ou escolas registadas.\n");
    }
}

/**
 * @brief ** FUNÇÃO EXPERIMENTAL ** Lista todos os utilizadores de forma paginada
 * @param escolas 
 * @param registos_escolas 
 */
void listar_utilizadores_paginado(t_utilizador utilizadores[], int *registos_utilizadores)
{
    char selecao = '0'; // v = voltar, a = anterior, p = proxima
    int posicao, offset = 0, max_char_nome = 0, max_char_tipo = 0; // id, NIF e id da escola não precisam de ser calculados visto que têm caracteres fixos
    
    printf("%d", *registos_utilizadores);
    if(*registos_utilizadores > 0)
    {
        for(posicao = 0; posicao < *registos_utilizadores; posicao++)
        {
            max_char_nome = (strlen(utilizadores[posicao].nome) > max_char_nome ? strlen(utilizadores[posicao].nome) : max_char_nome);
        }
        
        do{
            system("cls");
            printf("\n#\tTipo\t\t");
            aplicar_tabs("Nome", max_char_nome);
            printf("NIF\t\tEscola\tSaldo");

            for(/* offset */; offset < (*registos_utilizadores); offset++)
            {
                printf("\n%d\t", utilizadores[offset].identificador);
                switch(utilizadores[offset].tipo)
                {
                    case '1':
                        aplicar_tabs("Estudante", 11);
                        break;

                    case '2':
                        aplicar_tabs("Docente", 11);
                        break;
                    
                    case '3':
                        aplicar_tabs("Funcionario", 11);
                        break;
                }
                aplicar_tabs(utilizadores[offset].nome, max_char_nome);
                printf("%d\t%d\tsaldo", utilizadores[offset].NIF, utilizadores[offset].escola);
            }
            printf("\nPagina %d de %d", (offset/25)+1, ceil(*registos_utilizadores/25)); //confirmar se as contas estão certas

            do{
                printf("\n(v)oltar, (a)nterior, (p)roxima? ");
                scanf(" %c", &selecao);
                if(selecao != 'v' && selecao != 'a' && selecao != 'p')
                    printf("\nOpcao Invalida. Tente Novamente.\n");
                
            }while(selecao != 'v' && selecao != 'a' && selecao != 'p');
            
            if(selecao == 'p')
            {
                if((offset + 25) < *registos_utilizadores)
                    offset += 25;
            }

            if(selecao == 'a')
            {
                if((offset-25) > 0)
                    offset -= 25;
            }
        }while(selecao != 'v');
    }else{
        printf("\n Sem registos a mostrar.\n");
    }
}

/**
 * @brief 
 * @param utilizadores 
 * @param num_registos 
 */
void registar_utilizadores(t_utilizador utilizadores[], int *num_registos)
{
    utilizadores[*num_registos].identificador = utilizadores[*num_registos].identificador + 1;
    utilizadores[*num_registos].escola = ler_inteiro("Insira o identificador da escola", 1, 5);
    ler_string(utilizadores[*num_registos].nome, "Indique o seu nome", 3, 100);
    utilizadores[*num_registos].NIF = ler_inteiro("NIF", 100000000, 299999999);
    printf("\nEstudante - 1\tDocente - 2\tFuncionario - 3\n");
    utilizadores[*num_registos].tipo = ler_inteiro("Escolha o tipo de utilizador consoante a linha", 1, 3);
    ler_string(utilizadores[*num_registos].email,"Indique o email: ",5,100);
    utilizadores[*num_registos].saldo = 0;
    *num_registos = *num_registos + 1;
}

/**
 * @brief
 * Pede as informações necessárias e regista o utilizador no vetor de estruturas t_utilizador
 * @param utilizadores vetor de estruturas do tipo t_utilizador onde a informação será guardada
 * @param num_registos (Ponteiro) Quantidade de registos já realizados. Utilizado para determinar
 * o índice do vetor onde a informação será guardada.
 */
void registar_utilizadores(t_utilizador utilizadores[],int*num_registos)
{
    system("cls");
    utilizadores[*num_registos].escola=ler_numero("Insira o identificador da escola a que pertence",1,5);
    ler_string(utilizadores[*num_registos].nome,"Indique o seu nome ",3,100);
    utilizadores[*num_registos].NIF=ler_numero("Insira o NIF",100000000,299999999);
    printf("0-Estudante\t1-Funcionário\t2-Docente\n");
    utilizadores[*num_registos].tipo=ler_numero("Insira o tipo de utilizador ",0,2);
    ler_string(utilizadores[*num_registos].email,"Indique o email ",5,100);
    *num_registos=*num_registos+1;
    utilizadores[*num_registos].identificador=*num_registos;
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
 * @brief Lista todos os utilizadores existentes
 * @param utilizadores Vetor de estruturas do tipo t_utilizador onde a informação está guardada
 * @param num_registos (Ponteiro) Quantidade de registos já realizados. Utilizado para iterar pelo vetor.
 */
void consultar_utilizadores(t_utilizador utilizadores[],int*num_registos)
{
    int posicao;

    printf("\n\nID\t");
    aplicar_tabs("Escola",1 );
    aplicar_tabs("Nome",1);
    aplicar_tabs("NIF",1);
    aplicar_tabs("Tipo",1);
    aplicar_tabs("Email",1);
    if(*num_registos>0){
        for(posicao=0;posicao<*num_registos;posicao++){
            printf("\n%d\t", utilizadores[posicao].identificador);
            printf("%d\t",utilizadores[posicao].escola);
            printf("%s\t",utilizadores[posicao].nome);
            printf("%d\t",utilizadores[posicao].NIF);
            printf("%d\t",utilizadores[posicao].tipo);
            printf("%s\t",utilizadores[posicao].email);
        }
    }
    else{
        printf("\n\nNao existem escolas registadas\n\n");
    }
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