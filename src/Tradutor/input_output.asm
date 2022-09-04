section .data
neg_                    db '-'
neg_size                EQU $-neg_
newln                   db 0dH, 0aH
bytes_msg               db 'Foram lidos/escritos '
bytes_msg_size          EQU $-bytes_msg
bytes_msg_2             db ' bytes', 0dH, 0ah
bytes_msg_2_size        EQU $-bytes_msg_2

section .bss
number                  resb 10
number_of_bytes         resb 10
result_str_int          resd 1
result_int_str_reverse  resb 10
result_int_str          resb 10
result_int_str_size     resd 1

section .text
global _start
_start:
        ; input
        push 10
        push number
        call input

        ; output
        push dword [result_str_int]
        call output

        mov eax, 1
        mov ebx, 0
        int 80h


input:
        ;criando a pilha local (frame de pilha)
        push EBP
        mov EBP,ESP

        ;empilhando registradores que vão ser utilizados
        push eax
        push ebx
        push ecx

read_str:
        ; le o str
        mov eax, 3
        mov ebx, 0
        mov ecx, [EBP+8] ; ponteiro da lbl
        mov edx, [EBP+12] ; tamanho
        int 80h

        ;salva o n de bytes lidos em esi
        mov esi, eax
        
        ; exibe a msg "Foram lidos/escritos" 
        mov eax, 4
        mov ebx, 1 
        mov ecx, bytes_msg
        mov edx, bytes_msg_size
        int 80h

        ;exibe a qnt de bytes lidos
        push esi
        call convert_int_2_str
        mov eax, 4
        mov ebx, 1 
        mov ecx, result_int_str
        mov edx, [result_int_str_size]
        int 80h
        
        ; exibe a msg " bytes" 
        mov eax, 4
        mov ebx, 1 
        mov ecx, bytes_msg_2
        mov edx, bytes_msg_2_size
        int 80h

str_2_int:
        mov eax, 0 ; eax = valor total
        mov ebx, number ; ebx = numero
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
        jne fim_input ;se nao for pula para o fim
        imul eax, -1 ;se for multiplica por -1

fim_input:
        mov [result_str_int], eax ;move o resultado para result_str_int
        ;desempilha os registradores usado
        pop eax
        pop ebx
        pop ecx
        ;desempilha EBP
        pop EBP
        ret 4

output:
        ;criando a pilha local (frame de pilha)
        push EBP
        mov EBP,ESP

        mov eax, [ESP+8]
        push dword [ESP+8]
        call convert_int_2_str

        ;empilhando registradores que vão ser utilizados
        push eax
        push ebx
        push ecx
        push edx
        push esi
        push edi


        mov eax, 4
        mov ebx, 1 
        mov ecx, result_int_str
        mov edx, [result_int_str_size]
        int 80h

        mov eax, 4
        mov ebx, 1 
        mov ecx, newln
        mov edx, 2
        int 80h

        ; exibe a msg "Foram lidos/escritos" 
        mov eax, 4
        mov ebx, 1 
        mov ecx, bytes_msg
        mov edx, bytes_msg_size
        int 80h

        push dword [result_int_str_size]
        call convert_int_2_str

        ; exibe a qnt de "bytes" 
        mov eax, 4
        mov ebx, 1 
        mov ecx, result_int_str
        mov edx, [result_int_str_size]
        int 80h

        ; exibe a msg " bytes" 
        mov eax, 4
        mov ebx, 1 
        mov ecx, bytes_msg_2
        mov edx, bytes_msg_2_size
        int 80h

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


; função para converter o int para str, o resultado vai pra result_int_str
convert_int_2_str:
        push EBP
        mov EBP,ESP

        ;empilhando registradores que vão ser utilizados
        push eax
        push ebx
        push ecx
        push edx
        push esi
        push edi

        mov eax, [EBP+8] ; valor
        mov esi, result_int_str_reverse ; ponteiro para resultado
        mov edi, 0; tamanho da string

        cmp eax, 0
        jge pos

        ; print -
        mov eax,4
        mov ebx,1
        mov ecx,neg_
        mov edx,neg_size
        int 80h
        mov eax, [EBP+8] ; valor
        ; transforma para positivo
        imul eax, -1            
        inc edi    
pos: 
        mov edx, 0
        mov ecx, 10
        div ecx
        add edx, '0'
        mov [esi], edx
        inc esi
        inc edi 
        cmp eax, 0
        jne pos 

revert:
        inc edi
        mov [result_int_str_size], edi
        mov esi, result_int_str_reverse
        mov edx, result_int_str
        mov eax, 0

loop1_reverse:
        mov cl, [esi] ;pega o char
        inc eax
        cmp eax, edi ;compara final str
        ja loop2_reverse 
        inc esi 
        jmp loop1_reverse

loop2_reverse:
        dec esi 
        mov cl, [esi] ; pega o char
        dec edi ; dec no tamanho
        cmp edi, -1 ; compara se ta no inicio da sring
        je fim_convert_int_2_str
        mov [edx], byte cl ;salva o char no conteudo de esi
        inc edx ; incrementa o ponteiro
        jmp loop2_reverse

fim_convert_int_2_str:
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
