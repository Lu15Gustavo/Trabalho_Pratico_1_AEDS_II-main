#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Necessário para a função time
#include <limits.h> // Para INT_MAX
#include "aluno.h"
#include "disciplina.h"

// Adiciona a declaração externa de qtdDisciplinas, assumindo que está definida em disciplina.c
extern int qtdDisciplinas;
extern Disciplina disciplinas[]; // Adiciona a declaração externa do vetor de disciplinas
#define MAX_ALUNOS 1000000

// Variáveis globais removidas - trabalharemos apenas com arquivos (memória secundária)

void registrarLog(char *mensagem) {
    FILE *logFile = fopen("buscas.log", "a"); // Abre o arquivo log em modo de adição
    if (logFile != NULL) {
        fprintf(logFile, "%s\n", mensagem); // Grava a mensagem no arquivo
        fclose(logFile); // Fecha o arquivo
    } else {
        printf("Erro ao abrir o arquivo de log.\n");
    }
}
void cadastrarAluno(FILE *arq) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de alunos.\n");
        return;
    }

    Aluno aluno;
    printf("\nCadastrar Aluno\n");
    printf("Nome: ");
    scanf("%s", aluno.nome);
    printf("Matricula: ");
    scanf("%d", &aluno.matricula);
    printf("Disciplina: ");
    scanf("%s", aluno.disciplina);
    printf("Email: ");
    scanf("%s", aluno.email);
    aluno.qtdDisciplinas = 0; // Inicializa o número de disciplinas do aluno

    // Grava o aluno no arquivo de forma binária
    fwrite(&aluno, sizeof(Aluno), 1, arq);

    printf("Aluno cadastrado com sucesso!\n");
}



void listarAlunos(FILE *arq) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de alunos.\n");
        return;
    }

    Aluno aluno;
    printf("\nListar Alunos\n");
    
    // Lê cada aluno do arquivo até o final
    while (fread(&aluno, sizeof(Aluno), 1, arq)) {
        printf("Nome: %s | Matricula: %d | E-mail: %s\n", aluno.nome, aluno.matricula, aluno.email);
        printf("Disciplinas matriculadas: ");
        
        // Exibe as disciplinas do aluno
        for (int j = 0; j < aluno.qtdDisciplinas; j++) {
            printf("%d", aluno.disciplinas[j]);
            if (j < aluno.qtdDisciplinas - 1) printf(", ");
        }
        printf("\n");
    }
}



void salvarAlunos(FILE *arq) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de alunos.\n");
        return;
    }

    // Esta função agora apenas sincroniza o arquivo (flush)
    // Dados já estão sendo gravados diretamente no arquivo
    clock_t inicio = clock();
    
    // Força a sincronização dos dados com o disco
    fflush(arq);
    
    // Obtém estatísticas do arquivo
    fseek(arq, 0, SEEK_END);
    long tamanhoArquivo = ftell(arq);
    int totalAlunos = tamanhoArquivo / sizeof(Aluno);
    
    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    
    printf("Arquivo sincronizado com sucesso!\n");
    printf("Total de alunos no arquivo: %d\n", totalAlunos);
    printf("Tamanho do arquivo: %ld bytes\n", tamanhoArquivo);
    printf("Tempo de sincronizacao: %.2f ms\n", tempoGasto * 1000);
    
    // Log da operação
    char logMsg[256];
    snprintf(logMsg, sizeof(logMsg), "Sincronizacao Arquivo: %d alunos, %ld bytes, %.2f ms", 
             totalAlunos, tamanhoArquivo, tempoGasto * 1000);
    registrarLog(logMsg);

    printf("Dados dos alunos sincronizados com sucesso!\n");
}


