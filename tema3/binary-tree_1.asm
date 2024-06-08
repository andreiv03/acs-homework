section .text
	global inorder_parc
	extern array_idx_1

inorder_parc:
	enter 0, 0
	
	mov esi, [ebp + 8]
	mov edi, [ebp + 12]

	cmp esi, 0
	jne left
	leave
	ret

	left:
		push esi
		push edi

		push edi
		push dword [esi + 4]
		call inorder_parc
		add esp, 8

		pop edi
		pop esi

	root:
		mov eax, 4
		imul eax, dword [array_idx_1]

		mov edx, [esi]
		mov [edi + eax], edx

		inc dword [array_idx_1]

	right:
		push esi
		push edi

		push edi
		push dword [esi + 8]
		call inorder_parc
		add esp, 8

		pop edi
		pop esi

	leave
	ret
