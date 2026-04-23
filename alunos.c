
#include <stdio.h>   
#include <stdlib.h>  //  remove, rename, system
#include <string.h>  // strcspn (para remover \n)


#define ARQUIVO "arquivo.txt" // CONSTANTE - Define o nome do arquivo usado no programa

// STRUCT (Estrutura) - Cria um novo tipo de dado chamado "Aluno"
struct Aluno {
    int id_aluno;      
    char nome[50];     
    char curso[50];    
    float nota;        
};

// FUNÇÃO: cadastrar()
void cadastrar() {
    
    // Se o arquivo NÃO existir, ele será CRIADO automaticamente
    FILE *arq = fopen(ARQUIVO, "a");

    // Verifica se o arquivo foi aberto com sucesso
    if (arq == NULL) {
        printf("Erro ao abrir arquivo.\n");  
        return;                             // Sai da função, volta ao menu
    }

    // Cria uma variável 'a' do tipo struct Aluno
    struct Aluno a;

    // ----- LÊ OS DADOS DO TECLADO -----
    
    printf("\nID do aluno: ");
    scanf("%d", &a.id_aluno);   // Lê o inteiro e guarda em a.id_aluno
                                // & = passa o ENDEREÇO da variável
    
    getchar();  //remove o ENTER (\n) que ficou no teclado
    
    printf("Nome: ");
    fgets(a.nome, 50, stdin);   // Lê uma LINHA inteira do teclado 
                                // 50 = tamanho máximo
                                // stdin = entrada padrão
    
   
    // strcspn procura a posição do \n e substitui por \0 (fim de string)
    a.nome[strcspn(a.nome, "\n")] = 0;

    printf("Curso: ");
    fgets(a.curso, 50, stdin);          // Lê o curso
    a.curso[strcspn(a.curso, "\n")] = 0; 

    printf("Nota: ");
    scanf("%f", &a.nota);  // Lê número decimal 

    // ----- GRAVA NO ARQUIVO -----
    
    // fprintf = printf, mas grava no ARQUIVO em vez da tela
    // Formato: id;nome;curso;nota 
    fprintf(arq, "%d;%s;%s;%.2f\n", a.id_aluno, a.nome, a.curso, a.nota);
    // %d = inteiro, %s = string, %.2f = float com 2 casas decimais,

    fclose(arq);  // FECHA o arquivo 
    printf("Aluno cadastrado com sucesso!\n");
}
// FUNÇÃO: listar()
// LER todos os alunos do arquivo e mostrar na tela
// MODO DE ABERTURA: "r" (read = leitura apenas)
void listar() {
    
    // Abre o arquivo em modo LEITURA 
    FILE *arq = fopen(ARQUIVO, "r");

    // Se arquivo não existe (ainda não cadastrou nada)
    if (arq == NULL) {
        printf("Nenhum aluno cadastrado.\n");
        return;  // Volta ao menu
    }

    // Cria variável para armazenar cada aluno lido
    struct Aluno a;

    printf("\n--- Lista de Alunos ---\n");

    // fscanf lê do arquivo no formato especificado
    // %d = inteiro 
    // %49[^;] = string de até 49 caracteres
    // %f = lê float
    // == 4  só continua se conseguiu ler os 4 campos corretamente
    while (fscanf(arq, "%d;%49[^;];%49[^;];%f\n",
                  &a.id_aluno, a.nome, a.curso, &a.nota) == 4) {
        
        // Mostra os dados do aluno na tela formatados
        printf("ID: %d | Nome: %s | Curso: %s | Nota: %.2f\n",
               a.id_aluno, a.nome, a.curso, a.nota);
    }

    fclose(arq);  // Fecha o arquivo
}