void carregarAlunos(FILE *arq) {
    if (arq != NULL) {
        clock_t inicio = clock();
        
        // Obtém informações do arquivo sem carregar na memória
        fseek(arq, 0, SEEK_END);
        long tamanhoArquivo = ftell(arq);
        int totalAlunos = tamanhoArquivo / sizeof(Aluno);
        fseek(arq, 0, SEEK_SET);
        
        clock_t fim = clock();
        double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
        
        printf("Arquivo de alunos verificado com sucesso!\n");
        printf("Total de alunos no arquivo: %d\n", totalAlunos);
        printf("Tamanho do arquivo: %ld bytes\n", tamanhoArquivo);
        printf("Tempo de verificacao: %.2f ms\n", tempoGasto * 1000);
        
        // Log da operação
        char logMsg[256];
        snprintf(logMsg, sizeof(logMsg), "Verificacao Arquivo: %d alunos, %ld bytes, %.2f ms", 
                 totalAlunos, tamanhoArquivo, tempoGasto * 1000);
        registrarLog(logMsg);
        
        // Não carrega na memória - apenas verifica o arquivo
        printf("NOTA: Dados mantidos em memoria secundaria (arquivo).\n");
    } else {
        printf("Erro ao abrir o arquivo de alunos.\n");
    }
}

int buscaBinariaAluno(FILE *arq, int matricula) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de alunos.\n");
        return -1;
    }

    clock_t comeco, final;
    double tempoGasto;
    int comparacoes = 0; // Contador de comparações
    comeco = clock();

    int inicio = 0, fim;
    
    // Obtém o número total de alunos no arquivo para definir o valor de "fim"
    fseek(arq, 0, SEEK_END); // Move o ponteiro do arquivo para o final
    long tamanhoArquivo = ftell(arq);  // Obtém o tamanho do arquivo
    fim = tamanhoArquivo / sizeof(Aluno) - 1; // Define "fim" como o índice do último aluno

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;

        // Posiciona o ponteiro do arquivo na posição do "meio" para ler o aluno
        fseek(arq, meio * sizeof(Aluno), SEEK_SET);
        Aluno aluno;
        fread(&aluno, sizeof(Aluno), 1, arq);  // Lê o aluno do arquivo

        comparacoes++; // Incrementa a contagem de comparações

        if (aluno.matricula == matricula) {
            final = clock();
            tempoGasto = (double)(final - comeco) / CLOCKS_PER_SEC;
            char logMsg[256];
            snprintf(logMsg, sizeof(logMsg), "Busca Binária: %d comparações, tempo: %.2f ms", comparacoes, tempoGasto * 1000);
            registrarLog(logMsg); // Registra no arquivo log
            return meio;  // Retorna o índice do aluno encontrado
        } else if (aluno.matricula < matricula) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    final = clock();
    tempoGasto = (double)(final - comeco) / CLOCKS_PER_SEC;
    char logMsg[256];
    snprintf(logMsg, sizeof(logMsg), "Busca Binária: %d comparações, tempo: %.2f ms (não encontrado)", comparacoes, tempoGasto * 1000);
    registrarLog(logMsg); // Registra no arquivo log

    return -1;  // Se não encontrar o aluno, retorna -1
}



int buscaSequencialAluno(FILE *arq, int matricula) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de alunos.\n");
        return -1;
    }

    clock_t comeco, final;
    double tempoGasto;
    int comparacoes = 0;
    comeco = clock();

    Aluno aluno;
    int i = 0;

    // Lê o arquivo e percorre os alunos sequencialmente
    while (fread(&aluno, sizeof(Aluno), 1, arq)) {
        comparacoes++;  // Incrementa a contagem de comparações
        if (aluno.matricula == matricula) {
            final = clock();
            tempoGasto = (double)(final - comeco) / CLOCKS_PER_SEC;
            char logMsg[256];
            snprintf(logMsg, sizeof(logMsg), "Busca Sequencial: %d comparações, tempo: %.2f ms", comparacoes, tempoGasto * 1000);
            registrarLog(logMsg);  // Registra no arquivo log
            return i;  // Retorna o índice do aluno encontrado
        }
        i++;  // Incrementa o índice do aluno
    }

    final = clock();
    tempoGasto = (double)(final - comeco) / CLOCKS_PER_SEC;
    char logMsg[256];
    snprintf(logMsg, sizeof(logMsg), "Busca Sequencial: %d comparações, tempo: %.2f ms (não encontrado)", comparacoes, tempoGasto * 1000);
    registrarLog(logMsg);  // Registra no arquivo log

    return -1;  // Se não encontrar o aluno, retorna -1
}



