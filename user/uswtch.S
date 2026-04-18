# void uswtch(struct context **old, struct context *new);
#
# Save callee-saved registers and return address into *old,
# then load from new and jump (ret) to new's eip.

.globl uswtch
uswtch:
  movl 4(%esp), %eax
  movl 8(%esp), %edx

  movl (%eax), %ecx

  movl %edi, 0(%ecx)
  movl %esi, 4(%ecx)
  movl %ebx, 8(%ecx)
  movl %ebp, 12(%ecx)
  movl (%esp), %edi
  movl %edi, 16(%ecx)

  movl %edx, (%eax)

  movl %edx, %ecx
  movl 0(%ecx), %edi
  movl 4(%ecx), %esi
  movl 8(%ecx), %ebx
  movl 12(%ecx), %ebp
  movl 16(%ecx), %eax
  movl %eax, (%esp)
  ret
