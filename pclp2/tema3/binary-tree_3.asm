section .text
	global inorder_fixing

inorder_fixing:
	enter 0, 0

	mov esi, [ebp + 8]
	mov edx, [ebp + 12]

	cmp esi, 0
	jne left
	leave
	ret

	left:
		push esi
		push edx

		push esi
		push dword [esi + 4]
		call inorder_fixing
		add esp, 8

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
			jge modify_left

		check_right:
			mov ecx, [edx + 8]
			cmp esi, ecx
			jne right

			mov ecx, [edx]
			cmp [esi], ecx
			jle modify_right

		jmp right

		modify_left:
			mov ecx, [edx]
			mov [esi], ecx
			dec dword [esi]
			jmp right
			
		modify_right:
			mov ecx, [edx]
			mov [esi], ecx
			inc dword [esi]
			jmp right

	right:
		push esi
		push edx

		push esi
		push dword [esi + 8]
		call inorder_fixing
		add esp, 8

		pop edx
		pop esi

	leave
	ret
