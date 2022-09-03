# Project-2-SB-2022.1

## To-do

- [x] traduzir opcodes simples
- [ ] section .data e .bss
- [ ] multiplicacao e divisao
- [x] input e output
    - [ ] bytes lidos e escritos
- [ ] s_input e s_output

## DORES

### Como traduzir os dados:

- INPUT -> coloca pra .bss como byte
    - converte string pra num e declara no .data
- S_INPUT -> coloca pra .bss como vetor de byte
    - converte string pra num e declara no .data ?
- SENAO -> declara no .data como const

- no S_INPUT:
    - declarar antes o label
    - mov [qnt_loop_s_input], qnt
    - fazer um loop da qnt
    - call input  
