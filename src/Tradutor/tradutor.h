#include <bits/stdc++.h>
using namespace std;

/**
    Funcao responsavel pela tradução de codigo objeto para IA 32. 
    @param caminho_arq1 string representando o caminho do arquivo a ser traduzido.
*/
void tradutor(string caminho_arq1){

    vector<string> section_data;
    vector<string> section_bss;
    map<int, string> section_text;
    map<int, string> labels;
    map<int, string> flag_jump;

    vector<int> cod_objeto = ler_arquivo(caminho_arq1); 
    int cont_label = 1;
    int i = 0;

    section_data = var_data_aux();
    section_bss = var_bss_aux();

    for (i = 0; i < (int)cod_objeto.size(); ){

        int i_opcode = i;
        int qtd_operandos = 0;
        int opcode = cod_objeto[i];
        vector<int> operandos(2);

        i++;
        for(int j = 1; j < tamanho_instrucao(opcode); j++, i++){
            operandos[j-1] = cod_objeto[i];
            qtd_operandos++;
        }

        create_labels(labels, cont_label, opcode, operandos, section_bss, section_data, cod_objeto);

        if(opcode == 5 or opcode == 6 or opcode == 7 or opcode == 8){
            if(operandos[0] > i_opcode) flag_jump[operandos[0]] = labels[operandos[0]];
            else section_text[operandos[0]] = labels[operandos[0]] + ":\n" + section_text[operandos[0]];
        }

        string label_jump = (flag_jump.count(i_opcode) ? flag_jump[i_opcode] : "");
        string res = convert_to_IA32(opcode, operandos[0], operandos[1], labels, label_jump);
        section_text[i_opcode] = res;
        
        if (opcode == 14)
            break;
    }

    string line;
    ifstream in_file{"functions.asm"};
    ofstream out_file{"output.asm"};
    if (in_file && out_file) {
        while (getline(in_file, line)) {
            out_file << line << "\n";
        }
        
        out_file << endl;
        out_file << "section .data" << endl;
        for (int i=0; i < (int)section_data.size(); i++){
            out_file << section_data[i] << endl;
        }

        out_file << "\nsection .bss" << endl;
        for (int i=0; i < (int)section_bss.size(); i++){
            out_file << section_bss[i] << endl;
        }

        out_file << "\nsection .text" << endl;
        out_file << "global _start\n_start:" << endl;
        for (auto [idx, res] : section_text){
            out_file << res;
        }
    }

    in_file.close();
    out_file.close();
}
