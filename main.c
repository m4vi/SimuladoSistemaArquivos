/*Código da função copiarStr para tratamento da string do comando:*/
#include<stdio.h>
#include<string.h>

void copiarStr(char dest[], char orig[], int ini, int fim){
    int i=0, tam=strlen(orig);
    while(i+ini<tam && i<fim-ini && orig[i+ini] != '\0' && orig[i+ini] != '\n'){
        dest[i] = orig[i+ini];
        i++;
    }
    dest[i] = '\0';
}

int main(){
    char str[14],cmd[3], par[11];
    do{
        fflush(stdin);
        gets(str);
        copiarStr(cmd, str, 0, 2);
        copiarStr(par, str, 3, 13);
        printf("str: %s\ncmd: %s\npar: %s\n\n",str,cmd,par);
    }while(strcmp(cmd,"ex")!=0);
    return 0;
}