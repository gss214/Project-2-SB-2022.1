s_input:

        ;criando a pilha local (frame de pilha)
        push EBP
        mov EBP,ESP

        ;empilhando registradores que vão ser utilizados
        push ebx
        push ecx
        push esi
        push edi

        sub esi, esi ; esi = 0 eh o indice
        sub ecx, ecx ; n total de bytes

loop_s_input:

        ;chama input
        push number
        call input
        add ecx, eax

        mov edi, [EBP+8] ; edi = ponteiro pra label
        mov ebx, [EBP+12] ; ebx = numero de iteracoes

        mov eax, [result_str_int]
        mov [edi+esi*4], eax ; edi[esi] = resultado da chamado do input
        inc esi ; esi += 1

        ; loop enquanto eax for menor que o numero de iteracoes
        cmp esi, ebx
        jl loop_s_input

fim_s_input:

        mov eax, ecx ;n total de bytes
        ; desempilha registradores
        pop esi
        pop edi
        pop ecx
        pop ebx
        pop EBP ;desempilha EBP
        ret 4

s_output:

        ;criando a pilha local (frame de pilha)
        push EBP
        mov EBP,ESP

        ;empilhando registradores que vão ser utilizados
        push ebx
        push ecx
        push esi
        push edi

        sub esi, esi ; esi = 0 eh o indice
        sub ecx, ecx ; n total de bytes

loop_s_output:

        mov edi, [EBP+8] ; edi = ponteiro pra label
        mov ebx, [EBP+12] ; ebx = numero de iteracoes

        ;chama output
        push dword [edi+esi*4]
        call output
        add ecx, eax

        inc esi

        ; loop enquanto eax for menor que o numero de iteracoes
        cmp esi, ebx
        jl loop_s_output

fim_s_output:

        mov eax, ecx ;n total de bytes
        ; desempilha registradores
        pop esi
        pop edi
        pop ecx
        pop ebx
        pop EBP ;desempilha EBP
        ret 4


input:
        ;criando a pilha local (frame de pilha)
        push EBP
        mov EBP,ESP

        ;empilhando registradores que vão ser utilizados
        pusha

read_str:
        ; le o str
        mov eax, 3
        mov ebx, 0
        mov ecx, [EBP+8] ; ponteiro da lbl
        mov edx, 10 ; tamanho
        int 80h

        ;salva o n de bytes lidos
        mov [number_of_bytes], eax
        
        ; exibe a msg "Foram lidos/escritos" 
        mov eax, 4
        mov ebx, 1 
        mov ecx, bytes_msg
        mov edx, bytes_msg_size
        int 80h

        ;exibe a qnt de bytes lidos
        push dword [number_of_bytes]
        call convert_int_2_str
        mov eax, [result_int_str]
        
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
        popa
        ;desempilha EBP
        pop EBP
        mov eax, [number_of_bytes]
        ret 4

output:
        ;criando a pilha local (frame de pilha)
        push EBP
        mov EBP,ESP

        ;empilhando registradores que vão ser utilizados
        pusha

        mov eax, [EBP+8]
        push dword [EBP+8]
        call convert_int_2_str      
        mov eax, [result_int_str_size]
        mov [number_of_bytes], eax

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
        popa
        ;desempilha EBP
        pop EBP
        mov eax, [number_of_bytes]
        ret 4


; função para converter o int para str, o resultado vai pra result_int_str
convert_int_2_str:
        push EBP
        mov EBP,ESP

        ;empilhando registradores que vão ser utilizados
        pusha

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
        mov [edx], byte cl ;salva o char no conteudo de edx
        inc edx ; incrementa o ponteiro
        jmp loop2_reverse

fim_convert_int_2_str:
        ;desempilha os registradores usado
        popa
        ;desempilha EBP
        pop EBP
        ret 4
