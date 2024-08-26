#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 256

typedef struct File {
    char nome[MAX_NOME];
    struct File *next;
} File;

typedef struct Directory {
    char nome[MAX_NOME];
    File *files;
    struct Directory *subdirs;
    struct Directory *next;
    struct Directory *parent;
} Directory;

Directory* criar_diretorio(const char *nome, Directory *parent) {
    Directory *dir = (Directory *)malloc(sizeof(Directory));
    strncpy(dir->nome, nome, sizeof(dir->nome));
    dir->files = NULL;
    dir->subdirs = NULL;
    dir->next = NULL;
    dir->parent = parent;
    return dir;
}

File* criar_arquivo(const char *nome) {
    File *file = (File *)malloc(sizeof(File));
    strncpy(file->nome, nome, sizeof(file->nome));
    file->next = NULL;
    return file;
}

void adicionar_subdiretorio(Directory *parent, Directory *subdir) {
    subdir->next = parent->subdirs;
    parent->subdirs = subdir;
}

void adicionar_arquivo(Directory *dir, File *file) {
    file->next = dir->files;
    dir->files = file;
}

void listar_conteudo(Directory *dir) {

    File *file = dir->files;
    while (file != NULL) {
        printf("%s\n", file->nome);
        file = file->next;
    }

    Directory *subdir = dir->subdirs;
    while (subdir != NULL) {
        printf("%s\n", subdir->nome);
        subdir = subdir->next;
    }
}

Directory* encontrar_diretorio(Directory *root, const char *nome) {
    Directory *dir = root->subdirs;
    while (dir != NULL) {
        if (strcmp(dir->nome, nome) == 0) {
            return dir;
        }
        dir = dir->next;
    }
    return NULL;
}

File* encontrar_arquivo(Directory *dir, const char *nome) {
    File *file = dir->files;
    while (file != NULL) {
        if (strcmp(file->nome, nome) == 0) {
            return file;
        }
        file = file->next;
    }
    return NULL;
}

void excluir_diretorio(Directory *parent, const char *nome) {
    Directory *prev = NULL;
    Directory *dir = parent->subdirs;
    while (dir != NULL) {
        if (strcmp(dir->nome, nome) == 0) {
            if (prev == NULL) {
                parent->subdirs = dir->next;
            } else {
                prev->next = dir->next;
            }
            liberar_diretorios(dir->subdirs);
            liberar_arquivos(dir->files);
            free(dir);
            printf("Diretorio '%s' excluido com sucesso.\n", nome);
            return;
        }
        prev = dir;
        dir = dir->next;
    }
    printf("Diretorio '%s' nao encontrado.\n", nome);
}

void excluir_arquivo(Directory *dir, const char *nome) {
    File *prev = NULL;
    File *file = dir->files;
    while (file != NULL) {
        if (strcmp(file->nome, nome) == 0) {
            if (prev == NULL) {
                dir->files = file->next;
            } else {
                prev->next = file->next;
            }
            free(file);
            printf("Arquivo '%s' excluido com sucesso.\n", nome);
            return;
        }
        prev = file;
        file = file->next;
    }
    printf("Arquivo '%s' nao encontrado.\n", nome);
}

void liberar_arquivos(File *file) {
    while (file != NULL) {
        File *temp = file;
        file = file->next;
        free(temp);
    }
}

void liberar_diretorios(Directory *dir) {
    while (dir != NULL) {
        liberar_arquivos(dir->files);
        liberar_diretorios(dir->subdirs);
        Directory *temp = dir;
        dir = dir->next;
        free(temp);
    }
}

void executar_comando(char *comando, Directory **current_dir) {
    char *token = strtok(comando, " ");
    if (token == NULL) return;

    if (strcmp(token, "ls") == 0) {
        listar_conteudo(*current_dir);
    } else if (strcmp(token, "cd") == 0) {
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("Uso: cd <diretorio>\n");
            return;
        }
        Directory *dir = encontrar_diretorio(*current_dir, token);
        if (dir != NULL) {
            *current_dir = dir;
        } else {
            printf("Diretorio '%s' nao encontrado.\n", token);
        }
    } else if (strcmp(token, "cd..") == 0) {
        if ((*current_dir)->parent != NULL) {
            *current_dir = (*current_dir)->parent;

            //Implementar pilha com diretorio
        } else {
            printf("Você ja esta no diretorio raiz.\n");
        }
    } else if (strcmp(token, "mkdir") == 0) {
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("Uso: mkdir <nome_da_pasta>\n");
            return;
        }
        if (encontrar_diretorio(*current_dir, token) != NULL) {
            printf("Diretorio '%s' ja existe.\n", token);
        } else {
            Directory *new_dir = criar_diretorio(token, *current_dir);
            adicionar_subdiretorio(*current_dir, new_dir);
            printf("Diretorio '%s' criado com sucesso.\n", token);
        }
    } else if (strcmp(token, "touch") == 0) {
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("Uso: touch <nome_do_arquivo>\n");
            return;
        }
        if (encontrar_arquivo(*current_dir, token) != NULL) {
            printf("Arquivo '%s' ja existe.\n", token);
        } else {
            File *new_file = criar_arquivo(token);
            adicionar_arquivo(*current_dir, new_file);
            printf("Arquivo '%s' criado com sucesso.\n", token);
        }
    } else if (strcmp(token, "rm") == 0) {
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("Uso: rm <nome_do_arquivo>\n");
            return;
        }
        excluir_arquivo(*current_dir, token);
    } else if (strcmp(token, "rmdir") == 0) {
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("Uso: rmdir <nome_da_pasta>\n");
            return;
        }
        excluir_diretorio(*current_dir, token);
    } else if (strcmp(token, "exit") == 0) {
        // Exit command to exit the shell loop
        exit(0);
    } else {
        printf("Comando desconhecido: %s\n", token);
    }
}

int main() {
    Directory *root = criar_diretorio("", NULL);
    Directory *home = criar_diretorio("home", root);
    Directory *docs = criar_diretorio("docs", home);
    Directory *music = criar_diretorio("music", home);

    adicionar_subdiretorio(root, home);
    adicionar_subdiretorio(home, docs);
    adicionar_subdiretorio(home, music);

    File *file1 = criar_arquivo("file1.txt");
    File *file2 = criar_arquivo("file2.txt");
    adicionar_arquivo(docs, file1);
    adicionar_arquivo(music, file2);

    Directory *current_dir = root;
    char comando[512];

    while (1) {
        printf("-%s-> ", current_dir->nome);
        if (fgets(comando, sizeof(comando), stdin) == NULL) {
            printf("Erro ao ler o comando.\n");
            continue;
        }
        comando[strcspn(comando, "\n")] = '\0'; // Remove o caractere de nova linha
        executar_comando(comando, &current_dir);
    }

    liberar_diretorios(root);
    return 0;
}
