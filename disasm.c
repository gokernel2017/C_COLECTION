//-------------------------------------------------------------------
//
// Disasm x86 | AT&T syntax:
//
// void Disasm (UCHAR *code, char *name, int len);
//
// FILE:
//   disasm.c
//
// COMPILE:
//   gcc disasm.c -o disasm -Wall
//
// OPCODES IMPLEMENTED:
// --------------------------
// 1 - push %ebp
// 1 - push %rbp
// 1 - leave
// 1 - ret
// 1 - leaveq
// 1 - retq
// 2 - mov  %esp, %ebp
// 3 - mov  %rsp, %rbp
// ...
// --------------------------
//
// BY: Francisco - gokernel@hotmail.com
//
//-------------------------------------------------------------------
//
#include <stdio.h>
#include <string.h>

#define UCHAR unsigned char

static int    i;
static UCHAR  *o;

// call prototypes:
void call_mov_value_eax (void);
void call_sub_esp (void);
void call_sub_rsp (void);

struct _1OP {
    char  *text;
    UCHAR op[1];
    int   len;
    void (*call) (void);
}_1op[] = {
  #ifdef __x86_64__
  { "push   %rbp", { 0x55 }, 1, NULL },
  { "leaveq",      { 0xc9 }, 1, NULL },
  { "retq",        { 0xc3 }, 1, NULL },
  #else
  { "push   %ebp", { 0x55 }, 1, NULL },
  { "leave",       { 0xc9 }, 1, NULL },
  { "ret",         { 0xc3 }, 1, NULL },
  #endif
  { "mov    %eax, var_?",  { 0xa3 }, 5, NULL }, // 32 BITS:  a3    10 40 40 00  | mov  %eax, 0x404010
  { "mov    $0x3e8, %eax", { 0xb8 }, 5, call_mov_value_eax }, // b8 	e8 03 00 00		|	mov $0x3e8, %eax
  { NULL, " " }
};

struct _2OP {
    char *text;
    UCHAR op[2];
    int len;
    void (*call) (void);
}_2op[] = {
  { "mov    %esp, %ebp", { 0x89, 0xe5 }, 2, NULL},
  { "call   *%eax",      { 0xff, 0xd0 }, 2, NULL },
  { "sub    $0x8, %esp", { 0x83, 0xec }, 3, call_sub_esp },  // 83 ec      08   sub  $0x8,%esp
  { NULL, " " }
};

struct _3OP {
    char *text;
    UCHAR op[3];
    int len;
    void (*call) (void);
}_3op[] = {
  { "mov    %rsp, %rbp",  { 0x48, 0x89, 0xe5 }, 3, NULL },
  { "mov    %eax, var_?", { 0x89, 0x04, 0x25 }, 7, NULL }, // 89 04 25    28 0b 60 00 	| mov  %eax, 0x600b28
  { "sub    $0x8, %rsp",  { 0x48, 0x83, 0xec }, 4, call_sub_rsp }, // 64 BITS: 48 83 ec   08  | sub   $0x8, %rsp
  { NULL, " " }
};

void print_space (int l) {
    int i = 30;
    while (i--) {
        if (l-- < 0) printf (" ");
    }
    printf ("| ");
}

//-------------------------------------------------------------------
//
// This display the opcodes and increment ( i, o )
//
//-------------------------------------------------------------------
//
void print_op_increment (int len) {
    while (len--) {
        printf ("0x%x ", *o);
        i++; o++;
    }
    printf ("\n");
}

void call_sub_esp (void) { // 83 ec   08  | sub  $0x8, %esp
    int len = printf ("%04d: sub    $%d, %%esp", i, (UCHAR)o[2]);
    print_space (len);
}

void call_sub_rsp (void) { // 48 83 ec   08  | sub   $0x8, %rsp
    int len = printf ("%04d: sub    $%d, %%rsp", i, (UCHAR)o[3]);
    print_space (len);
}

void call_mov_value_eax (void) { // b8   e8 03 00 00  |	mov  $0x3e8, %eax
    int len = printf ("%04d: mov    $%d, %%eax", i, *(int*)(o+1));
    print_space (len);
}

int _1 (void) {
    struct _1OP *op = _1op;
    while (op->text) {
        if (op->op[0] == o[0]) {
            if (op->call)
                op->call();
            else {
                int len = printf ("%04d: %s", i, op->text);
                print_space (len);
            }
            print_op_increment (op->len);
            return 1;
        }
        op++;
    }
    return 0;
}

int _2 (void) {
    struct _2OP *op = _2op;
    while (op->text) {
        if (op->op[0] == o[0] && op->op[1] == o[1]) {
            if (op->call)
                op->call();
            else {
                int len = printf ("%04d: %s", i, op->text);
                print_space (len);
            }
            print_op_increment (op->len);
            return 1;
        }
        op++;
    }
    return 0;
}

int _3 (void) {
    struct _3OP *op = _3op;
    while (op->text) {
        if (op->op[0] == o[0] && op->op[1] == o[1] && op->op[2] == o[2]) {
            if (op->call)
                op->call();
            else {
                int len = printf ("%04d: %s", i, op->text);
                print_space (len);
            }
            print_op_increment (op->len);
            return 1;
        }
        op++;
    }
    return 0;
}

void Disasm (UCHAR *code, char *name, int len) {
    o = code;
    i = 0;

    printf ("Disasm(%s) len: %d\n------------------------------\n", name, len);
    for (;;) {
        if (_3()) { }
        else
        if (_2()) { }
        else
        if (_1()) { }
        else {
            printf ("0x%x, ", (UCHAR)*o);
            o++; i++;
        }
        if (i >= len) break;
    }
    printf ("------------------------------\n");
}

int main (void) {
    UCHAR prog [] = {
      0x55,
      #ifdef __x86_64__
      0x48, 0x89, 0xe5,        // mov  %rsp, %rbp
      #else
      0x89, 0xe5,              // mov  %esp, %ebp
      #endif
      0x48, 0x83, 0xec, 0x30,   // sub  $48, %rsp
      0xc9,
      0xc3
    };
    UCHAR s[] = "\x55\x48\x89\xe5\xc9\xc3";

    Disasm (prog, "prog", sizeof(prog));

    // with "string" use: sizeof(string) - 1
    //
    Disasm (s, "s", sizeof(s)-1);

    return 0;
}

