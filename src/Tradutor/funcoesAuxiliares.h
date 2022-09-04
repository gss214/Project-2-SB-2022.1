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
int tamanho_instrucao(int opcode){
    if (opcode == 9 or opcode == 15 or opcode == 16){
        return 3;
    } else if (opcode == 14) {
        return 1;
    } else {
        return 2;
    }
}

void create_labels(map<int, string>& labels, int& cont_label, int opcode, vector<int> operandos, vector<string>& section_bss, vector<string>& section_data, vector<int>& cod_objeto){
    
    if(opcode == 15){
        if(labels.find(operandos[0]) != labels.end()){
            labels[operandos[0]] = "label" + to_string(cont_label);
            cont_label++;

            section_bss.push_back(labels[operandos[0]] + "\t\t resd " + to_string(operandos[1]));
        }
    }
    else if(opcode == 12){
        if(labels.find(operandos[0]) != labels.end()){
            labels[operandos[0]] = "label" + to_string(cont_label);
            cont_label++;

            section_bss.push_back(labels[operandos[0]] + "\t\t resd 1");
        }
    }
    else if(opcode != 14){
        if(labels.find(operandos[0]) != labels.end()){
            labels[operandos[0]] = "label" + to_string(cont_label);
            cont_label++;

            section_data.push_back(labels[operandos[0]] + "\t\t dd " + to_string(cod_objeto[operandos[0]]));
        }
    }

    if(opcode == 9){
        if(labels.find(operandos[1]) != labels.end()){
            labels[operandos[1]] = "label" + to_string(cont_label);
            cont_label++;

            section_data.push_back(labels[operandos[1]] + "\t\t dd " + to_string(cod_objeto[operandos[1]]));
        }
    }
}

vector<string> var_data_aux(){
    vector<string> ret;
    ret.push_back("neg_                 db '-'");
    ret.push_back("neg_size             EQU $-neg_");
    ret.push_back("newln                db 0dH, 0aH");
    ret.push_back("bytes_msg            db 'Foram lidos/escritos '");
    ret.push_back("bytes_msg_size       EQU $-bytes_msg");
    ret.push_back("bytes_msg_2          db ' bytes', 0dH, 0ah");
    ret.push_back("bytes_msg_2_size     EQU $-bytes_msg_2");

    return ret;
}

vector<string> var_bss_aux(){
    vector<string> ret;
    ret.push_back("number                  resb 10");
    ret.push_back("number_of_bytes         resb 10");
    ret.push_back("result_str_int          resd 1");
    ret.push_back("result_int_str_reverse  resb 10");
    ret.push_back("result_int_str          resb 10");
    ret.push_back("result_int_str_size     resd 1");

    return ret;
}

// LEMBRAR DE COLOCAR DIRETIVA DE TAMANHO (WORD,BYTE...)
vector<string> para_IA32(int opcode, string label1="", string label2="", int size=-1){

    vector<string> res;

    switch (opcode){
        //ADD
        case 1:
        {
            res.push_back("add EAX, [" + label1 + "]");
            break;
        }
        
        //SUB
        case 2:
        {
            res.push_back("sub EAX, [" + label1 + "]");
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
            res.push_back("jmp " + label1);
            break;
        }
        
        //JMPN
        case 6:
        {
            res.push_back("cmp EAX, 0");
            res.push_back("jl " + label1);
            break;
        }

        //JMPP
        case 7:
        {
            res.push_back("cmp EAX, 0");
            res.push_back("jg " + label1);
            break;
        }
        
        //JMPZ
        case 8:
        {
            res.push_back("cmp EAX, 0");
            res.push_back("je " + label1);
            break;
        }
        
        // COPY
        case 9:
        {
            res.push_back("mov EBX, [" + label1 + "]");
            res.push_back("mov [" + label2 + "], EBX");
            break;
        }
        
        // LOAD
        case 10:
        {
            res.push_back("mov EAX, [" + label1 + "]");
            break;
        }
        
        //STORE
        case 11:
        {
            res.push_back("mov " + label1 + ", [EAX]");
            break;    
        }
        
        //INPUT
        case 12:
        {
            res.push_back("push number");
            res.push_back("call input");
            res.push_back("mov eax, [result_str_int]");
            res.push_back("mov [" + label1 + "], eax");
            break;
        }
        
        //OUTPUT
        case 13:
        {
            res.push_back("push dword [" + label1 + "]");
            res.push_back("call output");
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
            res.push_back("push " + to_string(size));
            res.push_back("push " + label1);
            res.push_back("call s_input");
            break;
        } 
        
        //S_OUTPUT
        case 17:
        {
            res.push_back("push " + to_string(size));
            res.push_back("push " + label1);
            res.push_back("call s_input");
            break;
        } 
    }
    return res;
}

vector<string> convert_to_IA32(int opcode, int op1, int op2, map<int,string>& labels){
    vector<string> res;

    if(opcode == 14){
        res = para_IA32(opcode);
    }
    else if(opcode == 9){
        res = para_IA32(opcode, labels[op1], labels[op2]);
    }
    else if(opcode == 15 or opcode == 16){
        res = para_IA32(opcode, labels[op1], "", op2);
    }
    else{
        res = para_IA32(opcode, labels[op1]);
    }

    return res;
}