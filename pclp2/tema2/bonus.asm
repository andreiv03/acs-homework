section .text
	global bonus

bonus:
	push ebp
	mov ebp, esp
	pusha

	mov eax, [ebp + 8]
	mov ebx, [ebp + 12]
	mov ecx, [ebp + 16]

	.check_top_left:
		push eax
		inc eax
		push ebx
		dec ebx

		cmp eax, 7
		jg .clean_top_left
		cmp ebx, 0
		jl .clean_top_left

		cmp eax, 4
		jge .adjust_top_left_first_board

		.adjust_top_left_second_board:
			push ecx
			mov ecx, eax
			mov eax, 8
			imul ecx
			add eax, ebx
			mov ecx, eax
			xor edx, edx
			mov edx, 1
			shl edx, cl
			pop ecx
			or [ecx + 4], edx
			jmp .clean_top_left

		.adjust_top_left_first_board:
			push ecx
			mov ecx, eax
			mov eax, 8
			imul ecx
			sub eax, 32
			add eax, ebx
			mov ecx, eax
			xor edx, edx
			mov edx, 1
			shl edx, cl
			pop ecx
			or [ecx + 0], edx

	.clean_top_left:
		pop ebx
		pop eax


	.check_top_right:
		push eax
		inc eax
		push ebx
		inc ebx

		cmp eax, 7
		jg .clean_top_right
		cmp ebx, 7
		jg .clean_top_right

		cmp eax, 4
		jge .adjust_top_right_first_board

		.adjust_top_right_second_board:
			push ecx
			mov ecx, eax
			mov eax, 8
			imul ecx
			add eax, ebx
			mov ecx, eax
			xor edx, edx
			mov edx, 1
			shl edx, cl
			pop ecx
			or [ecx + 4], edx
			jmp .clean_top_right

		.adjust_top_right_first_board:
			push ecx
			mov ecx, eax
			mov eax, 8
			imul ecx
			sub eax, 32
			add eax, ebx
			mov ecx, eax
			xor edx, edx
			mov edx, 1
			shl edx, cl
			pop ecx
			or [ecx + 0], edx

	.clean_top_right:
		pop ebx
		pop eax


	.check_bottom_right:
		push eax
		dec eax
		push ebx
		inc ebx

		cmp eax, 0
		jl .clean_bottom_right
		cmp ebx, 7
		jg .clean_bottom_right

		cmp eax, 4
		jge .adjust_bottom_right_first_board

		.adjust_bottom_right_second_board:
			push ecx
			mov ecx, eax
			mov eax, 8
			imul ecx
			add eax, ebx
			mov ecx, eax
			xor edx, edx
			mov edx, 1
			shl edx, cl
			pop ecx
			or [ecx + 4], edx
			jmp .clean_bottom_right

		.adjust_bottom_right_first_board:
			push ecx
			mov ecx, eax
			mov eax, 8
			imul ecx
			sub eax, 32
			add eax, ebx
			mov ecx, eax
			xor edx, edx
			mov edx, 1
			shl edx, cl
			pop ecx
			or [ecx + 0], edx

	.clean_bottom_right:
		pop ebx
		pop eax

	
	.check_bottom_left:
		push eax
		dec eax
		push ebx
		dec ebx

		cmp eax, 0
		jl .clean_bottom_left
		cmp ebx, 0
		jl .clean_bottom_left

		cmp eax, 4
		jge .adjust_bottom_left_first_board

		.adjust_bottom_left_second_board:
			push ecx
			mov ecx, eax
			mov eax, 8
			imul ecx
			add eax, ebx
			mov ecx, eax
			xor edx, edx
			mov edx, 1
			shl edx, cl
			pop ecx
			or [ecx + 4], edx
			jmp .clean_bottom_left

		.adjust_bottom_left_first_board:
			push ecx
			mov ecx, eax
			mov eax, 8
			imul ecx
			sub eax, 32
			add eax, ebx
			mov ecx, eax
			xor edx, edx
			mov edx, 1
			shl edx, cl
			pop ecx
			or [ecx + 0], edx

	.clean_bottom_left:
		pop ebx
		pop eax

	popa
	leave
	ret
