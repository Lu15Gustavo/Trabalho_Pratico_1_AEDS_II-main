#ifndef ALUNO_H
#define ALUNO_H
#include "disciplina.h"
#define MAX_DISCIPLINAS 5

typedef struct {
    char nome[100];
    int matricula;
    char disciplina[100];
    char email[100];
    int disciplinas[MAX_DISCIPLINAS];
    int qtdDisciplinas;
} Aluno;
void cadastrarAluno(FILE *arq);
void listarAlunos(FILE *arq);
void salvarAlunos(FILE *arq);
void carregarAlunos(FILE *arq);
int buscaBinariaAluno(FILE *arq, int matricula);
void editarAluno(FILE *arq, int matricula);
int buscaSequencialAluno(FILE *arq, int matricula);
void bubbleSortAlunos(FILE *arq);
void cancelarMatriculaAluno( FILE *arq, int matricula);
void matricularAlunoEmDisciplina(FILE *arq, int matriculaAluno, int *codigosDisciplinas, int qtdDisciplinas);
void registrarLog(char *mensagem);
void gerarParticoesSelecaoNatural(FILE *arq, int tamanhoMemoria);
void verificarParticoes();
void intercalacaoOtima();
void verificarArquivoOrdenado();
void gerarRelatorioOperacoes();
void limparLog();

#endif