// Função de ordenação Bubble Sort para alunos - Versão Memória Secundária
void bubbleSortAlunos(FILE *arq) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de alunos.\n");
        return;
    }

    clock_t inicio = clock();
    
    // Calcula o número total de alunos no arquivo
    fseek(arq, 0, SEEK_END);
    long tamanhoArquivo = ftell(arq);
    int qtdAlunos = tamanhoArquivo / sizeof(Aluno);
    
    printf("Iniciando ordenacao de %d alunos usando memoria secundaria...\n", qtdAlunos);
    
    int trocas = 0;
    int comparacoes = 0;
    
    // Bubble Sort otimizado para trabalhar diretamente no arquivo
    for (int i = 0; i < qtdAlunos - 1; i++) {
        int houveTroca = 0;
        
        for (int j = 0; j < qtdAlunos - 1 - i; j++) {
            // Lê dois alunos consecutivos do arquivo
            Aluno aluno1, aluno2;
            
            // Posiciona e lê o primeiro aluno
            fseek(arq, j * sizeof(Aluno), SEEK_SET);
            fread(&aluno1, sizeof(Aluno), 1, arq);
            
            // Lê o segundo aluno
            fread(&aluno2, sizeof(Aluno), 1, arq);
            
            comparacoes++;
            
            // Se estão fora de ordem, troca
            if (aluno1.matricula > aluno2.matricula) {
                // Reposiciona para escrever na ordem correta
                fseek(arq, j * sizeof(Aluno), SEEK_SET);
                fwrite(&aluno2, sizeof(Aluno), 1, arq);
                fwrite(&aluno1, sizeof(Aluno), 1, arq);
                
                trocas++;
                houveTroca = 1;
            }
        }
        
        // Se não houve trocas, o array já está ordenado
        if (!houveTroca) {
            break;
        }
        
        if ((i + 1) % 10 == 0) {
            printf("Passada %d concluida...\n", i + 1);
        }
    }
    
    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    
    printf("\nOrdenacao concluida!\n");
    printf("Total de comparacoes: %d\n", comparacoes);
    printf("Total de trocas: %d\n", trocas);
    printf("Tempo de execucao: %.2f ms\n", tempoGasto * 1000);
    
    // Log da operação
    char logMsg[256];
    snprintf(logMsg, sizeof(logMsg), "Bubble Sort (Mem. Secundaria): %d alunos, %d comparacoes, %d trocas, %.2f ms", 
             qtdAlunos, comparacoes, trocas, tempoGasto * 1000);
    registrarLog(logMsg);

    printf("\nBase de dados de alunos ordenada com sucesso!\n");
}



void matricularAlunoEmDisciplina(FILE *arq, int matriculaAluno, int *codigosDisciplinas, int qtdDisciplinas) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de alunos.\n");
        return;
    }

    Aluno aluno;
    int encontrado = 0;

    // Busca o aluno no arquivo
    while (fread(&aluno, sizeof(Aluno), 1, arq)) {
        if (aluno.matricula == matriculaAluno) {
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Aluno não encontrado.\n");
        return;
    }

    // Matricular o aluno em cada disciplina
    for (int i = 0; i < qtdDisciplinas; i++) {
        // Verifica se o aluno já está matriculado nesta disciplina
        int jaMatriculado = 0;
        for (int j = 0; j < aluno.qtdDisciplinas; j++) {
            if (aluno.disciplinas[j] == codigosDisciplinas[i]) {
                jaMatriculado = 1;  // O aluno já está matriculado nesta disciplina
                break;
            }
        }

        // Se não estiver matriculado, adiciona a disciplina
        if (!jaMatriculado && aluno.qtdDisciplinas < MAX_DISCIPLINAS) {
            aluno.disciplinas[aluno.qtdDisciplinas] = codigosDisciplinas[i];
            aluno.qtdDisciplinas++;
            printf("Aluno matriculado na disciplina %d com sucesso.\n", codigosDisciplinas[i]);
        } else if (jaMatriculado) {
            printf("Aluno já está matriculado na disciplina %d.\n", codigosDisciplinas[i]);
        } else {
            printf("Aluno não pode ser matriculado em mais de %d disciplinas.\n", MAX_DISCIPLINAS);
            break;
        }
    }

    // Volta o ponteiro do arquivo para a posição do aluno encontrado e grava as alterações
    // Reposiciona o ponteiro no arquivo para a posição correta do aluno
    fseek(arq, -(long)(sizeof(Aluno)), SEEK_CUR);
    fwrite(&aluno, sizeof(Aluno), 1, arq);  // Grava os dados do aluno atualizados de volta no arquivo

    printf("Matricula realizada com sucesso!\n");
}




