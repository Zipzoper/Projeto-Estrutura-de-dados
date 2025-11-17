#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAMANHO_TABELA 101
#define MAX_NOME 50
#define MAX_HASH 65 // 64 chars + '\0'

// ------------------------------------
// Estruturas
// ------------------------------------

typedef struct NoUsuario {
    char nome_usuario[MAX_NOME];
    char hash_senha[MAX_HASH];
    struct NoUsuario *prox;
} NoUsuario;

typedef NoUsuario* TabelaHash[TAMANHO_TABELA];

// ------------------------------------
// Funções de Hash
// ------------------------------------

// Hash para strings (djb2)
int funcaoHash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return (hash % TAMANHO_TABELA + TAMANHO_TABELA) % TAMANHO_TABELA;
}

// Hash melhorado para senha (djb2)
unsigned long hashSenha(const char *senha) {
    unsigned long hash = 5381;
    int c;

    while ((c = *senha++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

// converte hash numérico → hexadecimal
void gerarHashSenha(const char *senha, char *saida) {
    unsigned long h = hashSenha(senha);
    sprintf(saida, "%016lX", h);
}

// ------------------------------------
// Funções obrigatórias
// ------------------------------------

void inicializaHash(TabelaHash tab) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        tab[i] = NULL;
    }
}

bool cadastraUsuario(TabelaHash tab, const char *username, const char *password) {
    int indice = funcaoHash(username);
    NoUsuario *atual = tab[indice];

    while (atual != NULL) {
        if (strcmp(atual->nome_usuario, username) == 0) {
            printf("❌ ERRO: Usuário '%s' já existe.\n", username);
            return false;
        }
        atual = atual->prox;
    }

    NoUsuario *novo = (NoUsuario *)malloc(sizeof(NoUsuario));
    if (!novo) {
        perror("ERRO malloc");
        exit(1);
    }

    strncpy(novo->nome_usuario, username, MAX_NOME);
    novo->nome_usuario[MAX_NOME - 1] = '\0';

    gerarHashSenha(password, novo->hash_senha);

    novo->prox = tab[indice];
    tab[indice] = novo;

    printf("✔ Usuário '%s' cadastrado com sucesso! (índice %d)\n", username, indice);
    return true;
}

bool login(TabelaHash tab, const char *username, const char *password) {
    int indice = funcaoHash(username);
    NoUsuario *atual = tab[indice];

    char hash_digitado[MAX_HASH];
    gerarHashSenha(password, hash_digitado);

    while (atual != NULL) {
        if (strcmp(atual->nome_usuario, username) == 0) {
            if (strcmp(atual->hash_senha, hash_digitado) == 0) {
                printf("🔓 Login bem-sucedido!\n");
                return true;
            } else {
                printf("❌ Senha incorreta.\n");
                return false;
            }
        }
        atual = atual->prox;
    }

    printf("❌ Usuário não encontrado.\n");
    return false;
}

bool removeUsuario(TabelaHash tab, const char *username) {
    int indice = funcaoHash(username);
    NoUsuario *atual = tab[indice];
    NoUsuario *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->nome_usuario, username) == 0) {
            if (anterior == NULL)
                tab[indice] = atual->prox;
            else
                anterior->prox = atual->prox;

            free(atual);
            printf("🗑 Usuário '%s' removido.\n", username);
            return true;
        }

        anterior = atual;
        atual = atual->prox;
    }

    printf("❌ Usuário não encontrado.\n");
    return false;
}

void imprimeTabela(TabelaHash tab) {
    printf("\n===== TABELA HASH =====\n");
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        if (tab[i] != NULL) {
            printf("[%d] -> ", i);
            NoUsuario *aux = tab[i];
            while (aux != NULL) {
                printf("(%s | %s) -> ", aux->nome_usuario, aux->hash_senha);
                aux = aux->prox;
            }
            printf("NULL\n");
        }
    }
}

void liberaHash(TabelaHash tab) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        NoUsuario *atual = tab[i];
        while (atual != NULL) {
            NoUsuario *prox = atual->prox;
            free(atual);
            atual = prox;
        }
        tab[i] = NULL;
    }
}

// ------------------------------------
// MENU
// ------------------------------------

int main() {
    TabelaHash usuarios;
    inicializaHash(usuarios);

    int opc;
    char user[MAX_NOME], senha[50];

    do {
        printf("\n====== MENU ======\n");
        printf("1 - Cadastrar usuário\n");
        printf("2 - Login\n");
        printf("3 - Remover usuário\n");
        printf("4 - Imprimir tabela\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1:
                printf("Username: ");
                scanf("%s", user);
                printf("Senha: ");
                scanf("%s", senha);
                cadastraUsuario(usuarios, user, senha);
                break;

            case 2:
                printf("Username: ");
                scanf("%s", user);
                printf("Senha: ");
                scanf("%s", senha);
                login(usuarios, user, senha);
                break;

            case 3:
                printf("Username a remover: ");
                scanf("%s", user);
                removeUsuario(usuarios, user);
                break;

            case 4:
                imprimeTabela(usuarios);
                break;

            case 0:
                liberaHash(usuarios);
                printf("Encerrando...\n");
                break;

            default:
                printf("Opção inválida.\n");
        }
    } while (opc != 0);

    return 0;
}
