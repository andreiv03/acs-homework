struc proc
	.pid: resw 1
	.prio: resb 1
	.time: resw 1
endstruc

section .text
	global sort_procs

sort_procs:
	enter 0, 0
	pusha

	mov edx, [ebp + 8]
	mov eax, [ebp + 12]

	mov ebx, eax
	dec ebx

	sub esp, 4

	.outer_loop:
		mov dword [ebp - 4], 0

		.inner_loop:
			push edx
			mov eax, proc_size
			mul dword [ebp - 4]
			pop edx

			.check_prio:
				mov cl, byte [edx + eax + proc.prio]
				cmp cl, byte [edx + eax + proc_size + proc.prio]
				jg .swap
				jl .iterate_inner_loop

			.check_time:
				mov cx, word [edx + eax + proc.time]
				cmp cx, word [edx + eax + proc_size + proc.time]
				jg .swap
				jl .iterate_inner_loop

			.check_pid:
				mov cx, word [edx + eax + proc.pid]
				cmp cx, word [edx + eax + proc_size + proc.pid]
				jg .swap
				jmp .iterate_inner_loop

			.swap:
				push word [edx + eax + proc.pid]
				push word [edx + eax + proc_size + proc.pid]
				pop cx
				mov word [edx + eax + proc.pid], cx
				pop cx
				mov word [edx + eax + proc_size + proc.pid], cx

				mov cl, byte [edx + eax + proc.prio]
				push cx
				mov cl, byte [edx + eax + proc_size + proc.prio]
				push cx
				pop cx
				mov byte [edx + eax + proc.prio], cl
				pop cx
				mov byte [edx + eax + proc_size + proc.prio], cl

				push word [edx + eax + proc.time]
				push word [edx + eax + proc_size + proc.time]
				pop cx
				mov word [edx + eax + proc.time], cx
				pop cx
				mov word [edx + eax + proc_size + proc.time], cx

			.iterate_inner_loop:
				inc dword [ebp - 4]
				cmp dword [ebp - 4], ebx
				jl .inner_loop

		.iterate_outer_loop:
			dec ebx
			jnz .outer_loop

	add esp, 4
	
	popa
	leave
	ret