void editarAluno(FILE *arq, int matricula) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de alunos.\n");
        return;
    }

    Aluno aluno;
    int encontrado = 0;

    // Busca o aluno no arquivo
    while (fread(&aluno, sizeof(Aluno), 1, arq)) {
        if (aluno.matricula == matricula) {
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Aluno não encontrado.\n");
        return;
    }

    // Solicita a nova informação para o aluno
    printf("Novo Nome: ");
    scanf("%s", aluno.nome);
    printf("Nova Disciplina: ");
    scanf("%s", aluno.disciplina);
    printf("Novo E-mail: ");
    scanf("%s", aluno.email);

    // Volta o ponteiro do arquivo para a posição do aluno encontrado e grava as alterações
    // Reposiciona o ponteiro de volta para a posição do aluno no arquivo para a gravação dos dados atualizados
    long posicao = ftell(arq) - sizeof(Aluno);  // Obtém a posição onde o aluno foi lido
    fseek(arq, posicao, SEEK_SET);  // Volta para a posição do aluno

    fwrite(&aluno, sizeof(Aluno), 1, arq);  // Grava os dados do aluno atualizados de volta no arquivo

    printf("Dados do aluno atualizados com sucesso!\n");
}



// Função para cancelar matrícula usando memória secundária
void cancelarMatriculaAluno(FILE *arq, int matricula) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de alunos.\n");
        return;
    }

    clock_t inicio = clock();
    
    // Cria arquivo temporário para armazenar alunos não removidos
    FILE *arqTemp = fopen("temp_alunos.dat", "wb");
    if (arqTemp == NULL) {
        printf("Erro ao criar arquivo temporario.\n");
        return;
    }

    Aluno aluno;
    int encontrado = 0;
    int totalAlunos = 0;
    int alunosRestantes = 0;
    
    // Posiciona no início do arquivo original
    fseek(arq, 0, SEEK_SET);
    
    printf("Processando remocao da matricula %d...\n", matricula);
    
    // Lê cada aluno e copia para arquivo temporário (exceto o que será removido)
    while (fread(&aluno, sizeof(Aluno), 1, arq)) {
        totalAlunos++;
        
        if (aluno.matricula == matricula) {
            encontrado = 1;
            printf("Aluno encontrado: %s (Matricula: %d)\n", aluno.nome, aluno.matricula);
        } else {
            // Copia aluno para arquivo temporário
            fwrite(&aluno, sizeof(Aluno), 1, arqTemp);
            alunosRestantes++;
        }
    }
    
    fclose(arqTemp);
    
    if (!encontrado) {
        printf("Aluno com matricula %d nao encontrado.\n", matricula);
        remove("temp_alunos.dat"); // Remove arquivo temporário
        return;
    }
    
    // Fecha arquivo original para poder renomear
    fclose(arq);
    
    // Substitui arquivo original pelo temporário
    remove("alunos.dat"); // Remove arquivo original
    rename("temp_alunos.dat", "alunos.dat"); // Renomeia temporário
    
    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    
    printf("\nRemocao concluida com sucesso!\n");
    printf("Alunos processados: %d\n", totalAlunos);
    printf("Alunos restantes: %d\n", alunosRestantes);
    printf("Tempo de execucao: %.2f ms\n", tempoGasto * 1000);
    
    // Log da operação
    char logMsg[256];
    snprintf(logMsg, sizeof(logMsg), "Remocao Aluno (Mem. Secundaria): matricula %d, %d alunos processados, %.2f ms", 
             matricula, totalAlunos, tempoGasto * 1000);
    registrarLog(logMsg);

    printf("Aluno de matrícula %d removido com sucesso.\n", matricula);
    
    // Nota: O arquivo foi fechado, será necessário reabrir no main
    printf("AVISO: Arquivo foi fechado. Reabra o arquivo no menu principal.\n");
}

