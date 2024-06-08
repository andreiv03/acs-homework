section .data
	vowels db "aeiou", 0

section .text
	global reverse_vowels
	extern strchr

reverse_vowels:
	push ebp
	push esp
	pop ebp

	push dword [ebp + 8]
	pop esi

	push_vowels:
		push dword [esi]
		pop eax
		cmp al, 0
		je end_push_vowels

		push eax
		push vowels
		call strchr
		add esp, 8

		cmp eax, 0
		je iterate_push_vowels

		push dword [esi]

		iterate_push_vowels:
			inc esi
			jmp push_vowels

	end_push_vowels:
		push dword [ebp + 8]
		pop esi

	pop_vowels:
		push dword [esi]
		pop eax
		cmp al, 0
		je end_pop_vowels

		push eax
		push vowels
		call strchr
		add esp, 8

		cmp eax, 0
		je iterate_pop_vowels

		xor eax, eax
		and byte [esi], al
		pop eax
		add byte [esi], al

		iterate_pop_vowels:
			inc esi
			jmp pop_vowels

	end_pop_vowels:
		push dword [ebp + 8]
		pop esi

	pop ebp
	ret
