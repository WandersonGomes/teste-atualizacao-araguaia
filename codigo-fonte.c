// --- AUTOR ---
/*
Nome: Wanderson Gomes da Costa
E-mail: dersom100@gmail.com
*/
// --- BIBLIOTECAS NECESSARIAS ---
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// --- PROTOTIPO DAS FUNCOES ---
char* criarString(int tamanho);
void destruirString(char* string);

void fecharProcessos();
void fazerDownloadExecutaveis();
int copiarExecutaveisParaDiretorios();

// --- PROGRAMA PRINCIPAL ---
int main() {
    fecharProcessos();
    fazerDownloadExecutaveis();
    copiarExecutaveisParaDiretorios();
    return 0;
}

// --- IMPLEMENTACAO DAS FUNCOES ---
char* criarString(int tamanho) {
    char* string = (char*) malloc(sizeof(char) * (tamanho + 1));
    return string;
}

void destruirString(char* string) {
    free(string);
}

void fecharProcessos() {
    puts("\n--- FECHANDO PROCESSOS ---");
    char* programa = "taskkill /F /IM";
    char nome_processo[50];
    FILE* arquivo = fopen("processos.cfg", "r");

    if (arquivo != NULL) {
        while (fscanf(arquivo, "%s", nome_processo) != EOF) {
            char* comando = criarString(strlen(programa) + strlen(nome_processo) + 1); //+1 espaco em branco
            sprintf(comando, "%s %s", programa, nome_processo);
            system(comando);
            destruirString(comando);
        }

        fclose(arquivo);
    } else {
        puts("ERROR: Nao foi possivel abrir o arquivo processos.cfg");
    }
}

void fazerDownloadExecutaveis() {
    puts("\n--- FAZENDO DOWNLOAD DOS EXECUTAVEIS ---");
    FILE* arquivo_executaveis = fopen("executaveis.cfg", "r");
    FILE* arquivo_repositorio = fopen("repositorio.cfg", "r");
    char* programa = "curl -o";
    char executavel[50];
    char repositorio[200];

    if (arquivo_repositorio != NULL) {
        fscanf(arquivo_repositorio, "%s", repositorio);

        if (arquivo_executaveis != NULL) {
            while (fscanf(arquivo_executaveis, "%s", executavel) != EOF) {
                char* comando = criarString(strlen(programa) + 2*strlen(executavel) + strlen(repositorio) + 2); // +2 espaco em branco
                sprintf(comando, "%s %s %s%s\n", programa, executavel, repositorio, executavel);
                system(comando);
                destruirString(comando);
            }

            fclose(arquivo_executaveis);
        } else {
            puts("ERROR: Nao foi possivel abrir o arquivo executaveis.cfg");
        }

        fclose(arquivo_repositorio);
    } else {
        puts("ERROR: Nao foi possivel abrir o arquivo repositorio.cfg");
    }
}

void prepararString(char* string) {
    if (string[strlen(string) - 1] == '\n')
        string[strlen(string) - 1] = '\0';
}

int copiarExecutaveisParaDiretorios() {
    puts("\n--- FAZENDO COPIA PARA OS DIRETORIOS ---");
    // define o comando de copia
    char* programa = "copy";

    // abre os arquivos
    FILE* arquivo_diretorios = fopen("diretorios.cfg", "r");
    if (arquivo_diretorios == NULL) {
        puts("ERROR: Nao foi possivel abrir o arquivo diretorios.cfg");
        return EXIT_FAILURE;
    }

    FILE* arquivo_executaveis = fopen("executaveis.cfg", "r");
    if (arquivo_executaveis == NULL) {
        puts("ERROR: Nao foi possivel abrir o arquivo executaveis.cfg");
        fclose(arquivo_diretorios);
        return EXIT_FAILURE;
    }

    // realiza o comando para cada executavel e diretorio 
    char* diretorio = NULL;
    unsigned int len = 0;
    while (getline(&diretorio, &len, arquivo_diretorios) != -1) {
        prepararString(diretorio);
        printf("Diretorio: %s\n", diretorio);
        
        char* executavel = NULL;
        len = 0;
        while (getline(&executavel, &len, arquivo_executaveis) != -1) {
            prepararString(executavel);
            printf("\tExecutavel: %s\n", executavel);

            char* comando = criarString(strlen(programa) + strlen(executavel) + strlen(diretorio) + 3); //2 espacos e o \n
            sprintf(comando, "%s %s \"%s\\\"", programa, executavel, diretorio);
            printf("\tComando: %s\n", comando);
            system(comando);
        }

        fseek(arquivo_executaveis, 0, SEEK_SET);
    }

    fclose(arquivo_executaveis);
    fclose(arquivo_diretorios);
}