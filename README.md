# Projeto 2 de Software Básico 2022.1

Projeto 2 da disciplina de Software Básico da UnB em 2022.1. \
Universidade de Brasília, Instituto de Ciências Exatas, Departamento de Ciência da Computação. \
Desenvolvido por: [Guilherme Silva Souza](https://github.com/gss214) e [Maria Eduarda Machado de Holanda](https://github.com/dudaholandah). \
Linguagem utilizada: C++ e Assembly IA32 (x86-32).

Descrição
----------

O trabalho consiste em implementar em C/C++, com auxílio de funções em IA-32, um método de tradução de um código objeto em linguagem hipotética vista em aula para linguagem IA-32. Para mais detalhes, leia a [Especificação](Especificação.pdf).

Especificação de compilação
--------------------------

Utilizamos Linux como Sistema Operacional. Para compilar o projeto, siga os seguintes passos:

<details>
  <summary>Compilar o tradutor</summary>
  
  - Para isso, utilizamos a versão 17 do C++.
  - `g++ -std=c++17 -Wall -o tradutor tradutor.cpp`
  - Certifique-se de estar na pasta do [Tradutor](src/Tradutor).
  
</details> 

<details>
  <summary>Traduzir o código objeto</summary>
  
  - Alguns arquivos objetos se encontram na pasta [Arquivos de Testes](/Arquivos%20de%20Testes).
  - Certifique-se de estar na mesma pasta do [Tradutor](src/Tradutor).
  - `./tradutor ../../Arquivos\ de\ Testes/bin.o`.
  
</details> 

<details>
  <summary>Compilar o código traduzido em IA-32</summary>
  
  - A saída gerada se encontra no arquivo `output.asm`.
  - Utilizamos no projeto o compilador `nasm`.
  - `nasm -f elf -o output.o output.asm`.
  
</details> 

<details>
  <summary>Ligar o código Assembly</summary>
  
  - `ld -m elf_i386 -o output output.o`
  
</details> 

<details>
  <summary>Executar o código Assembly</summary>
  
  - `./output`
  
</details> 

Existe também a possibilidade de rodar o script para compilar e executar o projeto. \
Basta estar utilizando o subsistema Linux e executar na pasta [Tradutor](src/Tradutor) o comando:

```
sh run.sh arquivo.o
```

Uma observação é que o arquivo de teste `arquivo.o` deve estar na pasta [Arquivos de Testes](/Arquivos%20de%20Testes).