// FUNÇÃO: alterar()
// ATUALIZA todos os dados de um aluno (nome, curso, nota)
//Arquivo temporário (não dá para editar no meio do arquivo)
void alterar() {
    
    // Abre arquivo ORIGINAL para LEITURA
    FILE *arq = fopen(ARQUIVO, "r");
    
    // Cria arquivo TEMPORÁRIO para ESCRITA
    // guarda os dados atualizados
    FILE *temp = fopen("temp.txt", "w");

    // Verifica se o arquivo original existe
    if (arq == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    // Variáveis auxiliares
    int idBusca;           // Guarda o ID digitado
    struct Aluno a;        // Guarda cada aluno lido
    int encontrado = 0;    // Se 0 = não achou, 1 = achou o aluno

    printf("Digite o ID do aluno para alterar: ");
    scanf("%d", &idBusca);  // Lê o ID a procurar

    // ----- LOOP: LÊ CADA ALUNO DO ARQUIVO ORIGINAL -----
    while (fscanf(arq, "%d;%49[^;];%49[^;];%f\n",
                  &a.id_aluno, a.nome, a.curso, &a.nota) == 4) {

        // Verifica se é o aluno que queremos alterar
        if (a.id_aluno == idBusca) {
            encontrado = 1;  // Marca que achou

            // ----- LÊ OS NOVOS DADOS -----
            
            printf("Novo nome: ");
            getchar();  // Limpa ENTER do scanf anterior
            fgets(a.nome, 50, stdin);
            a.nome[strcspn(a.nome, "\n")] = 0;

            printf("Novo curso: ");
            fgets(a.curso, 50, stdin);
            a.curso[strcspn(a.curso, "\n")] = 0;

            printf("Nova nota: ");
            scanf("%f", &a.nota);
        }

        // Grava no arquivo TEMPORÁRIO (com dados novos OU originais)
        fprintf(temp, "%d;%s;%s;%.2f\n",
                a.id_aluno, a.nome, a.curso, a.nota);
    }

    // Fecha ambos os arquivos
    fclose(arq);
    fclose(temp);

    // ----- TROCA OS ARQUIVOS -----
    remove(ARQUIVO);           // DELETA o arquivo original
    rename("temp.txt", ARQUIVO);  // Renomeia o temporário para o nome original
                                  // Agora o temporário vira o "oficial"

    // Mostra mensagem de resultado
    if (encontrado)
        printf("Aluno alterado com sucesso!\n");
    else
        printf("Aluno não encontrado.\n");
}

// FUNÇÃO: alterarCurso(), ATUALIZA APENAS o curso de um aluno (não mexe no resto)
void alterarCurso() {
    
    FILE *arq = fopen(ARQUIVO, "r");    // leitura
    FILE *temp = fopen("temp.txt", "w"); // escrita

    if (arq == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    int idBusca;
    struct Aluno a;
    int encontrado = 0;

    printf("Digite o ID do aluno para mudar o curso: ");
    scanf("%d", &idBusca);

    while (fscanf(arq, "%d;%49[^;];%49[^;];%f\n",
                  &a.id_aluno, a.nome, a.curso, &a.nota) == 4) {

        if (a.id_aluno == idBusca) {
            encontrado = 1;

            printf("Curso atual: %s\n", a.curso);  // Mostra o curso atual

            printf("Novo curso: ");
            getchar();  // Limpa ENTER
            fgets(a.curso, 50, stdin);  // Lê apenas o novo curso
            a.curso[strcspn(a.curso, "\n")] = 0;
            // Nome e nota permanecem os mesmos!
        }

        // Grava no temporário (nome e nota originais, curso novo)
        fprintf(temp, "%d;%s;%s;%.2f\n",
                a.id_aluno, a.nome, a.curso, a.nota);
    }

    fclose(arq);
    fclose(temp);

    remove(ARQUIVO);
    rename("temp.txt", ARQUIVO);

    if (encontrado)
        printf("Curso atualizado!\n");
    else
        printf("Aluno não encontrado.\n");
}
// FUNÇÃO: deletar(), DELETAR um aluno do arquivo
//  Copia TUDO EXCETO o aluno deletado para o temporário
void deletar() {
    
    FILE *arq = fopen(ARQUIVO, "r");     //  leitura
    FILE *temp = fopen("temp.txt", "w");  // escrita

    if (arq == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    int idBusca;
    struct Aluno a;
    int encontrado = 0;

    printf("Digite o ID do aluno para deletar: ");
    scanf("%d", &idBusca);

    while (fscanf(arq, "%d;%49[^;];%49[^;];%f\n",
                  &a.id_aluno, a.nome, a.curso, &a.nota) == 4) {

        // DIFERENÇA AQUI: usa != (diferente) em vez de == (igual)
        // Se o ID for DIFERENTE do que queremos deletar, COPIA para o temporário
        // Se for IGUAL, NÃO copia (efetivamente DELETA)
        if (a.id_aluno != idBusca) {
            fprintf(temp, "%d;%s;%s;%.2f\n",
                    a.id_aluno, a.nome, a.curso, a.nota);
        } else {
            encontrado = 1;  // Marca que achou e "pulou" (deletou)
        }
    }

    fclose(arq);
    fclose(temp);

    remove(ARQUIVO);
    rename("temp.txt", ARQUIVO);

    if (encontrado)
        printf("Dados apagados!\n");
    else
        printf("Infelizmente não encontrei o aluno.\n");
}
 
int main() {       // FUNÇÃO PRINCIPAL
    
    int opcao;  // Guarda o número que o usuário digitar no menu

    // do-while: executa PELO MENOS UMA VEZ, repete enquanto condição for verdade
    do {
        printf("\n------------- Sistema de Alunos -------------\n");
        printf(" |          1 -> Cadastrar                    |\n");
        printf(" |          2 -> Listar                       |\n");
        printf(" |          3 -> Alterar tudo                 |\n");
        printf(" |          4 -> Deletar                      |\n");
        printf(" |          5 -> Alterar apenas curso         |\n");
        printf(" |          0 -> Sair                         |\n");
        printf("\n");
        printf("  Qual sua escolha: ");
        
        scanf("%d", &opcao);  // Lê a opção digitada

        // ----- SWITCH: EXECUTA A FUNÇÃO CORRESPONDENTE -----
        switch (opcao) {
            case 1: 
                cadastrar();      // Chama função de cadastrar
                break;            // Sai do switch, volta ao menu
            
            case 2: 
                listar();         // Chama função de listar
                break;
            
            case 3: 
                alterar();        // Chama função de alterar tudo
                break;
            
            case 4: 
                deletar();        // Chama função de deletar
                break;
            
            case 5: 
                alterarCurso();   // Chama função de alterar apenas curso
                break;
            
            case 0: 
                printf("Saindo...\n");  // Mensagem de despedida
                break;
            
            default: 
                // Se digitou número diferente de 0,1,2,3,4,5
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);  // Repete enquanto opção for diferente de 0 (Sair)

    return 0;  
}
