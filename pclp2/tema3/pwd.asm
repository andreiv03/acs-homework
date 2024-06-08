section .data
	back db "..", 0
	curr db ".", 0
	slash db "/", 0
	
	SLASH_ASCII_CODE equ 47

section .text
	global pwd
	
	extern strcmp
	extern strcat
	extern strlen
	extern strrchr

pwd:
	enter 0, 0

	mov esi, [ebp + 8]
	mov ecx, [ebp + 12]
	mov edi, [ebp + 16]

	loop:
		check_current:
			push ecx
			push dword [esi]
			push curr
			call strcmp
			add esp, 8
			pop ecx

			cmp eax, 0
			jz iterate_loop

		check_parent:
			push ecx
			push dword [esi]
			push back
			call strcmp
			add esp, 8
			pop ecx

			cmp eax, 0
			jnz add_slash

		check_empty_string:
			push ecx
			push edi
			call strlen
			add esp, 4
			pop ecx

			cmp eax, 0
			jz iterate_loop

		remove_string:
			push eax

			push ecx
			push SLASH_ASCII_CODE
			push edi
			call strrchr
			add esp, 8
			pop ecx

			push ecx
			push eax
			call strlen
			add esp, 4
			pop ecx

			pop edx
			sub edx, eax
			mov byte [edi + edx], 0
			jmp iterate_loop

		add_slash:
			push ecx
			push slash
			push edi
			call strcat
			add esp, 8
			pop ecx

		add_string:
			push ecx
			push dword [esi]
			push edi
			call strcat
			add esp, 8
			pop ecx

		iterate_loop:
			add esi, 4
			dec ecx
			cmp ecx, 0
			jnz loop

	push slash
	push edi
	call strcat
	add esp, 8

	leave
	ret
