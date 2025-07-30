#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Necessário para a função time
#include "professor.h"
#include "aluno.h"
#include "disciplina.h"
#include "main.h"

    FILE *arq;
    int codigoDisciplina;
    char novoProfessor[100];
// Adiciona a declaração externa do array de professores
extern Professor professores[];
extern int qtdProfessores;
// Removido as declarações externas de arrays globais - agora trabalhamos com arquivos
// extern Aluno alunos[];
// extern int qtdAlunos;
// Adiciona a declaração externa do array de disciplinas
extern Disciplina disciplinas[];
extern int qtdDisciplinas;

void embaralhar(int *vet, int tam) {
    srand(time(NULL));
    for (int i = tam - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = vet[i];
        vet[i] = vet[j];
        vet[j] = temp;
    }
}

void criarBase(int tam) {
    printf("Criando base de dados com %d alunos...\n", tam);
    clock_t inicio = clock();
    
    // Cria arquivo diretamente
    FILE *arq = fopen("alunos.dat", "wb");
    if (arq == NULL) {
        printf("Erro ao criar arquivo de alunos.\n");
        return;
    }
    
    int i;
    int *vet = (int*)malloc(tam * sizeof(int));
    if (vet == NULL) {
        printf("Erro ao alocar memoria para embaralhamento.\n");
        fclose(arq);
        return;
    }

    // Preencher o vetor com índices sequenciais
    for (i = 0; i < tam; i++) {
        vet[i] = i;
    }

    // Embaralhar os índices
    embaralhar(vet, tam);

    // Criar e gravar dados de alunos embaralhados diretamente no arquivo
    for (i = 0; i < tam; i++) {
        Aluno aluno;
        int idx = vet[i];  // O índice embaralhado
        
        snprintf(aluno.nome, sizeof(aluno.nome), "Aluno%d", i + 1);
        aluno.matricula = 1000 + idx;  // Usar o índice embaralhado para matrícula
        snprintf(aluno.disciplina, sizeof(aluno.disciplina), "Disciplina%d", i + 1);
        snprintf(aluno.email, sizeof(aluno.email), "aluno%d@dominio.com", i + 1);
        
        aluno.qtdDisciplinas = 1;
        aluno.disciplinas[0] = 100 + i;
        
        // Grava diretamente no arquivo
        fwrite(&aluno, sizeof(Aluno), 1, arq);
    }
    
    fclose(arq);
    
    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    
    printf("Base de dados criada com sucesso!\n");
    printf("Total de alunos: %d\n", tam);
    printf("Tempo de criacao: %.2f ms\n", tempoGasto * 1000);
    printf("Arquivo: alunos.dat\n");
    
    // Log da operação
    char logMsg[256];
    snprintf(logMsg, sizeof(logMsg), "Criacao Base: %d alunos, %.2f ms", tam, tempoGasto * 1000);
    registrarLog(logMsg);
    
    // Criar dados de professores embaralhados
    int *vetProf = (int*)malloc(tam * sizeof(int));
    if (vetProf == NULL) {
        printf("Erro ao alocar memoria para professores.\n");
        free(vet);  // Libera vet antes de retornar
        return;
    }
    
    // Preencher e embaralhar índices para professores
    for (i = 0; i < tam; i++) {
        vetProf[i] = i;
    }
    embaralhar(vetProf, tam);
    
    for (i = 0; i < tam; i++) {
        int idx = vetProf[i];
        snprintf(professores[idx].nome, sizeof(professores[idx].nome), "Professor%d", i + 1);
        professores[idx].matricula = 2000 + i;
        snprintf(professores[idx].disciplina, sizeof(professores[idx].disciplina), "Disciplina%d", i + 1);
        professores[idx].salario = 3000 + (i * 100);
    }
    
    qtdProfessores = tam;

    // Abrir o arquivo para salvar os dados
    arq = fopen("professores.dat", "wb");
    if (arq != NULL) {
        salvarProfessores(arq);  // Salva os professores
        fclose(arq);
    } else {
        printf("Erro ao abrir professores.dat para salvar.\n");
    }

    // Criar dados de disciplinas embaralhadas
    int *vetDisc = (int*)malloc(tam * sizeof(int));
    if (vetDisc == NULL) {
        printf("Erro ao alocar memoria para disciplinas.\n");
        return;
    }
    
    // Preencher e embaralhar índices para disciplinas
    for (i = 0; i < tam; i++) {
        vetDisc[i] = i;
    }
    embaralhar(vetDisc, tam);
    
    for (i = 0; i < tam; i++) {
        int idx = vetDisc[i];
        disciplinas[idx].codigo = 100 + i;
        snprintf(disciplinas[idx].nome, sizeof(disciplinas[idx].nome), "Disciplina%d", i + 1);
        snprintf(disciplinas[idx].professor, sizeof(disciplinas[idx].professor), "Professor%d", i + 1);
    }
    
    // Libera todos os vetores
    free(vet);
    free(vetProf);
    free(vetDisc);
    qtdDisciplinas = tam;

    // Abrir o arquivo para salvar os dados
    arq = fopen("disciplinas.dat", "wb");
    if (arq != NULL) {
        salvarDisciplinas(arq);  // Salva as disciplinas
        fclose(arq);
    } else {
        printf("Erro ao abrir disciplinas.dat para salvar.\n");
    }

    printf("\nBase de dados criada com sucesso!\n");
}


