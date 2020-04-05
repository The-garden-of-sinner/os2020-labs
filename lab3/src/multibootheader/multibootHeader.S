.globl start

MAGIC = 0x1BADB002
FLAGS = 0
CHECKSUM = 0xE4524FFE

.section ".multiboot_header"
.align 4
  .long MAGIC
  .long FLAGS
  .long CHECKSUM

.text
.code32
start:
  call _start
  hlt