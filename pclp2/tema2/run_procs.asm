struc avg
	.quo: resw 1
	.remain: resw 1
endstruc

struc proc
	.pid: resw 1
	.prio: resb 1
	.time: resw 1
endstruc

section .data
	prio_result dd 0, 0, 0, 0, 0
	time_result dd 0, 0, 0, 0, 0

section .text
	global run_procs

run_procs:
	push ebp
	mov ebp, esp
	pusha

	xor ecx, ecx

clean_results:
	mov dword [prio_result + 4 * ecx], 0
	mov dword [time_result + 4 * ecx], 0

	inc ecx
	cmp ecx, 5
	jne clean_results

	mov ecx, [ebp + 8]
	mov ebx, [ebp + 12]
	mov eax, [ebp + 16]

	dec ebx

	.loop_one:
		.count_processes:
			push eax
			push ebx
			mov eax, proc_size
			mul ebx
			xor ebx, ebx
			mov bl, byte [ecx + eax + proc.prio]
			inc dword [prio_result + 4 * ebx - 4]
			pop ebx
			pop eax

		.sum_times:
			push eax
			push ebx
			mov eax, proc_size
			mul ebx
			xor ebx, ebx
			mov bl, byte [ecx + eax + proc.prio]
			xor edx, edx
			mov dx, word [ecx + eax + proc.time]
			add dword [time_result + 4 * ebx - 4], edx
			pop ebx
			pop eax
		
		.iterate_loop_one:
			dec ebx
			cmp ebx, 0
			jge .loop_one

	mov ebx, 4

	.loop_two:
		.check:
			cmp dword [prio_result + 4 * ebx], 0
			je .iterate_loop_two

		.apply:
			push eax
			mov eax, dword [time_result + 4 * ebx]
			xor edx, edx
			mov ecx, dword [prio_result + 4 * ebx]
			div ecx
			mov ecx, eax
			pop eax

			mov word [eax + 4 * ebx + avg.quo], cx
			mov word [eax + 4 * ebx + avg.remain], dx

		.iterate_loop_two:
			dec ebx
			cmp ebx, 0
			jge .loop_two

	popa
	leave
	ret
