# Project-2-SB-2022.1

## To-do

- [x] traduzir opcodes simples
- [x] section .data e .bss
- [x] input e output
- [x] s_input e s_output
- [ ] multiplicacao e divisao
- [ ] tratar label do jump
  - quando o opcode for um jump:
    - se o jump for pra trás: sec_text[pos] = "label: " + sec_text[pos]
    - senao, dar um push_back(label)
- [ ] retornar no eax a qtd de bytes lidos/escritos
