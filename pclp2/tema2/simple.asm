section .text
  global simple

simple:
  push ebp
  mov ebp, esp
  pusha

	mov ecx, [ebp + 8]
	mov esi, [ebp + 12]
	mov edi, [ebp + 16]
	mov edx, [ebp + 20]

	.loop:
		mov al, [esi + ecx - 1]
		add al, dl

		.check:
			cmp al, 'Z'
			jle .apply

			mov ah, al
			sub ah, 'Z'
			mov al, 'A' - 1
			add al, ah
			jmp .apply

		.apply:
			mov [edi + ecx - 1], al
			jmp .iterate_loop

		.iterate_loop:
			dec ecx
			jnz .loop

	popa
	leave
	ret
