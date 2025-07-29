# Função de Particionamento com Seleção Natural

## Descrição
A função `gerarParticoesSelecaoNatural()` implementa o algoritmo de seleção natural para criar partições ordenadas de alunos em arquivos separados na pasta "particoes/".

## Como Funciona

### 1. Algoritmo de Seleção Natural
- Lê blocos de dados do arquivo principal (alunos.dat) baseado no tamanho da memória especificado
- Ordena cada bloco internamente usando bubble sort
- Aplica seleção natural para criar sequências ordenadas maiores que o tamanho da memória
- Salva cada partição em um arquivo separado na pasta "particoes/"

### 2. Processo de Particionamento
1. **Leitura**: Lê um bloco de alunos do arquivo principal
2. **Ordenação**: Ordena o bloco por matrícula usando bubble sort
3. **Seleção Natural**: Seleciona o próximo aluno válido na sequência ordenada
4. **Gravação**: Escreve a partição ordenada em um arquivo separado
5. **Repetição**: Repete o processo até processar todos os alunos

### 3. Vantagens da Seleção Natural
- Cria partições maiores que o tamanho da memória disponível
- Mantém sequências ordenadas mesmo com dados embaralhados
- Otimiza o processo de ordenação externa
- Reduz o número de partições necessárias

## Como Usar

### 1. Criar Base de Dados
```
Opção 19: Criar Base de Dados
- Digite a quantidade de alunos (ex: 100)
```

### 2. Gerar Partições
```
Opção 20: Gerar Partições com Seleção Natural
- Digite o tamanho da memória (ex: 10 alunos por bloco)
```

### 3. Verificar Partições
```
Opção 21: Verificar Conteúdo das Partições
- Mostra o conteúdo de cada partição criada
- Verifica se cada partição está ordenada
```

## Estrutura de Arquivos

```
projeto/
├── alunos.dat              # Arquivo principal de alunos
├── particoes/              # Pasta das partições
│   ├── particao_0.dat     # Primeira partição
│   ├── particao_1.dat     # Segunda partição
│   └── ...                # Demais partições
└── programa.exe           # Executável
```

## Exemplo de Uso

1. **Criar base com 50 alunos**:
   - Menu → 19 → Digite: 50

2. **Gerar partições com memória de 5 alunos**:
   - Menu → 20 → Digite: 5
   - Resultado: 10 partições com 5 alunos cada

3. **Verificar conteúdo**:
   - Menu → 21
   - Mostra todas as partições e verifica se estão ordenadas

## Características da Implementação

- **Memória Eficiente**: Usa apenas a quantidade especificada de memória
- **Partições Ordenadas**: Cada partição é garantidamente ordenada por matrícula
- **Arquivos Separados**: Cada partição é salva em arquivo individual
- **Verificação**: Função para verificar se as partições estão corretas
- **Logging**: Mostra progresso e estatísticas do processo

## Complexidade
- **Tempo**: O(n log k) onde n é o número total de alunos e k é o tamanho da memória
- **Espaço**: O(k) onde k é o tamanho da memória especificado
