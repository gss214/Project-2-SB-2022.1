g++ -std=c++17 -Wall -o tradutor tradutor.cpp
echo "Compilando o tradutor...";

echo "Código objeto a ser traduzido": $1;
./tradutor ../../Arquivos\ de\ Testes/$1

echo "Compilando o código traduzido em IA32...";
nasm -f elf -o output.o output.asm

echo "Ligando...";
ld -m elf_i386 -o output output.o

echo "Executando...";
./output