// Função para gerar partições usando seleção natural
void gerarParticoesSelecaoNatural(FILE *arq, int tamanhoMemoria) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de alunos.\n");
        return;
    }

    // Cria buffer para armazenar alunos em memória
    Aluno *buffer = (Aluno*)malloc(tamanhoMemoria * sizeof(Aluno));
    if (buffer == NULL) {
        printf("Erro ao alocar memória para o buffer.\n");
        return;
    }

    int numParticao = 0;
    int alunosLidos = 0;
    int totalAlunos = 0;
    clock_t inicioGeral = clock(); // Marca início da operação

    // Calcula o número total de alunos no arquivo
    fseek(arq, 0, SEEK_END);
    long tamanhoArquivo = ftell(arq);
    totalAlunos = tamanhoArquivo / sizeof(Aluno);
    fseek(arq, 0, SEEK_SET);

    printf("\nIniciando geração de partições com seleção natural...\n");
    printf("Total de alunos: %d\n", totalAlunos);
    printf("Tamanho da memória: %d alunos\n", tamanhoMemoria);
    
    // Log detalhado do início
    char logMsg[512];
    snprintf(logMsg, sizeof(logMsg), "=== INICIO SELECAO NATURAL ===");
    registrarLog(logMsg);
    snprintf(logMsg, sizeof(logMsg), "Total de alunos a processar: %d", totalAlunos);
    registrarLog(logMsg);
    snprintf(logMsg, sizeof(logMsg), "Tamanho da memoria: %d alunos", tamanhoMemoria);
    registrarLog(logMsg);

    while ((alunosLidos = fread(buffer, sizeof(Aluno), tamanhoMemoria, arq)) > 0) {
        // Ordena o buffer atual usando bubble sort
        for (int i = 0; i < alunosLidos - 1; i++) {
            for (int j = 0; j < alunosLidos - 1 - i; j++) {
                if (buffer[j].matricula > buffer[j + 1].matricula) {
                    Aluno temp = buffer[j];
                    buffer[j] = buffer[j + 1];
                    buffer[j + 1] = temp;
                }
            }
        }

        // Aplica seleção natural para criar sequências ordenadas maiores
        int *marcados = (int*)calloc(alunosLidos, sizeof(int));
        int sequenciaAtual = 0;
        
        // Cria nome do arquivo da partição
        char nomeArquivo[256];
        snprintf(nomeArquivo, sizeof(nomeArquivo), "particoes/particao_%d.dat", numParticao);
        
        FILE *arquivoParticao = fopen(nomeArquivo, "wb");
        if (arquivoParticao == NULL) {
            printf("Erro ao criar arquivo de particao %s\n", nomeArquivo);
            free(buffer);
            free(marcados);
            return;
        }

        // Implementa seleção natural
        int ultimoEscrito = -1;
        int alunosEscritos = 0;
        
        while (alunosEscritos < alunosLidos) {
            int menorIndice = -1;
            int menorMatricula = INT_MAX;
            
            // Encontra o próximo aluno válido na sequência
            for (int i = 0; i < alunosLidos; i++) {
                if (!marcados[i] && buffer[i].matricula >= ultimoEscrito && buffer[i].matricula < menorMatricula) {
                    menorMatricula = buffer[i].matricula;
                    menorIndice = i;
                }
            }
            
            // Se não encontrou nenhum válido, reinicia a busca (nova sequência)
            if (menorIndice == -1) {
                ultimoEscrito = -1;
                for (int i = 0; i < alunosLidos; i++) {
                    if (!marcados[i] && buffer[i].matricula < menorMatricula) {
                        menorMatricula = buffer[i].matricula;
                        menorIndice = i;
                    }
                }
            }
            
            if (menorIndice != -1) {
                // Escreve o aluno no arquivo da partição
                fwrite(&buffer[menorIndice], sizeof(Aluno), 1, arquivoParticao);
                marcados[menorIndice] = 1;
                ultimoEscrito = buffer[menorIndice].matricula;
                alunosEscritos++;
            } else {
                break; // Não deveria acontecer, mas previne loop infinito
            }
        }

        fclose(arquivoParticao);
        free(marcados);
        
        printf("Particao %d criada com %d alunos (arquivo: %s)\n", 
               numParticao, alunosLidos, nomeArquivo);
        numParticao++;
    }

    free(buffer);
    
    clock_t fimGeral = clock();
    double tempoTotal = (double)(fimGeral - inicioGeral) / CLOCKS_PER_SEC;
    
    printf("\nParticionamento concluido!\n");
    printf("Total de particoes criadas: %d\n", numParticao);
    printf("Particoes salvas na pasta: particoes/\n");
    
    // Log detalhado do resultado
    snprintf(logMsg, sizeof(logMsg), "Particoes criadas: %d", numParticao);
    registrarLog(logMsg);
    snprintf(logMsg, sizeof(logMsg), "Tempo total selecao natural: %.2f ms", tempoTotal * 1000);
    registrarLog(logMsg);
    snprintf(logMsg, sizeof(logMsg), "Alunos por particao (media): %.1f", (double)totalAlunos / numParticao);
    registrarLog(logMsg);
    snprintf(logMsg, sizeof(logMsg), "=== FIM SELECAO NATURAL ===");
    registrarLog(logMsg);
}

