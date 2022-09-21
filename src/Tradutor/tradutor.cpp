/*
Projeto 2 da disciplina Software Basico
Universidade de Brasilia
Instituto de Ciencias Exatas
Departamento de Ciencia da Computacao
Software Basico 1/2021
Professor: Bruno Macchiavello
Desenvolvido por: Guilherme Silva Souza e Maria Eduarda Machado de Holanda
*/

#include <bits/stdc++.h>

#include "funcoesAuxiliares.h"
#include "tradutor.h"

using namespace std;

int main(int argc, char *argv[]){  
    string caminho_arq1 = argv[1];
    tradutor(caminho_arq1);
    return 0;
}
