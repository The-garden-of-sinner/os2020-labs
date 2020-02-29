.text
.global  start

start:
        .long 0x1badb002
        .long 0x0
	.long -0x1badb002
si:	//output through serial interface
        movw $0x3f8, %dx
nop
nop
//output every character of "helloworldPB18111683"
        movb $0x68, %al         
        outb %al, %dx
        movb $0x65, %al       
        outb %al, %dx
        movb $0x6c, %al        
        outb %al, %dx
        movb $0x6c, %al       
        outb %al, %dx
        movb $0x6f, %al      
        outb %al, %dx
        movb $0x77, %al        
        outb %al, %dx
        movb $0x6f, %al      
        outb %al, %dx
        movb $0x72, %al       
        outb %al, %dx
        movb $0x6c, %al        
        outb %al, %dx
        movb $0x64, %al        
        outb %al, %dx
        movb $0x20, %al        
        outb %al, %dx
        movb $0x50, %al        
        outb %al, %dx
        movb $0x42, %al        
        outb %al, %dx
        movb $0x31, %al        
        outb %al, %dx
        movb $0x38, %al        
        outb %al, %dx
        movb $0x31, %al        
        outb %al, %dx
        movb $0x31, %al        
        outb %al, %dx
        movb $0x31, %al        
        outb %al, %dx
        movb $0x36, %al        
        outb %al, %dx
        movb $0x38, %al        
        outb %al, %dx
        movb $0x33, %al        
        outb %al, %dx


VGA:
//output in vga, each command write two chars into the vga video memory(显存大概是这样翻译？) 
	movl $0x2f652f68, 0xB8000
	movl $0x2f6c2f6c, 0xB8004
        movl $0x2f772f6f, 0xB8008
        movl $0x2f722f6f, 0xB800C
        movl $0x2f642f6c, 0xB8010
        movl $0x2f3c2f2c, 0xB8014
	movl $0x2f422f50, 0xB8018
        movl $0x2f382f31, 0xB801C
        movl $0x2f312f31, 0xB8020
        movl $0x2f362f31, 0xB8024
	movl $0x2f332f38, 0xB8028
	movl $0x2f5f2f3e, 0xB802C
	movl $0x2f542f3c, 0xB8030
	movl $0x2f6e2f6f, 0xB8034
	movl $0x2f4a2f67, 0xB8038
	movl $0x2f6e2f75, 0xB803C
	movl $0x2f692f78, 0xB8040
	movl $0x2f6e2f6f, 0xB8044
	movl $0x2f3e2f67, 0xB8048
    
       jl VGA