// Função para verificar o conteúdo das partições
void verificarParticoes() {
    printf("\nVerificando conteudo das particoes:\n");
    printf("=====================================\n");
    
    int numParticao = 0;
    char nomeArquivo[256];
    
    while (1) {
        snprintf(nomeArquivo, sizeof(nomeArquivo), "particoes/particao_%d.dat", numParticao);
        
        FILE *arquivoParticao = fopen(nomeArquivo, "rb");
        if (arquivoParticao == NULL) {
            break; // Não há mais partições
        }
        
        printf("\nPartição %d (%s):\n", numParticao, nomeArquivo);
        
        Aluno aluno;
        int contador = 0;
        int ultimaMatricula = -1;
        int ordenada = 1;
        
        while (fread(&aluno, sizeof(Aluno), 1, arquivoParticao)) {
            printf("  Aluno %d: %s (Matricula: %d)\n", contador + 1, aluno.nome, aluno.matricula);
            
            // Verifica se está ordenada
            if (ultimaMatricula != -1 && aluno.matricula < ultimaMatricula) {
                ordenada = 0;
            }
            ultimaMatricula = aluno.matricula;
            contador++;
        }
        
        printf("  Total de alunos: %d\n", contador);
        printf("  Particao ordenada: %s\n", ordenada ? "SIM" : "NAO");
        
        fclose(arquivoParticao);
        numParticao++;
    }
    
    if (numParticao == 0) {
        printf("Nenhuma particao encontrada. Execute primeiro a geracao de partições.\n");
    } else {
        printf("\nTotal de partições verificadas: %d\n", numParticao);
    }
}

// Estrutura para controlar cada partição durante a intercalação
typedef struct {
    FILE *arquivo;
    Aluno alunoAtual;
    int temDados;
    int numeroParticao;
} ControleParticao;

// Função para encontrar a próxima partição com menor matrícula
int encontrarMenorMatricula(ControleParticao *particoes, int numParticoes) {
    int menorIndice = -1;
    int menorMatricula = INT_MAX;
    
    for (int i = 0; i < numParticoes; i++) {
        if (particoes[i].temDados && particoes[i].alunoAtual.matricula < menorMatricula) {
            menorMatricula = particoes[i].alunoAtual.matricula;
            menorIndice = i;
        }
    }
    
    return menorIndice;
}

