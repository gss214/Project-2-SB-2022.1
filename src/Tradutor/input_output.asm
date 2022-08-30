section .data
msg                     db 'Please input the number: '
msgsize                 EQU $-msg
msgFinal                db 'Your number is: '
msgFinalsize            EQU $-msg

section .bss
number                  resd 1
result_str_int          resd 1
result_int_str          resd 1
result_int_str_size     resd 1

section .text
global _start
_start:
        ; exibe a msg na tela
        mov eax,4
        mov ebx,1
        mov ecx,msg
        mov edx,msgsize
        int 80h
        
        ; le o numero (str)
        mov eax, 3
        mov ebx, 0
        mov ecx, number ; 3 bits contando com o sinal
        mov edx, 32
        int 80h

        ;função de converter string para int
        push dword number
        call convert_str_2_int
        ;mov eax, [result_str_int]
        
        ;função de converter int para string
        push dword [result_str_int]
        call convert_int_2_str
        a:
        mov eax, [result_int_str_size]
        ; exibe a msg final na tela
        mov eax,4
        mov ebx,1
        mov ecx,msgFinal
        mov edx,msgFinalsize
        int 80h

        ; exibe a o numero digitado pelo usuario
        mov eax,4
        mov ebx,1
        mov ecx,result_int_str
        mov edx,[result_int_str_size]
        int 80h

        fimprog:
                mov eax, 1
                mov ebx, 0
                int 80h


convert_str_2_int:
        ;criando a pilha local (frame de pilha)
        push EBP
        mov EBP,ESP

        ;empilhando registradores que vão ser utilizados
        push eax
        push ebx
        push ecx

        mov eax, 0 ; eax = valor total
        mov ebx, [EBP+8] ; ebx = numero
        loopchar: 
                movzx ecx, byte [ebx] ;carrega o char 
                inc ebx
                ;verificando o sinal (soh ignora pq vai ser tratado no final)
                cmp ecx, '-'
                je loopchar
                ; verificando se chegou no fim
                cmp ecx, '0'
                jb ehneg?
                cmp ecx, '9'
                ja ehneg?

                ;convertendo
                sub ecx, '0'
                imul eax, 10
                add eax, ecx

                jmp loopchar
      
        ehneg?:    
                    mov ebx, [EBP+8] ; ebx = numero
                    movzx ecx, byte [ebx] ;carrega o primeiro char
                    cmp ecx, '-' ;verifica se é um -
                    jne fim_convert_str_2_int ;se nao for pula para o fim
                    imul eax, -1 ;se for multiplica por -1
                    
        fim_convert_str_2_int:
            mov [result_str_int], eax ;move o resultado para result_str_int
            ;desempilha os registradores usado
            pop eax
            pop ebx
            pop ecx
            ;desempilha EBP
            pop EBP
            ret 4

convert_int_2_str:
                ;criando a pilha local (frame de pilha)
                push EBP
                mov EBP,ESP

                ;empilhando registradores que vão ser utilizados
                push eax
                push ebx
                push ecx
                push edx
                push esi
                push edi
                mov eax, [EBP+8]
                mov esi, result_int_str
                mov edi, 0 ;tamanho da string
                ;mov ebx, [EBP+8] ; ebx = numero
                loopint:
                        ;faz a divisao do numero por 10, no eax fica o dividendo, no ecx fica o divisor
                        mov edx, 0
                        mov ecx, 10
                        div ecx ;resultado da divisao: no eax fica o quociente no edx fica o resto e
                        add edx, '0'
                        mov [esi], edx ;move o que ta em edx para o conteudo de esi = result_int_str
                        inc esi ; +1 no ponteiro result_int_str
                        inc edi
                        cmp eax, 0 ;compara se o numero é diferente de 0
                        jne loopint
                fim_convert_int_2_str:
                        mov [esi], byte 0Dh
                        inc edi
                        mov [result_int_str_size], edi
                        ;desempilha os registradores usado
                        pop eax
                        pop ebx
                        pop ecx
                        pop edx
                        pop esi
                        pop edi
                        ;desempilha EBP
                        pop EBP
                        ret 4
