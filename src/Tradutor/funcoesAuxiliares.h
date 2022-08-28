#include <bits/stdc++.h>
#include <fstream>

using namespace std;

map<string,int> opcodes = {{"ADD", 1}, {"SUB", 2}, {"MULT", 3}, {"DIV", 4}, {"JMP", 5}, {"JMPN", 6}, {"JMPP", 7}, {"JMPZ", 8}, {"COPY", 9}, {"LOAD", 10}, {"STORE", 11}, {"INPUT", 12}, {"OUTPUT", 13}, {"STOP", 14}, {"S_INPUT", 15}, {"S_OUTPUT", 16}};

vector<int> ler_arquivo(string caminho_arq){
    ifstream file(caminho_arq);
    string str;
    vector<int> cod_objeto;
    while (getline(file, str)) {
        istringstream iss(str);
        string token;
        while (getline(iss, token, ' ')){
            token = token.c_str();
            cod_objeto.push_back(stoi(token));
        }
    }
    return cod_objeto;
}

/**
    Retorna o tamanho de uma instrucao.
    @param instrucao uma string correspondente a instrucao.
    @return um inteiro
*/
int tamanhoDaInstrucao(int opcode){
    if (opcode == 9 or opcode == 15 or opcode == 16){
        return 3;
    } else if (opcode == 14) {
        return 1;
    } else {
        return 2;
    }
}

// LEMBRAR DE COLOCAR DIRETIVA DE TAMANHO (WORD,BYTE...)
vector<string> paraIA32(int opcode, int op1, int op2){

    vector<string> res;

    switch (opcode){
        //ADD
        case 1:
        {
            string lbl = "lbl";
            res.push_back("add EAX, [" + lbl + "]");
            break;
        }
        
        //SUB
        case 2:
        {
            string lbl = "lbl";
            res.push_back("sub EAX, [" + lbl + "]");
            break;
        }
        
        //MUL
        case 3:
            res.push_back("OPERACAO IMUL");
            break;
        
        //DIV
        case 4:
            res.push_back("OPERACAO IDIV");
            break;
        
        //JMP
        case 5:
        {
            string lbl = "lbl";
            res.push_back("jmp " + lbl);
            break;
        }
        
        //JMPN
        case 6:
        {
            string lbl = "lbl";
            res.push_back("cmp EAX, 0");
            res.push_back("jl " + lbl);
            break;
        }

        //JMPP
        case 7:
        {
            string lbl = "lbl";
            res.push_back("cmp EAX, 0");
            res.push_back("jg " + lbl);
            break;
        }
        
        //JMPZ
        case 8:
        {
            string lbl = "lbl";
            res.push_back("cmp EAX, 0");
            res.push_back("je " + lbl);
            break;
        }
        
        // COPY
        case 9:
        {
            string lb1 = "lb1";
            string lb2 = "lb2";
            res.push_back("mov EBX, [" + lb1 + "]");
            res.push_back("mov [" + lb2 + "], EBX");
            break;
        }
        
        // LOAD
        case 10:
        {
            string lbl = "lbl";
            res.push_back("mov EAX, [" + lbl + "]");
            break;
        }
        
        //STORE
        case 11:
        {
            string lbl = "lbl";
            res.push_back("mov " + lbl + ", [EAX]");
            break;    
        }
        
        //INPUT
        case 12:
        {
            res.push_back("OPERACAO INPUT");
            break;
        }
        
        //OUTPUT
        case 13:
        {
            res.push_back("OPERACAO OUTPUT");
            break;
        }
        
        //STOP
        case 14:
        {
            res.push_back("mov EAX, 1");
            res.push_back("mov EBX, 0");
            res.push_back("int 80h");
            break;
        } 
        
        //S_INPUT
        case 16:
        {
            res.push_back("OPERACAO S_INPUT");
            break;
        } 
        
        //S_OUTPUT
        case 17:
        {
            res.push_back("OPERACAO S_OUTPUT");
            break;
        } 
    }
    return res;
}

/**
    Verifica se um token eh uma instrucao valida ou nao.
    @param token uma string correspondente ao token a ser verificado.
    @return um inteiro (opcode), se entre 1 e 16 eh uma instucao valida, caso 0 nao.
*/
int verificaOpcode(string token){
    return opcodes[token];
}

/**
    Retorna o numero de operandos de uma instrucao.
    @param instrucao uma string correspondente a instrucao.
    @return um inteiro correspondente ao numero de operandos da instrucao.
*/
int verificaOperando(string instrucao){
    if (instrucao == "COPY"){
        return 2;
    } else if (instrucao == "STOP" || instrucao == "SPACE"){
        return 0;
    } else {
        return 1;
    }
}