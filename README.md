# Sistema de Geração de IDs Únicos

## 📝 Descrição

Implementação de três algoritmos para geração de identidades únicas utilizando diferentes estruturas de dados: **Fila (Queue)**, **Pilha (Stack)** e **Lista Encadeada (Linked List)**. 

## 👥 Equipe de Desenvolvimento
- Francisco André
- Francisco Bernardo
- Erik Renan
- Adonay Davyd
- João Lucas

Cada versão garante:
- ✅ IDs não sequenciais
- ✅ Sem repetições
- ✅ Comunicação via sockets TCP

## ⚙️ Estruturas Implementadas

### 1. Fila (Queue)

**Lógica de geração:**
```c
(contador_base * 31 + 17) % 1000000 + (contador_base % 7) * 100000
```

- **Armazenamento**: FIFO (First-In-First-Out)
- **Complexidade**:
  - Inserção: O(1)
  - Verificação de duplicatas: O(n)

### 2. Pilha (Stack)

**Lógica de geração:**
```c
((contador_base * 13 + 29) % 900000) + 100000
```

- **Armazenamento**: LIFO (Last-In-First-Out)
- **Complexidade**:
  - Inserção: O(1)
  - Verificação de duplicatas: O(n)

### 3. Lista Encadeada (Linked List)

**Lógica de geração:**
```c
(contador_base * 23 + 41) % 800000 + 200000  // Para pares
(contador_base * 19 + 37) % 600000 + 400000  // Para ímpares
```

- **Armazenamento**: Inserção no início
- **Complexidade**:
  - Inserção: O(1)
  - Verificação de duplicatas: O(n)

## 🚀 Como Executar

### Compilação

```bash
# Servidor (substitua <estrutura> por fila, pilha ou lista)
gcc server_<estrutura>.c -o server_<estrutura>

# Cliente
gcc client.c -o client
```

### Uso

1. **Inicie o servidor:**
   ```bash
   ./server_<estrutura>
   ```

2. **Execute o cliente:**
   ```bash
   ./client
   ```
   - O cliente solicitará quantos IDs deseja gerar

## 📊 Métricas de Desempenho

*(Exemplo teórico - execute localmente para dados reais)*

| Estrutura      | 10.000 IDs | 1.000.000 IDs |
|---------------|------------|---------------|
| **Fila**      | ~0.5s      | ~50s          |
| **Pilha**     | ~0.5s      | ~50s          |
| **Lista**     | ~0.3s      | ~40s          |

## 🔧 Melhorias Futuras

- Adicionar tabela hash para verificação O(1) de duplicatas
- Usar algoritmos de embaralhamento mais robustos (ex: Fisher-Yates)
- Implementar persistência de dados
- Adicionar logs detalhados de performance
- Suporte para múltiplos clientes simultâneos


## 🛠️ Requisitos do Sistema

- **Compilador**: GCC ou equivalente
- **Sistema Operacional**: Linux/Unix/macOS
- **Bibliotecas**: Standard C Library, Socket API

## 📖 Exemplo de Uso

```bash
# Terminal 1: Iniciar servidor com fila
./server_fila

# Terminal 2: Executar cliente
./client
> Quantos IDs deseja gerar? 100
> IDs gerados: 234567, 891234, 456789, ...
```

## 🐛 Troubleshooting

### Erro: "Address already in use"
```bash
# Aguarde alguns segundos ou use:
sudo netstat -tlnp | grep :8080
kill -9 <PID>
```

### Problema de compilação
```bash
# Certifique-se de ter as bibliotecas necessárias
sudo apt-get install build-essential  # Ubuntu/Debian
```

## 📜 Licença

MIT License - Disponível para uso e modificação.

---

**Desenvolvido como projeto educacional para demonstrar diferentes estruturas de dados em C com comunicação TCP/IP.**
