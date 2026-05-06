#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ARQ_ALUNO      "alunos.txt"
#define ARQ_PROFESSOR  "professores.txt"
#define ARQ_DISCIPLINA "disciplinas.txt"
#define ARQ_MATRICULA  "matriculas.txt"

struct Aluno { int id; char nome[50]; char turma[20]; };
struct Professor { int id; char nome[50]; char area[50]; };
struct Disciplina { int id; char nome[50]; int id_prof; int carga; };
struct Matricula { int id_disc; int id_aluno; char periodo[20]; };

// --- UTILITÁRIOS ---

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int nomeEhValido(char *nome) {
    int i;
    if (strlen(nome) == 0) return 0;
    for (i = 0; nome[i] != '\0'; i++) {
        if (!isalpha(nome[i]) && !isspace(nome[i])) return 0;
    }
    return 1;
}

int gerarID(char *nomeArquivo) {
    FILE *f = fopen(nomeArquivo, "r");
    if (!f) return 1001;
    int id, maior = 1000;
    char linha[200];
    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%d;", &id) == 1) {
            if (id > maior) maior = id;
        }
    }
    fclose(f);
    return maior + 1;
}

// Verifica se um ID existe em um arquivo específico
int idExiste(char *nomeArquivo, int idProcurado) {
    FILE *f = fopen(nomeArquivo, "r");
    if (!f) return 0;
    int id;
    char linha[200];
    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d;", &id);
        if (id == idProcurado) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

// --- CADASTROS ---

void cadastrarAluno() {
    struct Aluno a;
    a.id = gerarID(ARQ_ALUNO); // ID automático, sem input do usuário
    limparBuffer();
    
    printf("\n--- Cadastrar Aluno (ID Gerado: %d) ---\n", a.id);
    do {
        printf("Nome: ");
        fgets(a.nome, 50, stdin);
        a.nome[strcspn(a.nome, "\n")] = 0;
    } while (!nomeEhValido(a.nome) && printf("[!] Nome invalido.\n"));

    printf("Turma: ");
    fgets(a.turma, 20, stdin);
    a.turma[strcspn(a.turma, "\n")] = 0;

    FILE *f = fopen(ARQ_ALUNO, "a");
    fprintf(f, "%d;%s;%s\n", a.id, a.nome, a.turma);
    fclose(f);
    printf(">> Aluno salvo com sucesso!\n");
}

void cadastrarProfessor() {
    struct Professor p;
    p.id = gerarID(ARQ_PROFESSOR);
    limparBuffer();

    printf("\n--- Cadastrar Professor (ID Gerado: %d) ---\n", p.id);
    do {
        printf("Nome: ");
        fgets(p.nome, 50, stdin);
        p.nome[strcspn(p.nome, "\n")] = 0;
    } while (!nomeEhValido(p.nome) && printf("[!] Nome invalido.\n"));

    printf("Area: ");
    fgets(p.area, 50, stdin);
    p.area[strcspn(p.area, "\n")] = 0;

    FILE *f = fopen(ARQ_PROFESSOR, "a");
    fprintf(f, "%d;%s;%s\n", p.id, p.nome, p.area);
    fclose(f);
    printf(">> Professor salvo!\n");
}

void cadastrarDisciplina() {
    struct Disciplina d;
    d.id = gerarID(ARQ_DISCIPLINA);
    
    printf("\n--- Cadastrar Disciplina (ID Gerado: %d) ---\n", d.id);
    printf("Nome da Disciplina: ");
    limparBuffer();
    fgets(d.nome, 50, stdin);
    d.nome[strcspn(d.nome, "\n")] = 0;

    // Validação do Professor
    printf("ID do Professor Responsavel: ");
    scanf("%d", &d.id_prof);
    if (!idExiste(ARQ_PROFESSOR, d.id_prof)) {
        printf("[ERRO] Professor com ID %d nao encontrado! Operacao cancelada.\n", d.id_prof);
        return;
    }

    printf("Carga Horaria: ");
    scanf("%d", &d.carga);

    FILE *f = fopen(ARQ_DISCIPLINA, "a");
    fprintf(f, "%d;%s;%d;%d\n", d.id, d.nome, d.id_prof, d.carga);
    fclose(f);
    printf(">> Disciplina salva!\n");
}

void cadastrarMatricula() {
    struct Matricula m;
    printf("\n--- Nova Matricula ---\n");
    
    printf("ID da Disciplina: ");
    scanf("%d", &m.id_disc);
    if (!idExiste(ARQ_DISCIPLINA, m.id_disc)) {
        printf("[ERRO] Disciplina inexistente!\n");
        return;
    }

    printf("ID do Aluno: ");
    scanf("%d", &m.id_aluno);
    if (!idExiste(ARQ_ALUNO, m.id_aluno)) {
        printf("[ERRO] Aluno inexistente!\n");
        return;
    }

    limparBuffer();
    printf("Periodo (ex: 2026.1): ");
    fgets(m.periodo, 20, stdin);
    m.periodo[strcspn(m.periodo, "\n")] = 0;

    FILE *f = fopen(ARQ_MATRICULA, "a");
    fprintf(f, "%d;%d;%s\n", m.id_disc, m.id_aluno, m.periodo);
    fclose(f);
    printf(">> Matricula confirmada!\n");
}

void listarTudo() {
    int i;
    char nomes[4][20] = {"ALUNOS", "PROFESSORES", "DISCIPLINAS", "MATRICULAS"};
    char arquivos[4][30] = {ARQ_ALUNO, ARQ_PROFESSOR, ARQ_DISCIPLINA, ARQ_MATRICULA};
    
    for(i = 0; i < 4; i++) {
        FILE *f = fopen(arquivos[i], "r");
        printf("\n=== %s ===\n", nomes[i]);
        if (!f) { printf("Vazio.\n"); continue; }
        char linha[200];
        while (fgets(linha, sizeof(linha), f)) printf("%s", linha);
        fclose(f);
    }
}

int main() {
    int opcao;
    while (1) {
        printf("\n=========== MENU ACADEMICO ===========\n");
        printf("1. Aluno    2. Professor    3. Disciplina\n");
        printf("4. Matricula  5. Listar Tudo  0. Sair\n");
        printf("======================================\nEscolha: ");
        
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            continue;
        }
        if (opcao == 0) break;

        switch (opcao) {
            case 1: cadastrarAluno(); break;
            case 2: cadastrarProfessor(); break;
            case 3: cadastrarDisciplina(); break;
            case 4: cadastrarMatricula(); break;
            case 5: listarTudo(); break;
            default: printf("Opcao invalida.\n");
        }
    }
    return 0;
}
