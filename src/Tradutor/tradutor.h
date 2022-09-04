#include <bits/stdc++.h>
using namespace std;

vector<string> section_data;
vector<string> section_bss;
vector<string> section_text;
map<int, string> labels;

void tradutor(string caminho_arq1){
    vector<int> cod_objeto = ler_arquivo(caminho_arq1);
    int cont_label = 1;
    int i = 0;

    section_data = var_data_aux();
    section_bss = var_bss_aux();

    for (i = 0; i < cod_objeto.size(); ){

        int opcode = cod_objeto[i];
        vector<int> operandos(2);

        i++;
        for(int j = 1; j < tamanho_instrucao(opcode); j++, i++)
            operandos[j-1] = cod_objeto[i];

        create_labels(labels, cont_label, opcode, operandos, section_bss, section_data, cod_objeto);

        auto aux = convert_to_IA32(opcode, operandos[0], operandos[1], labels);

        for (auto str : aux)
            section_text.push_back(str);
        
        if (opcode == 14)
            break;
    }

    cout << "section .data" << endl;
    for (int i=0; i < section_data.size(); i++){
        cout << section_data[i] << endl;
    }

    cout << "section .bss" << endl;
    for (int i=0; i < section_bss.size(); i++){
        cout << section_bss[i] << endl;
    }

    cout << "section .text" << endl;
    for (int i = 0; i < section_text.size(); i++){
        cout << section_text[i] << endl;
    }
}
