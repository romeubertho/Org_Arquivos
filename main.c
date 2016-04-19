/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Romeu
 *
 * Created on 8 de Março de 2016, 14:43
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _gravarDados 1
#define _recuperarDados 2
#define _recuperarRRN 3
#define _sim 4
#define _nao 5

/*
 * 
 */

struct Produto //estrutura para o registro
{
    char Nome[46];
    char Marca[46];
    int EAN13;
    float Valor;
};

int identificaComando(char c[]) {
    int esc;
    if (strncmp(c, "GD", 2) == 0) {
        esc = _gravarDados;
    } else if (strncmp(c, "RD", 2) == 0) {
        esc = _recuperarDados;
    } else if (strncmp(c, "RRN", 4) == 0) {
        esc = _recuperarRRN;
    } else if (strncmp(c, "sim", 3) == 0) {
        esc = _sim;
    } else if (strncmp(c, "nao", 3) == 0) {
        esc = _nao;
    } else {
        esc = 0;
    }
    return esc;
}

void imprimeComandos() {
    printf("---------- OPÇÕES ----------");
    printf("\n");
    printf("GD - Gravar dados\n");
    printf("RD - Recuperar dados\n");
    printf("RRN - Recuperar dados pelo RRN\n");
}

void read() {
    struct Produto registro;
    //----Abre/cria o arquivo
    FILE * fp;
    int counter = 0;
    fp = fopen("produtos.bin", "rb");
    if (!fp) {
        printf("Erro ao abrir o arquivo");
        return;
    }
    printf("\033c");
    printf("---------- DADOS ----------");
    printf("\n");
    while (fp) {
        fseek(fp, sizeof (struct Produto)*counter, SEEK_SET);
        int hRead = fread(&registro, sizeof (struct Produto), 1, fp);
        if (!hRead)
            break;
        printf("[%d] %s %s %d %0.2f\n", counter, registro.Nome, registro.Marca, registro.EAN13, registro.Valor);
        printf("\n");
        counter++;
    }
    fclose(fp);
}

void readRRN() {
    struct Produto registro;
    int RRN;
    char input[20];
    //----Abre/cria o arquivo
    FILE * fp;
    fp = fopen("produtos.bin", "rb");
    if (!fp) {
        printf("Erro ao abrir o arquivo");
        return;
    }
    printf("\033c");
    do
    {
    printf("Entre com o VALOR de RRN: \n");
    scanf("%d", &RRN);
    printf("\n");
    printf("\033c");
    printf("---------- DADOS RRN [%d] ----------", RRN);
    printf("\n");
    fseek(fp, sizeof (struct Produto)*RRN, SEEK_SET);
    int hRead = fread(&registro, sizeof (struct Produto), 1, fp);
    if (!hRead) {
        printf("RRN [%d] não existe!\n", RRN);
        return;
    }
    printf("[%d] %s %s %d %0.2f\n", RRN, registro.Nome, registro.Marca, registro.EAN13, registro.Valor);
    printf("\n");
    printf("Recuperar dados pelo RRN novamente? sim/nao: \n");
        scanf("%4s", input);
        printf("\033c");
    } while (identificaComando(input) == _sim);
    fclose(fp);
}

void write() {
    struct Produto registro;
    //----Abre/cria o arquivo
    FILE * fp;
    int counter;
    char input[5];
    fp = fopen("produtos.bin", "ab+");
    if (!fp) {
        printf("Erro ao abrir o arquivo");
        return;
    }
    printf("\033c");
    do // loop do registro
    {
        //----Usuırio entra com o registro das pessoas
        printf("Entre com o NOME: \n");
        scanf("%99s", registro.Nome);
        printf("\n");
        printf("Entre com a MARCA: \n");
        scanf("%99s", registro.Marca);
        printf("\n");
        printf("Entre com o EAN13: \n");
        scanf("%d", &registro.EAN13);
        printf("\n");
        printf("Entre com o VALOR: \n");
        scanf("%f", &registro.Valor);
        printf("\n");
        //----- Faz a escrita no fim do arquivo
        int hWrite = fwrite(&registro, sizeof (struct Produto), 1, fp);
        if (hWrite) {
            printf("Registro adicionado com sucesso! %s %s %d %0.2f\n", registro.Nome, registro.Marca, registro.EAN13, registro.Valor);
        } else printf("Erro ao adicionar no registro!\n");
        //---Opção para continuar com a entrada de novos registros
        printf("Adicionar novos registros? sim/nao: \n");
        scanf("%4s", input);
        printf("\033c");
    } while (identificaComando(input) == _sim); // Se "s" usuırio entra com um novo registro, caso contrırio, ı impresso os registros no terminal
    fclose(fp); // fecha o arquivo
}

int main(int argc, char** argv) {
    char input[20];
    do {
        imprimeComandos();
        scanf("%19s", input);
        int comando = identificaComando(input);
        switch (comando) {
            case _gravarDados:
                write();
                break;
            case _recuperarDados:
                read();
                break;
            case _recuperarRRN:
                readRRN();
                break;
            default:
                return 0;
        }
    } while (identificaComando(input) != 0);
    return 0;
}