section .data
read0: db 'How many numbers? ',0
read0len: equ $-read0
strconst0: db '1: 0',0
strconst0len: equ $-strconst0
strconst1: db '2: 1',0
strconst1len: equ $-strconst1
strconst2: db ': ',0
strconst2len: equ $-strconst2

section .bss
n: resb 4
buffer: resb 255
first: resb 4
second: resb 4
temp: resb 4
i: resb 4

section .text
global _start

_start:
mov eax,read0
call sprint
call readint
mov [n],eax
mov eax,[n]
push eax
mov eax,2
comparison0:
pop ebx
cmp ebx,eax
jge .comptrue
mov eax,0
jmp .done
.comptrue:
mov eax,1
.done:
push eax
cmp eax,0
je iflbl0
call proc0
iflbl0:
pop eax
ifblock0:
xor ebx,ebx
quit: mov eax,1
int 80h

strlen:
push ebx
mov ebx,eax
.next:
cmp byte[eax],0
jz .finished
inc eax
jmp .next
.finished:
sub eax,ebx
pop ebx
ret

sprint:
push edx
push ecx
push ebx
push eax
call strlen
mov edx,eax
pop eax
mov ecx,eax
mov ebx,1
mov eax,4
int 80h
pop ebx
pop ecx
pop edx
ret

atoi:
push ebx
push ecx
push edx
push esi
mov esi,eax
mov eax,0
mov ecx,0
.multiplyLoop:
xor ebx,ebx
mov bl,[esi+ecx]
cmp bl,48
jl  .finished
cmp bl,57
jg  .finished
cmp bl,10
je .finished
cmp bl,0
jz .finished
sub bl,48
add eax,ebx
mov ebx,10
mul ebx
inc ecx
jmp .multiplyLoop
.finished:
mov ebx,10
div ebx
pop esi
pop edx
pop ecx
pop ebx
ret

readint:
mov edx,255
mov ecx,buffer
mov ebx,0
mov eax,3
int 80h
mov eax,buffer
call atoi
ret

proc0:
push ebx
mov ebx,strconst0
mov eax,buffer
call strcpy
mov eax,buffer
pop ebx
call sprint
call printLF
push ebx
mov ebx,strconst1
mov eax,buffer
call strcpy
mov eax,buffer
pop ebx
call sprint
call printLF
mov eax,0
mov [first],eax
mov eax,1
mov [second],eax
mov eax,2
mov [i],eax
whileloop0:
mov eax,[i]
push eax
mov eax,[n]
comparison1:
pop ebx
cmp ebx,eax
jl .comptrue
mov eax,0
jmp .done
.comptrue:
mov eax,1
.done:
cmp eax,0
je donewhileloop0
call proc1
jmp whileloop0
donewhileloop0:
ret

strcpy:
push eax
mov eax,ebx
call strlen
mov ecx,eax
add ecx,1
pop eax
mov esi,ebx
mov edi,eax
rep movsb
ret

printLF:
push eax
mov eax, 0Ah
push eax
mov eax,esp
call sprint
pop eax
pop eax
ret

proc1:
mov eax,[i]
push eax
mov eax,1
pop ebx
add eax,ebx
mov [i],eax
mov eax,[second]
mov [temp],eax
mov eax,[first]
push eax
mov eax,[second]
pop ebx
add eax,ebx
mov [second],eax
mov eax,[temp]
mov [first],eax
mov eax,[i]
call uiprint
push ebx
mov ebx,strconst2
mov eax,buffer
call strcpy
mov eax,buffer
pop ebx
call sprint
mov eax,[second]
call uiprint
call printLF
ret

uiprint:
push eax
push ecx
push edx
push esi
mov ecx,0
.divideLoop:
inc ecx
mov edx,0
mov esi,10
idiv esi
add edx,48
push edx
cmp eax,0
jnz .divideLoop
.printLoop:
dec ecx
mov eax,esp
call sprint
pop eax
cmp ecx,0
jnz .printLoop
pop esi
pop edx
pop ecx
pop eax
ret

