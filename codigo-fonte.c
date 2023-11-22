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

void mostrarMensagem(LPCSTR mensagem);

void fecharProcessos();
void fazerDownloadExecutaveis();
void copiarExecutaveisParaDiretorios();

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

void copiarExecutaveisParaDiretorios() {
    FILE* arquivo_diretorios = fopen("diretorios.cfg", "r");
    FILE* arquivo_executaveis = fopen("executaveis.cfg", "r");

    char* programa = "copy";
    char diretorio[100];
    char executavel[50];

    if (arquivo_diretorios != NULL) {
        while (fscanf(arquivo_diretorios, "%s", diretorio) != EOF) {
            if (arquivo_executaveis != NULL) {
                while (fscanf(arquivo_executaveis, "%s", executavel) != EOF) {
                    char* comando = criarString(strlen(programa) + strlen(executavel) + strlen(diretorio) + 2); //espaco em branco
                    sprintf(comando, "%s %s %s", programa, executavel, diretorio);
                    system(comando);
                    destruirString(comando);
                }
                
                fclose(arquivo_executaveis);
            } else {
                puts("ERROR: Nao foi possivel abrir o arquivo executaveis.cfg");
            }
        }

        fclose(arquivo_diretorios);
    } else {
        puts("ERROR: Nao foi possivel abrir o arquivo diretorios.cfg");
    }
}