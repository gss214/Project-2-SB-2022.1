#include <bits/stdc++.h>
using namespace std;

vector<string> section_data;
vector<string> section_bss;
vector<string> section_text;

void tradutor(string caminho_arq1){
    vector<int> cod_objeto = ler_arquivo(caminho_arq1);

    for (int i = 0; i < cod_objeto.size(); ){

        int opcode = cod_objeto[i];
        vector<int> operandos(2);

        i++;
        for(int j = 1; j < tamanhoDaInstrucao(opcode); j++, i++)
            operandos[j-1] = cod_objeto[i];
        
       
        auto aux = paraIA32(opcode, operandos[0], operandos[1]);
        for (auto str : aux)
            section_text.push_back(str);
        
        if (opcode == 14)
            break;
    }

    cout << "section .text" << endl;
    for (int i = 0; i < section_text.size(); i++){
        cout << section_text[i] << endl;
    }
}
