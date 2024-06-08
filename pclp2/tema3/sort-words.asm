section .data
	delimiters db " ,.", 0

section .text
	global get_words
	global compare_func
	global sort

	extern strtok
	extern strlen
	extern strcmp
	extern qsort

get_words:
	enter 0, 0

	mov esi, [ebp + 8]
	mov edi, [ebp + 12]
	mov ecx, [ebp + 16]

	push edi

	push ecx
	push delimiters
	push esi
	call strtok
	add esp, 8
	pop ecx

	mov dword [edi], eax
	dec ecx

	loop:
		add edi, 4

		push ecx
		push delimiters
		push 0
		call strtok
		add esp, 8
		pop ecx

		mov dword [edi], eax

		iterate_loop:
			dec ecx
			cmp ecx, 0
			jnz loop

	leave
	ret

compare_func:
	enter 0, 0

	push esi
	push edi

	mov esi, [ebp + 8]
	mov esi, [esi]
	mov edi, [ebp + 12]
	mov edi, [edi]

	push esi
	call strlen
	add esp, 4

	push eax

	push edi
	call strlen
	add esp, 4

	compare_by_length:
		pop edx
		cmp edx, eax
		jl smaller
		jg larger

	compare_by_lexicography:
		push edi
		push esi
		call strcmp
		add esp, 8

		cmp eax, 0
		jg larger

	smaller:
		mov eax, -1
		pop edi
		pop esi
		leave
		ret

	larger:
		mov eax, 1
		pop edi
		pop esi
		leave
		ret

	xor eax, eax
	pop edi
	pop esi
	leave
	ret

sort:
	enter 0, 0

	mov esi, [ebp + 8]
	mov ecx, [ebp + 12]
	mov edx, [ebp + 16]

	push compare_func
	push edx
	push ecx
	push esi
	call qsort
	add esp, 16

	leave
	ret