// Função para imprimir base de dados de alunos, professores e disciplinas
void imprimirBase() {
    printf("\nImprimindo a base de dados...\n");

    // Listar alunos
    listarAlunos(arq);

    // Listar professores
    listarProfessores(arq);

    // Listar disciplinas
    listarDisciplinas(arq);
}

int main() {
    int escolha;
    int M = 50;

    do {
        printf("\nMENU\n");
        printf("1. Cadastrar Professor\n");
        printf("2. Cadastrar Aluno\n");
        printf("3. Cadastrar Disciplina\n");
        printf("4. Listar Professores\n");
        printf("5. Listar Alunos\n");
        printf("6. Listar Disciplinas\n");
        printf("7. Editar Professor\n");
        printf("8. Editar Aluno\n");
        printf("9. Buscar Professor com busca binaria\n");
        printf("10. Buscar Aluno com busca binaria\n");
        printf("11. Buscar Professor com busca sequencial\n"); 
        printf("12. Buscar Aluno com busca sequencial\n");
        printf("13. Ordenar Alunos pelo Bubble Sort\n");
        printf("14. Ordenar Professores pelo Bubble Sort\n");
        printf("15. Ordenar Disciplina pelo Bubble Sort\n");
        printf("16. Trocar Professor de Disciplina\n");
        printf("17. Cancelar Matricula de Aluno\n"); 
        printf("18. Matricular Aluno em uma ou mais disciplina\n"); 
        printf("19. Criar Base de Dados\n");
        printf("20. Gerar Particoes com Selecao Natural\n");
        printf("21. Verificar Conteudo das Particoes\n");
        printf("22. Intercalacao Otima\n");
        printf("23. Verificar Arquivo Final Ordenado\n");
        printf("24. Gerar Relatorio Detalhado\n");
        printf("25. Limpar Arquivo de Log\n");
        printf("\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);
        
        switch (escolha) {
            case 1: {
                FILE *arqProf = fopen("professores.dat", "ab");
                if (arqProf != NULL) {
                    cadastrarProfessor(arqProf);
                    fclose(arqProf);
                } else {
                    printf("Erro ao abrir professores.dat\n");
                }
                break;
            }
            case 2:
                FILE *arqAlunos = fopen("alunos.dat", "ab");
                if (arqAlunos != NULL) {
                    cadastrarAluno(arqAlunos);
                    fclose(arqAlunos);
                } else {
                    printf("Erro ao abrir alunos.dat\n");
                }
                break;
            case 3:
                FILE *arqDisc = fopen("disciplinas.dat", "ab");
                if (arqDisc != NULL) {
                    cadastrarDisciplina(arqDisc);
                    fclose(arqDisc);
                } else {
                    printf("Erro ao abrir alunos.dat\n");
                }
                break;
            case 4: {
                FILE *arqProf = fopen("professores.dat", "rb");
                if (arqProf != NULL) {
                    listarProfessores(arqProf);
                    fclose(arqProf);
                } else {
                    printf("Erro ao abrir professores.dat\n");
                }
                break;
            }
            case 5:{
                FILE *arqAlunos = fopen("alunos.dat", "rb");
                if (arqAlunos != NULL) {
                    listarAlunos(arqAlunos);
                    fclose(arqAlunos);
                } else {
                    printf("Erro ao abrir alunos.dat\n");
                }
                break;
            }
            case 6:{
                FILE *arqDisc = fopen("disciplinas.dat", "rb");
                if (arqDisc != NULL) {
                    listarDisciplinas(arqDisc);
                    fclose(arqDisc);
                } else {
                    printf("Erro ao abrir disciplinas.dat\n");
                }
                break;
            }
            case 7: {
                int matricula;
                printf("Digite a matricula do professor para editar: ");
                scanf("%d", &matricula);
                FILE *arqProf = fopen("professores.dat", "r+b");
                if (arqProf != NULL) {
                    editarProfessor(arqProf, matricula);
                    fclose(arqProf);
                } else {
                    printf("Erro ao abrir Professor.dat\n");
                }
                break;
            }
            case 8: {
                int matricula;
                printf("Digite a matricula do aluno para editar: ");
                scanf("%d", &matricula);
                FILE *arqAlunos = fopen("alunos.dat", "r+b");
                if (arqAlunos != NULL) {
                    editarAluno(arqAlunos, matricula);
                    fclose(arqAlunos);
                } else {
                    printf("Erro ao abrir alunos.dat\n");
                }
                break;
            }
            case 9: {
                int matricula;
                printf("Digite a matricula do professor para buscar: ");
                scanf("%d", &matricula);
                FILE *arqProf = fopen("professores.dat", "rb");
                if (arqProf != NULL) {
                    int indice = buscaBinariaProfessor(arqProf, matricula);
                    if (indice != -1) {
                        // Lê o professor encontrado do arquivo
                        Professor prof;
                        fseek(arqProf, indice * sizeof(Professor), SEEK_SET);
                        fread(&prof, sizeof(Professor), 1, arqProf);
                        printf("Professor encontrado: %s\n", prof.nome);
                    } else {
                        printf("Professor nao encontrado.\n");
                    }
                    fclose(arqProf);
                } else {
                    printf("Erro ao abrir professores.dat\n");
                }
                break;
            }
            case 10: {
                int matricula;
                printf("Digite a matricula do aluno para buscar: ");
                scanf("%d", &matricula);
                FILE *arqAlunos = fopen("alunos.dat", "rb");
                if (arqAlunos != NULL) {
                    int indice = buscaBinariaAluno(arqAlunos, matricula);
                    if (indice != -1) {
                        // Lê o aluno encontrado do arquivo
                        Aluno aluno;
                        fseek(arqAlunos, indice * sizeof(Aluno), SEEK_SET);
                        fread(&aluno, sizeof(Aluno), 1, arqAlunos);
                        printf("Aluno encontrado: %s\n", aluno.nome);
                    } else {
                        printf("Aluno nao encontrado.\n");
                    }
                    fclose(arqAlunos);
                } else {
                    printf("Erro ao abrir alunos.dat\n");
                }
                break;
            }
            case 11: {
                int matricula;
                printf("Digite a matricula do professor para buscar: ");
                scanf("%d", &matricula);
                FILE *arqProf = fopen("professores.dat", "rb");
                if (arqProf != NULL) {
                    int indice = buscaSequencialProfessor(arqProf, matricula);
                    if (indice != -1) {
                        // Lê o professor encontrado do arquivo
                        Professor prof;
                        fseek(arqProf, indice * sizeof(Professor), SEEK_SET);
                        fread(&prof, sizeof(Professor), 1, arqProf);
                        printf("Professor encontrado: %s\n", prof.nome);
                    } else {
                        printf("Professor nao encontrado.\n");
                    }
                    fclose(arqProf);
                } else {
                    printf("Erro ao abrir professores.dat\n");
                }
                break;
            }  
                
            case 12: {
                int matricula;
                printf("Digite a matricula do aluno para buscar: ");
                scanf("%d", &matricula);
                FILE *arqAlunos = fopen("alunos.dat", "rb");
                if (arqAlunos != NULL) {
                    int indice = buscaSequencialAluno(arqAlunos, matricula);
                    if (indice != -1) {
                        // Lê o aluno encontrado do arquivo
                        Aluno aluno;
                        fseek(arqAlunos, indice * sizeof(Aluno), SEEK_SET);
                        fread(&aluno, sizeof(Aluno), 1, arqAlunos);
                        printf("Aluno encontrado: %s\n", aluno.nome);
                    } else {
                        printf("Aluno nao encontrado.\n");
                    }
                    fclose(arqAlunos);
                } else {
                    printf("Erro ao abrir alunos.dat\n");
                }
                break;
            }
            case 13: {
                FILE *arqAlunos = fopen("alunos.dat", "r+b");
                if (arqAlunos != NULL) {
                    bubbleSortAlunos(arqAlunos);  // Ordena a base de dados dos alunos
                    fclose(arqAlunos);
                    printf("Base de alunos ordenada com sucesso!\n");
                } else {
                    printf("Erro ao abrir alunos.dat\n");
                }
                break;
            }
            case 14: {
                FILE *arqProf = fopen("professores.dat", "r+b");
                if (arqProf != NULL) {
                    bubbleSortProfessores(arqProf);  // Ordena a base de dados dos professores
                    fclose(arqProf);
                    printf("Base de professores ordenada com sucesso!\n");
                } else {
                    printf("Erro ao abrir professores.dat\n");
                }
                break;
            }
            case 15: {
                FILE *arq = fopen("disciplinas.dat", "r+b");
                if (arq != NULL) {
                    bubbleSortDisciplina(arq);  // Ordena a base de dados das disciplinas
                    fclose(arq);
                    printf("Base de disciplinas ordenada com sucesso!\n");
                } else {
                    printf("Erro ao abrir disciplinas.dat\n");
                }
                break;
            }   
            case 16: {
                FILE *arqDisc = abrirArquivoDisciplina("disciplinas.dat", "r+b");
                if (arqDisc != NULL) {
                    // Solicita o código da disciplina e o nome do novo professor
                    printf("Digite o codigo da disciplina: ");
                    scanf("%d", &codigoDisciplina);
                    printf("Digite o nome do novo professor: ");
                    getchar();  // Limpa o buffer
                    fgets(novoProfessor, sizeof(novoProfessor), stdin);
                    novoProfessor[strcspn(novoProfessor, "\n")] = '\0';  // Remove o '\n' do final da string

                    // Chama a função para trocar o professor responsável pela disciplina
                    trocarProfessorDisciplina(codigoDisciplina, novoProfessor, arqDisc);

                    // Fecha o arquivo após a operação
                    fclose(arqDisc);
                } else {
                    printf("Erro ao abrir disciplinas.dat\n");
                }
                break;
            }                
            case 17: {  // Caso para cancelar matrícula
                int matricula;
                printf("Digite a matrícula do aluno para cancelar: ");
                scanf("%d", &matricula);
                FILE *arqAlunos = fopen("alunos.dat", "r+b");
                if (arqAlunos != NULL) {
                    cancelarMatriculaAluno(arqAlunos, matricula);
                    fclose(arqAlunos);
                } else {
                    printf("Erro ao abrir alunos.dat\n");
                }
                break;
            } // Nova opção no menu   
            case 18: {  // Matricular aluno em uma ou mais disciplinas
                int matriculaAluno, qtdDisciplinas;
                printf("Digite a matrícula do aluno para matricular: ");
                scanf("%d", &matriculaAluno);

                printf("Digite o número de disciplinas para matricular: ");
                scanf("%d", &qtdDisciplinas);

                int codigosDisciplinas[qtdDisciplinas];

                for (int i = 0; i < qtdDisciplinas; i++) {
                    printf("Digite o código da disciplina %d: ", i + 1);
                    scanf("%d", &codigosDisciplinas[i]);
                }

                FILE *arqAlunos = fopen("alunos.dat", "r+b");
                if (arqAlunos != NULL) {
                    matricularAlunoEmDisciplina(arqAlunos, matriculaAluno, codigosDisciplinas, qtdDisciplinas);
                    fclose(arqAlunos);
                } else {
                    printf("Erro ao abrir alunos.dat\n");
                }
                break;
            }
            case 19: {  // Nova opção no menu
                int tam;
                printf("Digite a quantidade de registros para a base de dados: ");
                scanf("%d", &tam);
                criarBase(tam);  // Chama a função para criar a base
                break; 
                }
            case 20: {  // Gerar partições com seleção natural
                int tamanhoMemoria;
                printf("Digite o tamanho da memória (número de alunos por partição): ");
                scanf("%d", &tamanhoMemoria);
                
                FILE *arqAlunos = fopen("alunos.dat", "rb");
                if (arqAlunos != NULL) {
                    gerarParticoesSelecaoNatural(arqAlunos, tamanhoMemoria);
                    fclose(arqAlunos);
                } else {
                    printf("Erro ao abrir alunos.dat\n");
                }
                break;
            }
            case 21: {  // Verificar conteúdo das partições
                verificarParticoes();
                break;
            }
            case 22: {  // Intercalação ótima
                // Criar arquivo temporário para a intercalação
                FILE *arqTemp = fopen("alunos_temp.dat", "wb");
                if (arqTemp != NULL) {
                    intercalacaoOtima(arqTemp);
                    fclose(arqTemp);
                    
                    // Substituir o arquivo original pelo arquivo ordenado
                    if (remove("alunos.dat") == 0) {
                        if (rename("alunos_temp.dat", "alunos.dat") == 0) {
                            printf("Arquivo alunos.dat foi substituido pela versao ordenada!\n");
                        } else {
                            printf("Erro ao renomear arquivo temporario.\n");
                        }
                    } else {
                        printf("Erro ao remover arquivo original.\n");
                    }
                } else {
                    printf("Erro ao criar arquivo temporario para intercalacao.\n");
                }
                break;
            }
            case 23: {  // Verificar arquivo final ordenado
                verificarArquivoOrdenado();
                break;
            }
            case 24: {  // Gerar relatório detalhado
                gerarRelatorioOperacoes();
                break;
            }
            case 25: {  // Limpar arquivo de log
                limparLog();
                break;
            }
            case 0:{
                // Sair do programa
                printf("Saindo do programa...\n");
                break;    
            }      
        }   
    } while (escolha != 0);

    return 0;
}