// Função de intercalação ótima
void intercalacaoOtima(FILE *arquivoDestino) {
    if (arquivoDestino == NULL) {
        printf("Erro: arquivo de destino nao fornecido.\n");
        return;
    }
    
    printf("\nIniciando intercalacao otima das particoes...\n");
    
    // Log do início da intercalação
    char logMsg[512];
    snprintf(logMsg, sizeof(logMsg), "=== INICIO INTERCALACAO OTIMA ===");
    registrarLog(logMsg);
    
    // Contar quantas partições existem
    int numParticoes = 0;
    char nomeArquivo[256];
    
    while (1) {
        snprintf(nomeArquivo, sizeof(nomeArquivo), "particoes/particao_%d.dat", numParticoes);
        FILE *teste = fopen(nomeArquivo, "rb");
        if (teste == NULL) {
            break;
        }
        fclose(teste);
        numParticoes++;
    }
    
    if (numParticoes == 0) {
        printf("Nenhuma particao encontrada. Execute primeiro a geracao de particoes.\n");
        snprintf(logMsg, sizeof(logMsg), "ERRO: Nenhuma particao encontrada");
        registrarLog(logMsg);
        return;
    }
    
    printf("Encontradas %d particoes para intercalar.\n", numParticoes);
    snprintf(logMsg, sizeof(logMsg), "Particoes a intercalar: %d", numParticoes);
    registrarLog(logMsg);
    
    // Alocar memória para controlar as partições
    ControleParticao *particoes = (ControleParticao*)malloc(numParticoes * sizeof(ControleParticao));
    if (particoes == NULL) {
        printf("Erro ao alocar memoria para intercalacao.\n");
        return;
    }
    
    // Abrir todas as partições e ler o primeiro aluno de cada uma
    for (int i = 0; i < numParticoes; i++) {
        snprintf(nomeArquivo, sizeof(nomeArquivo), "particoes/particao_%d.dat", i);
        particoes[i].arquivo = fopen(nomeArquivo, "rb");
        particoes[i].numeroParticao = i;
        
        if (particoes[i].arquivo != NULL) {
            particoes[i].temDados = fread(&particoes[i].alunoAtual, sizeof(Aluno), 1, particoes[i].arquivo);
        } else {
            particoes[i].temDados = 0;
            printf("Erro ao abrir particao %d\n", i);
        }
    }
    
    // Usar o arquivo de destino fornecido como parâmetro
    FILE *arquivoSaida = arquivoDestino;
    
    // Posiciona no início do arquivo para garantir que escreverá do começo
    fseek(arquivoSaida, 0, SEEK_SET);
    
    int totalAlunos = 0;
    int comparacoes = 0;
    clock_t inicio = clock();
    
    // Processo de intercalação
    while (1) {
        // Encontrar a partição com menor matrícula
        int menorIndice = encontrarMenorMatricula(particoes, numParticoes);
        comparacoes++;
        
        if (menorIndice == -1) {
            break; // Todas as partições foram processadas
        }
        
        // Escrever o aluno com menor matrícula no arquivo de saída
        fwrite(&particoes[menorIndice].alunoAtual, sizeof(Aluno), 1, arquivoSaida);
        totalAlunos++;
        
        // Ler próximo aluno da partição que forneceu o menor
        particoes[menorIndice].temDados = fread(&particoes[menorIndice].alunoAtual, 
                                               sizeof(Aluno), 1, 
                                               particoes[menorIndice].arquivo);
    }
    
    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    
    // Força sincronização dos dados com o disco
    fflush(arquivoSaida);
    
    // Fechar todos os arquivos das partições (mas não o arquivo de destino)
    for (int i = 0; i < numParticoes; i++) {
        if (particoes[i].arquivo != NULL) {
            fclose(particoes[i].arquivo);
        }
    }
    
    free(particoes);
    
    printf("\nIntercalacao concluida com sucesso!\n");
    printf("Total de alunos intercalados: %d\n", totalAlunos);
    printf("Numero de comparacoes: %d\n", comparacoes);
    printf("Tempo de execucao: %.2f ms\n", tempoGasto * 1000);
    printf("Dados gravados no arquivo fornecido.\n");
    
    // Log detalhado do resultado
    snprintf(logMsg, sizeof(logMsg), "Alunos intercalados: %d", totalAlunos);
    registrarLog(logMsg);
    snprintf(logMsg, sizeof(logMsg), "Comparacoes realizadas: %d", comparacoes);
    registrarLog(logMsg);
    snprintf(logMsg, sizeof(logMsg), "Tempo intercalacao: %.2f ms", tempoGasto * 1000);
    registrarLog(logMsg);
    snprintf(logMsg, sizeof(logMsg), "Eficiencia: %.2f comparacoes/aluno", (double)comparacoes / totalAlunos);
    registrarLog(logMsg);
    snprintf(logMsg, sizeof(logMsg), "Taxa processamento: %.0f alunos/ms", totalAlunos / (tempoGasto * 1000));
    registrarLog(logMsg);
    snprintf(logMsg, sizeof(logMsg), "=== FIM INTERCALACAO OTIMA ===");
    registrarLog(logMsg);
    
    // Log resumo geral
    snprintf(logMsg, sizeof(logMsg), "*** RESUMO GERAL: %d particoes -> %d alunos em %.2f ms ***", 
             numParticoes, totalAlunos, tempoGasto * 1000);
    registrarLog(logMsg);
}

