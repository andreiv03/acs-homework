section .text
	global inorder_intruders
	extern array_idx_2

inorder_intruders:
	enter 0, 0

	mov esi, [ebp + 8]
	mov edx, [ebp + 12]
	mov edi, [ebp + 16]

	cmp esi, 0
	jne left
	leave
	ret

	left:
		push esi
		push edx
		push edi

		push edi
		push esi
		push dword [esi + 4]
		call inorder_intruders
		add esp, 12

		pop edi
		pop edx
		pop esi

	root:
		cmp edx, 0
		je right

		cmp dword [esi + 4], 0
		jne right
		cmp dword [esi + 8], 0
		jne right

		check_left:
			mov ecx, [edx + 4]
			cmp esi, ecx
			jne check_right
			
			mov ecx, [edx]
			cmp [esi], ecx
			jge insert

		check_right:
			mov ecx, [edx + 8]
			cmp esi, ecx
			jne right

			mov ecx, [edx]
			cmp [esi], ecx
			jle insert

		jmp right

		insert:
			mov eax, 4
			imul eax, dword [array_idx_2]

			mov ecx, [esi]
			mov [edi + eax], ecx

			inc dword [array_idx_2]

	right:
		push esi
		push edx
		push edi

		push edi
		push esi
		push dword [esi + 8]
		call inorder_intruders
		add esp, 12

		pop edi
		pop edx
		pop esi

	leave
	ret
