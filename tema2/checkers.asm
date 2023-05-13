section .text
	global checkers

checkers:
	push ebp
	mov ebp, esp
	pusha

	mov eax, [ebp + 8]
	mov ebx, [ebp + 12]
	mov ecx, [ebp + 16]

	.check_top_left:
		push eax
		dec eax
		push ebx
		inc ebx

		cmp eax, 0
		jl .clean_top_left
		cmp ebx, 7
		jg .clean_top_left

		mov edx, eax
		imul edx, 8
		add edx, ebx
		mov byte [ecx + edx], 1

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

		mov edx, eax
		imul edx, 8
		add edx, ebx
		mov byte [ecx + edx], 1

	.clean_top_right:
		pop ebx
		pop eax

	.check_bottom_right:
		push eax
		inc eax
		push ebx
		dec ebx

		cmp eax, 7
		jg .clean_bottom_right
		cmp ebx, 0
		jl .clean_bottom_right

		mov edx, eax
		imul edx, 8
		add edx, ebx
		mov byte [ecx + edx], 1

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

		mov edx, eax
		imul edx, 8
		add edx, ebx
		mov byte [ecx + edx], 1

	.clean_bottom_left:
		pop ebx
		pop eax

	popa
	leave
	ret