//-------------------------------------------------------------------
//
// Simple Boot Stage 1:
//
//
// COMPILE:
//   gcc -s -c boot.s -o boot.o -Wall
//   ld boot.o -o boot.bin -Ttext=0x7c00 -e _start
//   objcopy -O binary boot.bin bkernel
// OR:
//   make
//
//
// TESTING WITH QEMU:
//   qemu-system-i386 bkernel
//
//
// CREATE A BOOTABLE CDROM:
//   mkdosfs -C floppy.flp 1440 || exit
//   dd status=noxfer conv=notrunc if=bkernel of=floppy.flp || exit
//   mkisofs -R -b floppy.flp -o /temp/hello_boot.iso /temp/hello_boot
//
//
// REFERENCE:
//   01: https://github.com/mig-hub/mikeOS
//   02: https://github.com/mit-pdos/xv6-public
//   03: https://github.com/benchlab/benOS-Bootloaders
//   04: http://www.sde.cs.titech.ac.jp/~gondow/udos/index.html
//   05: https://github.com/microsoft/MS-DOS
//   06: https://github.com/cirosantilli/x86-bare-metal-examples
//   07: https://github.com/devversion/CKernel
//
//
// BY: Francisco - gokernel@hotmail.com
//
//-------------------------------------------------------------------
//
.code16
.text
.globl _start;
_start:

    jmp boot
    nop

boot:

    cli

    //-----------------------------------------------
    // basic setup:
    //-----------------------------------------------
    //
    mov   $0,   %ax // set up segments
    mov   %ax,  %ds
    mov   %ax,  %es
    mov   %ax,  %ss // setup stack
    mov   %ax,  %si
    //-----------------------------------------------

    mov   $0x7c00,  %sp // stack grows downwards from 0x7C00

    //-----------------------------------------------
    //---------------  START ALL HERE  --------------
    //-----------------------------------------------
    //
    // Display: "Hello"
    //
    mov   $0xe,%ah
    mov   $72,%al
    int   $0x10
    mov   $101,%al
    int   $0x10
    mov   $108,%al
    int   $0x10
    int   $0x10
    mov   $111,%al
    int   $0x10

//-----------------------------------------------
//-----------  HALT AND FOREVER LOOP  -----------
//-----------------------------------------------
label_forever:
    hlt
    jmp label_forever


//-----------------------------------------------
// strings:
//-----------------------------------------------
//
msgWelcome: .asciz "Welcome to My OS!\n\r\0"

//-----------------------------------------------
//---------  BOOT SIGNATURE 512 BYTES  ----------
//-----------------------------------------------
. = _start + 510
.byte 0x55
.byte 0xaa

// COMPILE:
//-----------------------------------------------
//   gcc -s -c boot.s -o boot.o -Wall
//   ld boot.o -o boot.bin -Ttext=0x7c00 -e _start
//   objcopy -O binary boot.bin bkernel
//-----------------------------------------------

