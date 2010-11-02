global initialize_arithmetic_encoder
global remove_symbol_from_stream
global initialize_arithmetic_decoder
;global encode_symbol
global flush_arithmetic_encoder
global get_current_count
global underflow_bits
global code
global low
global high
extern output_bit
extern input_bit
extern divide

segment	.bss
	code 		resw 1
	low  		resw 1
	high 		resw 1
	underflow_bits	resd 1
	range		resd 1
	counter		resw 1

segment	.text
; floating point exception
get_current_count:
	push	ebp
	mov	ebp, esp

	mov	ax, [high]
	sub	ax, [low]
	inc	ax
	movzx	eax, ax
	mov	[range], eax
	
	mov	ax, [code]
	sub	ax, [low]
	movzx	eax, ax
	inc	eax
	lea	ecx, [ebp+8]
	mov	cx, word [ecx+4]
	movzx	ecx, cx
	imul	eax, ecx
	dec	eax
	mov	ecx, [range]
	push	ecx
	push	eax
	call	divide
	
	pop	ebp
	ret

; works
initialize_arithmetic_encoder:
	mov	word [low], 0
	mov	word [high], 0xffff
	mov	dword [underflow_bits], 0
	;mov	dword [underflow_bits + 4], 0
	ret

;works
flush_arithmetic_encoder:
	push   	ebp
  	mov   	ebp, esp
        mov	eax, [low]
        and	eax, 0x4000
        push	eax
        call	output_bit
        add	esp, 4
        inc	word [underflow_bits]
while:
	test	dword [underflow_bits], 0
	je	end
	dec	dword [underflow_bits]
	mov	eax, [low]
	not	eax
	and	eax, 0x4000
	push	eax
	call 	output_bit
	add 	esp, 4
	jmp	while
end:
	pop	ebp
        ret
        
initialize_arithmetic_decoder:
	push   	ebp
  	mov   	ebp, esp
  	
	mov	word [code], 0
	mov	byte [counter], 16
for_loop:
	shl	word [code], 1
	call	input_bit
	add	[code], eax
	dec	byte [counter]
	jz	for_loop
	
	mov	word [low], 0
	mov	word [high], 0xffff
	
	pop	ebp
	ret

remove_symbol_from_stream:
	push	ebp
	mov	ebp, esp
	
	lea	ecx, [ebp+8]
	
	mov	ax, [high]
	sub	ax, [low]
	movzx	eax, ax
	inc	eax
	mov	[range], eax
	imul	ax, word [ecx+2]
	idiv	word [ecx+4]
	dec	eax
	add	ax, [low]
	mov	[high], ax
	mov	eax, [range]
	imul	ax, word [ecx]
	idiv	word [ecx+4]
	add	ax, [low]
	mov	[low], ax
	
loop2:
	mov	ax, [high]
	mov	cx, [low]
	and	ax, 0x8000
	and	cx, 0x8000
	cmp	ax, cx
	je	end
	mov	cx, [low]
	and	cx, 0x4000
	cmp	cx, 0x4000
	jne	end
	mov	ax, [high]
	and	ax, 0x4000
	jnz	end
	xor	word [code], 0x4000
	and	word [low], 0x3fff
	or	word [high], 0x4000
	jmp	end
