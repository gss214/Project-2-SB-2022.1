#include <bits/stdc++.h>
#include <fstream>

using namespace std;

map<string,int> opcodes = {{"ADD", 1}, {"SUB", 2}, {"MULT", 3}, {"DIV", 4}, {"JMP", 5}, {"JMPN", 6}, {"JMPP", 7}, {"JMPZ", 8}, {"COPY", 9}, {"LOAD", 10}, {"STORE", 11}, {"INPUT", 12}, {"OUTPUT", 13}, {"STOP", 14}, {"S_INPUT", 15}, {"S_OUTPUT", 16}};

/**
    Le um arquivo e retorna ele em um vetor de inteiros. 

    @param caminho string contendo o caminho do arquivo de entrada.
    @return um vetor de inteiros representando o codigo objeto.
*/
vector<int> ler_arquivo(string caminho_arq){
    ifstream file(caminho_arq);
    string str;
    vector<int> cod_objeto;
    while (getline(file, str)) {
        istringstream iss(str);
        string token;
        while (getline(iss, token, ' ')){
            token = token.c_str();
            if(token[0] >= '0' and token[0] <= '9')cod_objeto.push_back(stoi(token));
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

/**
    Funcao responsavel por gerar as labels no tradução. 

    @param labels.
    @param cont_label.
    @param opcode.
    @param operandos.
    @param section_bss.
    @param section_data.
    @param cod_objeto.
*/
void create_labels(map<int, string>& labels, int& cont_label, int opcode, vector<int> operandos, vector<string>& section_bss, vector<string>& section_data, vector<int>& cod_objeto){
    
    if(opcode == 15){
        if(!labels.count(operandos[0])){
            labels[operandos[0]] = "label" + to_string(cont_label);
            cont_label++;

            section_bss.push_back(labels[operandos[0]] + "\t\t resd " + to_string(operandos[1]));
        }
    }
    else if(opcode == 12){
        if(!labels.count(operandos[0])){
            labels[operandos[0]] = "label" + to_string(cont_label);
            cont_label++;

            section_bss.push_back(labels[operandos[0]] + "\t\t resd 1");
        }
    }
    else if(opcode != 14){
        if(!labels.count(operandos[0])){
            labels[operandos[0]] = "label" + to_string(cont_label);
            cont_label++;

            if(opcode != 5 and opcode != 6 and opcode != 7 and opcode != 8)
                section_data.push_back(labels[operandos[0]] + "\t\t dd " + to_string(cod_objeto[operandos[0]]));
        }
    }

    if(opcode == 9){
        if(!labels.count(operandos[1])){
            labels[operandos[1]] = "label" + to_string(cont_label);
            cont_label++;

            section_data.push_back(labels[operandos[1]] + "\t\t dd " + to_string(cod_objeto[operandos[1]]));
        }
    }
}

/**
    Funcao responsavel por gerar o .data inicial. 

    @return um vetor de strings contendo o .data inicial
*/
vector<string> var_data_aux(){
    vector<string> ret;
    ret.push_back("neg_                 db '-'");
    ret.push_back("neg_size             EQU $-neg_");
    ret.push_back("newln                db 0dH, 0aH");
    ret.push_back("bytes_msg            db 'Foram lidos/escritos '");
    ret.push_back("bytes_msg_size       EQU $-bytes_msg");
    ret.push_back("bytes_msg_2          db ' bytes', 0dH, 0ah");
    ret.push_back("bytes_msg_2_size     EQU $-bytes_msg_2");
    ret.push_back("overflow_msg         db 'Overflow Detectado! O programa sera encerrado.', 0dH, 0ah");
    ret.push_back("overflow_msg_size    EQU $-overflow_msg");

    return ret;
}


/**
    Funcao responsavel por gerar o .bss inicial. 

    @return um vetor de strings contendo o .bss inicial
*/
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


/**
    Funcao responsavel pela conversao de um opcode do coidgo objeto para IA 32. 

    @param label_jump
    @param opcode
    @param label1
    @param label2
    @param size
    @return uma string com a instrucao traduzida
*/
// LEMBRAR DE COLOCAR DIRETIVA DE TAMANHO (WORD,BYTE...)
string para_IA32(string label_jump, int opcode, string label1="", string label2="", int size=-1){

    //vector<string> res;
    string res = (label_jump == "" ? "" : label_jump + ":\n");

    switch (opcode){
        //ADD
        case 1:
        {
            res += "add EAX, [" + label1 + "]\n";
            break;
        }
        
        //SUB
        case 2:
        {
            res += "sub EAX, [" + label1 + "]\n";
            break;
        }
        
        //MUL
        case 3:
            res += "mov ECX, [" + label1 + "]\n";
            res += "imul ECX\n";
            res += "jo overflow\n";
            break;
        
        //DIV
        case 4:
            res += "mov ECX, [" + label1 + "]\n";
            res += "cdq\n";
            res += "idiv ECX\n";
            break;
        
        //JMP
        case 5:
        {
            res += "jmp " + label1 + "\n";
            break;
        }
        
        //JMPN
        case 6:
        {
            res += "cmp EAX, 0\n";
            res += "jl " + label1 + "\n";
            break;
        }

        //JMPP
        case 7:
        {
            res += "cmp EAX, 0\n";
            res += "jg " + label1 + "\n";
            break;
        }
        
        //JMPZ
        case 8:
        {
            res += "cmp EAX, 0\n";
            res += "je " + label1 + "\n";
            break;
        }
        
        // COPY
        case 9:
        {
            res += "mov EBX, [" + label1 + "]\n";
            res += "mov [" + label2 + "], EBX\n";
            break;
        }
        
        // LOAD
        case 10:
        {
            res += "mov EAX, [" + label1 + "]\n";
            break;
        }
        
        //STORE
        case 11:
        {
            res += "mov [" + label1 + "], EAX\n";
            break;    
        }
        
        //INPUT
        case 12:
        {
            res += "push number\n";
            res += "call input\n";
            res += "mov EAX, [result_str_int]\n";
            res += "mov [" + label1 + "], EAX\n";
            break;
        }
        
        //OUTPUT
        case 13:
        {
            res += "push dword [" + label1 + "]\n";
            res += "call output\n";
            break;
        }
        
        //STOP
        case 14:
        {
            res += "mov EAX, 1\n";
            res += "mov EBX, 0\n";
            res += "int 80h\n";
            break;
        } 
        
        //S_INPUT
        case 15:
        {
            res += "push " + to_string(size) + "\n";
            res += "push " + label1 + "\n";
            res += "call s_input\n";
            break;
        } 
        
        //S_OUTPUT
        case 16:
        {
            res += "push " + to_string(size) + "\n";
            res += "push " + label1 + "\n";
            res += "call s_output\n";
            break;
        } 
    }
    return res;
}

string convert_to_IA32(int opcode, int op1, int op2, map<int,string>& labels, string label_jump){
    string res;

    if(opcode == 14){
        res = para_IA32(label_jump, opcode);
    }
    else if(opcode == 9){
        res = para_IA32(label_jump, opcode, labels[op1], labels[op2]);
    }
    else if(opcode == 15 or opcode == 16){
        res = para_IA32(label_jump, opcode, labels[op1], "", op2);
    }
    else{
        res = para_IA32(label_jump, opcode, labels[op1]);
    }

    return res;
}
