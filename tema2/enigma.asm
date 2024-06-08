LETTERS_COUNT EQU 26

section .data
  extern len_plain

section .text
	global rotate_x_positions
	global enigma

rotate_x_positions:
	push ebp
	mov ebp, esp
	pusha

	mov eax, [ebp + 8]
	mov ebx, [ebp + 12]
	mov ecx, [ebp + 16]
	mov edx, [ebp + 20]
	
	add esp, 8
	mov [ebp - 4], eax
	mov edi, 1

	imul ebx, 2
	imul ebx, LETTERS_COUNT

	cmp edx, 0
	jz .shift_left
	jnz .shift_right

	.shift_left:
		cmp byte [ebp - 4], 0
		jz .end

		mov eax, [ebp - 4]

		.push_loop_left:
			add ebx, eax
			push dword [ecx + ebx - 1]
			sub ebx, eax

			dec eax
			cmp eax, 0
			jg .push_loop_left

		mov byte [ebp - 8], LETTERS_COUNT
		mov eax, [ebp - 4]
		sub [ebp - 8], eax
		mov eax, 0

		.shift_loop_left:
			add ebx, eax
			add ebx, [ebp - 4]
			mov edx, [ecx + ebx]
			sub ebx, [ebp - 4]
			mov [ecx + ebx], dl
			sub ebx, eax

			inc eax
			cmp eax, [ebp - 8]
			jl .shift_loop_left

		mov eax, LETTERS_COUNT
		sub eax, [ebp - 4]

		.pop_loop_left:
			add ebx, eax
			pop edx
			mov [ecx + ebx], dl
			sub ebx, eax

			inc eax
			cmp eax, LETTERS_COUNT
			jl .pop_loop_left

		dec edi
		cmp edi, 0
		jl .end
		
		add ebx, LETTERS_COUNT
		jmp .shift_left

	jmp .end

	.shift_right:
		cmp byte [ebp - 4], 0
		jz .end

		mov eax, LETTERS_COUNT
		sub eax, [ebp - 4]

		.push_loop_right:
			add ebx, eax
			push dword [ecx + ebx]
			sub ebx, eax

			inc eax
			cmp eax, LETTERS_COUNT
			jl .push_loop_right

		mov eax, [ebp - 4]
		mov [ebp - 8], eax
		dec byte [ebp - 8]
		mov eax, LETTERS_COUNT
		dec eax

		.shift_loop_right:
			add ebx, eax
			sub ebx, [ebp - 4]
			mov edx, [ecx + ebx]
			add ebx, [ebp - 4]
			mov [ecx + ebx], dl
			sub ebx, eax

			dec eax
			cmp eax, [ebp - 8]
			jg .shift_loop_right

		mov eax, [ebp - 4]

		.pop_loop_right:
			add ebx, eax
			pop edx
			mov [ecx + ebx - 1], dl
			sub ebx, eax

			dec eax
			cmp eax, 0
			jg .pop_loop_right

		dec edi
		cmp edi, 0
		jl .end
		
		add ebx, LETTERS_COUNT
		jmp .shift_right

	.end:
		sub esp, 8

	popa
	leave
	ret

enigma:
	push ebp
	mov ebp, esp
	pusha

	mov eax, [ebp + 8]
	mov ebx, [ebp + 12]
	mov ecx, [ebp + 16]
	mov edx, [ebp + 20]
	mov edi, [ebp + 24]

	popa
	leave
	ret
