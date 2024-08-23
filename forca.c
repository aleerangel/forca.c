#include <stdio.h> 
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include "funcoes.h"

//variaveis globais
char palavrasecreta[TAMANHO_LIMITE];
char chutes[26];
int numerochutes = 0;

int main() {
    escolhepalavrasecreta();
    abertura();

    //enquanto não acertou a palavra e não enforcou
    do {
        desenhaforca();
        chuta();
    } while(!ganhou() && !enforcou()); 

    //desenha a forca uma ultima vez depois de ganhar ou perder
    desenhaforca();

    if(ganhou()) {
        mensagemganhou();
    } else {
        mensagemperdeu();
    }

    adicionarpalavra();
    printf("Fim do jogo.");

    return 0;
}

void escolhepalavrasecreta() {
    //abre o arquivo
    FILE* f;
    f = fopen("palavras.txt", "r");
    if(f == 0) {
        printf("Desculpe, banco de dados nao disponivel\n\n");
        exit(1);
    }

    //ve a quantidade de palavras
    int qtddepalavras;
    fscanf(f, "%d", &qtddepalavras);
    
    //gera um numero aleatorio entre o numero de palavras
    srand(time(0));
    int randomico = rand() % qtddepalavras;

    //captura a palavra na posicao randomizada
    for(int i = 0; i <= randomico; i++) {
        fscanf(f, "%s", palavrasecreta);
    }
    
    fclose(f);
}

void abertura() {
    //imprime o cabeçalho
    printf("/****************/\n");
    printf("/ Jogo de Forca */\n");
    printf("/****************/\n\n");
}

void desenhaforca() {
    //desenha a forca de acordo com os erros
    int erros = chuteserrados();

    printf("  _______       \n");
    printf(" |/      |      \n");
    printf(" |      %c%c%c  \n", (erros >= 1 ? '(' : ' '), (erros >=1 ? '_' : ' '), (erros >= 1 ? ')' : ' '));
    printf(" |      %c%c%c  \n", (erros >= 3 ? '/' : ' '), (erros >= 2 ? '|' : ' '), (erros >= 4 ? '\\' : ' '));
    printf(" |       %c     \n", (erros >= 2 ? '|' : ' '));
    printf(" |      %c %c   \n", (erros >= 5 ? '/' : ' '), (erros >= 6 ? '\\' : ' '));
    printf(" |              \n");
    printf("_|___           \n");
    printf("\n\n");

    //a cada letra, se o chute for correto, insere a letra. se não, insere um traço
    for(int i = 0; i <strlen(palavrasecreta); i++) {
        int achou = verificarchute(palavrasecreta[i]);
        
        if(achou) {
            printf("%c ", palavrasecreta[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

void chuta() {
    //armazena os chutes no array
    char chute;
    scanf(" %c", &chute);
    chute = toupper(chute);
    chutes[numerochutes] = chute;
    numerochutes++;
}

int verificarchute(char letra) {
    //percorre a palavra letra por letra e compara cada letra com todos os chutes
    //repete a cada chute

    for (int j = 0; j < numerochutes; j++) {
    if(chutes[j] == letra) {
    return 1;
    }
    }
    return 0;
}

int ganhou() {
    //percorre a palavra letra por letra
    //enquanto alguma letra nao tiver sido chutada, não ganhou
    for(int i = 0; i < strlen(palavrasecreta); i++) {
        if(!verificarchute(palavrasecreta[i])) {
            return 0;
        }
    }
    return 1;
}

int enforcou() {
    //retorna se o numero de chutes foi acima do limite (forca)
    return chuteserrados() >= 6;
}

int chuteserrados() {
    //percorre a palavra letra por letra e compara com o chute
    int erros = 0;
    for(int i = 0; i < numerochutes; i++) {
        int acertou = 0;

        for (int j = 0; j < strlen(palavrasecreta); j++) {
            if(chutes[i] == palavrasecreta[j]) {
                acertou = 1;
                break;
            }
        }
        if(!acertou) {
            erros++;
        }
    }
    return erros;
}

void adicionarpalavra() {
    //adiciona uma palavra ao banco de dados
    char adicionar;

    printf("Você deseja adicionar uma nova palavra ao jogo? (S/N)\n");
    scanf(" %c", &adicionar);
    adicionar = toupper(adicionar);

    if(adicionar == 'S') {
        char novapalavra[TAMANHO_LIMITE];

        printf("Qual a nova palavra?\n");
        scanf("%s", novapalavra);

        //converte a palavra para maiusculas 
        for (int i = 0; novapalavra[i]; i++) {
            novapalavra[i] = toupper(novapalavra[i]);
        }

        //adiciona a palavra ao arquivo de palavras
        FILE* f;
        f = fopen("palavras.txt", "r+");
        if(f == 0) {
        printf("Desculpe, banco de dados nao disponivel\n\n");
        exit(1);
        }

        int qtd;
        fscanf(f, "%d", &qtd);
        qtd++;

        fseek(f, 0, SEEK_SET);
        fprintf(f, "%d", qtd);

        fseek(f, 0, SEEK_END);
        fprintf(f, "\n%s", novapalavra);

        fclose(f);

        printf("Palavra adicionada com sucesso!\n");
    }
}

void mensagemganhou() {
    printf("Parabéns! Você ganhou!\n\n");

    printf("       ___________      \n");
    printf("      '._==_==_=_.'     \n");
    printf("      .-\\:      /-.    \n");
    printf("     | (|:.     |) |    \n");
    printf("      '-|:.     |-'     \n");
    printf("        \\::.    /      \n");
    printf("         '::. .'        \n");
    printf("           ) (          \n");
    printf("         _.' '._        \n");
    printf("        '-------'       \n\n");
}

void mensagemperdeu() {
    printf("Poxa, você perdeu!\n");
    printf("A palavra secreta era **%s**\n\n", palavrasecreta);

    printf("    _______________         \n");
    printf("   /               \\       \n"); 
    printf("  /                 \\      \n");
    printf("//                   \\/\\  \n");
    printf("\\|   XXXX     XXXX   | /   \n");
    printf(" |   XXXX     XXXX   |/     \n");
    printf(" |   XXX       XXX   |      \n");
    printf(" |                   |      \n");
    printf(" \\__      XXX      __/     \n");
    printf("   |\\     XXX     /|       \n");
    printf("   | |           | |        \n");
    printf("   | I I I I I I I |        \n");
    printf("   |  I I I I I I  |        \n");
    printf("   \\_             _/       \n");
    printf("     \\_         _/         \n");
    printf("       \\_______/           \n");
}  