// Função para verificar se o arquivo final está ordenado
void verificarArquivoOrdenado() {
    FILE *arquivo = fopen("alunos_ordenados.dat", "rb");
    if (arquivo == NULL) {
        printf("Arquivo alunos_ordenados.dat nao encontrado.\n");
        printf("Execute primeiro a intercalacao otima.\n");
        return;
    }
    
    printf("\nVerificando arquivo final ordenado:\n");
    printf("====================================\n");
    
    Aluno aluno;
    int contador = 0;
    int ultimaMatricula = -1;
    int ordenado = 1;
    
    printf("Primeiros 10 alunos do arquivo ordenado:\n");
    
    while (fread(&aluno, sizeof(Aluno), 1, arquivo)) {
        if (contador < 10) {
            printf("  %d. %s (Matricula: %d)\n", contador + 1, aluno.nome, aluno.matricula);
        }
        
        // Verifica se está ordenado
        if (ultimaMatricula != -1 && aluno.matricula < ultimaMatricula) {
            ordenado = 0;
        }
        ultimaMatricula = aluno.matricula;
        contador++;
    }
    
    printf("\nEstatisticas do arquivo final:\n");
    printf("Total de alunos: %d\n", contador);
    printf("Arquivo ordenado: %s\n", ordenado ? "SIM" : "NAO");
    
    if (contador > 10) {
        // Mostrar os últimos alunos
        long offset = -5L * (long)sizeof(Aluno);
        fseek(arquivo, offset, SEEK_END);
        printf("\nUltimos 5 alunos do arquivo:\n");
        int inicio = contador - 5;
        for (int i = 0; i < 5 && fread(&aluno, sizeof(Aluno), 1, arquivo); i++) {
            printf("  %d. %s (Matricula: %d)\n", inicio + i + 1, aluno.nome, aluno.matricula);
        }
    }
    
    fclose(arquivo);
}

// Função para gerar relatório detalhado das operações
void gerarRelatorioOperacoes() {
    FILE *logFile = fopen("buscas.log", "r");
    if (logFile == NULL) {
        printf("Arquivo de log nao encontrado.\n");
        return;
    }
    
    printf("\n=== RELATORIO DETALHADO DAS OPERACOES ===\n");
    printf("==========================================\n");
    
    char linha[512];
    int contadorLinhas = 0;
    
    while (fgets(linha, sizeof(linha), logFile)) {
        contadorLinhas++;
        printf("%02d. %s", contadorLinhas, linha);
    }
    
    fclose(logFile);
    
    printf("\nTotal de operacoes registradas: %d\n", contadorLinhas);
    printf("==========================================\n");
}

// Função para limpar o arquivo de log
void limparLog() {
    FILE *logFile = fopen("buscas.log", "w");
    if (logFile != NULL) {
        fclose(logFile);
        printf("Arquivo de log limpo com sucesso.\n");
    } else {
        printf("Erro ao limpar o arquivo de log.\n");
    }
}