# void uswtch(struct context **old, struct context *new);
#
# struct context { edi, esi, ebx, ebp, eip, esp }
# Save callee regs, return address (eip), and stack pointer (esp), then
# load the next context and switch stacks before ret.

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
  movl %esp, 20(%ecx)

  movl %edx, (%eax)

  movl %edx, %ecx
  movl 0(%ecx), %edi
  movl 4(%ecx), %esi
  movl 8(%ecx), %ebx
  movl 12(%ecx), %ebp
  movl 20(%ecx), %esp
  ret
