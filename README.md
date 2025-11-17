# 🔐 Sistema de Autenticação com Tabela Hash (C)

Projeto desenvolvido para a disciplina **Estrutura de Dados**, utilizando:

- Tabela Hash implementada do zero
- Tratamento de colisões por **encadeamento separado**
- Armazenamento seguro usando **hash da senha**
- `malloc` e `free` (alocação dinâmica)
- Função de hash personalizada
- Operações completas: cadastrar, login, remover e imprimir

---

## 🚀 Funcionalidades

✔ **Cadastrar usuário**  
Armazena `username` + `hash da senha` na tabela hash.

✔ **Login**  
Calcula o hash da senha digitada e compara com o hash registrado.

✔ **Remover usuário**  
Remove o nó específico da lista encadeada.

✔ **Imprimir tabela**  
Mostra índices usados, encadeamentos e colisões.

✔ **Liberar memória**  
Libera toda a tabela no fim do programa.

---

## 🧠 Estrutura da Tabela Hash

- Tamanho da tabela: **101** (número primo)
- Hashing para username: **djb2**
- Hashing para senha: **djb2**, convertido para hexadecimal
- Colisões: **encadeamento separado**

---

## 📂 Como compilar

No Linux, macOS ou Windows (MinGW):

```bash
gcc main.c -o sistema
