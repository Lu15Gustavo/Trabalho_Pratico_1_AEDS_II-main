# Intercalação Ótima - Sistema de Ordenação Externa

## Descrição

Este sistema implementa uma solução completa de ordenação externa usando **Seleção Natural** para gerar partições ordenadas e **Intercalação Ótima** para combinar essas partições em um arquivo final completamente ordenado.

## Como Usar

### 1. Preparação dos Dados
Primeiro, é necessário ter dados para processar:
```
Opção 19: Criar Base de Dados
```
- Digite a quantidade de registros desejada (ex: 200)
- Isso criará uma base de dados com alunos, professores e disciplinas

### 2. Geração de Partições
```
Opção 20: Gerar Partições com Seleção Natural
```
- Digite o tamanho da memória (número de alunos por partição, ex: 10)
- O sistema criará partições ordenadas na pasta `particoes/`
- Cada partição será salva como `particao_X.dat`

### 3. Verificação das Partições (Opcional)
```
Opção 21: Verificar Conteúdo das Partições
```
- Mostra o conteúdo de cada partição criada
- Verifica se cada partição está ordenada
- Útil para debug e verificação

### 4. Intercalação Ótima
```
Opção 22: Intercalação Ótima
```
- Combina todas as partições em um único arquivo ordenado
- Cria o arquivo `alunos_ordenados.dat`
- Registra estatísticas no arquivo `buscas.log`

### 5. Verificação do Resultado Final
```
Opção 23: Verificar Arquivo Final Ordenado
```
- Mostra os primeiros e últimos alunos do arquivo final
- Verifica se o arquivo está completamente ordenado
- Exibe estatísticas do resultado

### 6. Relatório Detalhado das Operações
```
Opção 24: Gerar Relatório Detalhado
```
- Exibe todas as operações registradas no log
- Mostra métricas de performance completas
- Útil para análise e debugging

### 7. Limpeza do Log
```
Opção 25: Limpar Arquivo de Log
```
- Remove todas as entradas do arquivo de log
- Útil para iniciar uma nova sessão de testes

## Algoritmos Implementados

### Seleção Natural
- **Objetivo**: Criar partições ordenadas a partir de dados não ordenados
- **Vantagem**: Gera partições maiores que o tamanho da memória disponível
- **Funcionamento**: Usa um algoritmo que seleciona elementos em sequência crescente

### Intercalação Ótima
- **Objetivo**: Combinar múltiplas partições ordenadas em um arquivo final ordenado
- **Complexidade**: O(n log k), onde n é o número total de elementos e k é o número de partições
- **Memória**: Usa apenas O(k) memória adicional para controlar as partições

## Estrutura de Arquivos

```
├── alunos.dat              # Arquivo original de alunos
├── alunos_ordenados.dat    # Arquivo final ordenado (resultado)
├── particoes/              # Pasta com as partições
│   ├── particao_0.dat
│   ├── particao_1.dat
│   └── ...
└── buscas.log              # Log das operações
```

## Exemplo de Uso Completo

1. Execute o programa: `.\programa.exe`
2. Escolha opção **19** → Digite `200` (criar base com 200 alunos)
3. Escolha opção **20** → Digite `10` (partições de 10 alunos cada)
4. Escolha opção **22** → Executa a intercalação ótima
5. Escolha opção **23** → Verifica se o resultado está ordenado

## Métricas de Performance Detalhadas

O sistema agora registra métricas completas no arquivo `buscas.log`:

### **Seleção Natural:**
- **Total de alunos processados**
- **Tamanho da memória utilizada**
- **Número de partições criadas**
- **Tempo total de execução**
- **Média de alunos por partição**

### **Intercalação Ótima:**
- **Número de partições intercaladas**
- **Total de alunos intercalados**
- **Comparações realizadas**
- **Tempo de execução**
- **Eficiência (comparações por aluno)**
- **Taxa de processamento (alunos por ms)**

### Exemplo de Log Completo:
```
=== INICIO SELECAO NATURAL ===
Total de alunos a processar: 100
Tamanho da memoria: 10 alunos
Particoes criadas: 10
Tempo total selecao natural: 10.00 ms
Alunos por particao (media): 10.0
=== FIM SELECAO NATURAL ===
=== INICIO INTERCALACAO OTIMA ===
Particoes a intercalar: 10
Alunos intercalados: 100
Comparacoes realizadas: 101
Tempo intercalacao: 1.00 ms
Eficiencia: 1.01 comparacoes/aluno
Taxa processamento: 100 alunos/ms
=== FIM INTERCALACAO OTIMA ===
*** RESUMO GERAL: 10 particoes -> 100 alunos em 1.00 ms ***
```

## Vantagens do Método

1. **Eficiência de Memória**: Processa arquivos maiores que a memória disponível
2. **Escalabilidade**: Funciona com qualquer quantidade de dados
3. **Otimização**: Minimiza o número de comparações necessárias
4. **Robustez**: Trata casos extremos e arquivos corrompidos

## Aplicações Práticas

- Ordenação de grandes bases de dados
- Processamento de logs de sistemas
- Análise de dados científicos
- Sistemas de Business Intelligence
- Preparação de dados para algoritmos de machine learning

## Complexidade

- **Tempo**: O(n log n) onde n é o número total de elementos
- **Espaço**: O(k) onde k é o número de partições
- **I/O**: Minimiza operações de disco através de acesso sequencial
