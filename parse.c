//-------------------------------------------------------------------
//
// PARSE EXPRESSION:
//
//   This display assembly ( syntax AT&T ) opcode as result of expression: 
//
// FILH:
//   parse.c
//
// COMPILE:
//   gcc parse.c -o parse -Wall -m32
//
// BY: Fracisco - gokernel@hotmail.com
//
//-------------------------------------------------------------------
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REG_MAX    6
#define TOKEN_SIZE 1024 * 4
#define PROG_SIZE  10000

enum {
    TOK_INT = 255,
    TOK_IF,
    //
    TOK_ID,
    TOK_NUMBER,
    //
    TOK_PLUS_PLUS,  // ++
    TOK_MINUS_MINUS // --
};

enum { EAX = 0, ECX, EDX, EBX, ESI, EDI };

static char *REGISTER [REG_MAX] = { "%eax", "%ecx", "%edx", "%ebx", "%esi", "%edi" };
static int reg = 0;

static int
    tok,
    line,
    erro,
    save_tok,
    save_line
    ;

static char
    *str,
    *save_str,
    token [TOKEN_SIZE + 1],
    save_token [TOKEN_SIZE + 1]
    ;

//-----------------------------------------------
static int  expr0 (void); // primary expression
static void expr1 (void); // '+' '-' : ADDITION | SUBTRACTION
static void expr2 (void); // '*' '/' : MULTIPLICATION | DIVISION
static void expr3 (void); // '('
static void atom  (void); // atom expression
//-----------------------------------------------
static int see (void);

int lex (void) {
    register char *temp = token;
    register int c;

    *temp = 0;

label_top:
    c = *str;

    //#############   REMOVE SPACES  ############
    //
    if (c <= 32) {
        if (c == '\n') {
            line++;
        } else if (c == 0) {
            return (tok = 0);
        }
        str++; // <<<<<<<<<<  imcrement text position  >>>>>>>>>>
        goto label_top;
    }

    //##########  WORD, IDENTIFIER ...  #########
    //
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {

        while (
            *str == '_' ||
            (*str >= 'a' && *str <= 'z') ||
            (*str >= 'A' && *str <= 'Z') ||
            (*str>='0' && *str<='9'))
        {
            *temp++ = *str++;
        }
        *temp = '\0';

        // words:
        if (!strcmp(token, "int")) return (tok = TOK_INT);
        if (!strcmp(token, "if"))  return (tok = TOK_IF);
        // ...

        return (tok = TOK_ID);
    }

    //#################  NUMBER  ################
    //
    if (c >= '0' && c <= '9') {
        while ((*str >= '0' && *str <= '9') || *str == '.')
            *temp++ = *str++;
        *temp = 0;

        return (tok = TOK_NUMBER);
    }

    *temp++ = c;
    *temp = 0;
    // token := "++"
    if (c == '+' && str[1] == '+') { *temp++ = '+'; *temp=0; str += 2; return (tok = TOK_PLUS_PLUS); }
    if (c == '-' && str[1] == '-') { *temp++ = '-'; *temp=0; str += 2; return (tok = TOK_MINUS_MINUS); }
    str++;
    return (tok = c);
}

void lex_save (void) {
    sprintf (save_token, "%s", token);
    save_str  = str;
    save_tok  = tok;
    save_line = line;
}

void lex_restore (void) {
    sprintf (token, "%s", save_token);
    str  = save_str;
    tok  = save_tok;
    line = save_line;
}

void push (void) {
    if (reg < REG_MAX)
        reg++;
}

void pop (void) {
    if (reg > 0)
        reg--;
}

void emit_push_number (int i) {
    printf ("  mov $%d, %s\n", i, REGISTER[reg]);
    push();
}

void emit_push_var (char *name) {
    printf ("  mov %s, %s\n", name, REGISTER[reg]);
    push();
}

void emit_mov_reg_var (int reg_index, char *name) {
    printf ("  mov %s, %s\n", REGISTER[reg_index], name);
}

void emit_add (void) {
    pop();
    printf ("  add %s, %s\n", REGISTER[reg], REGISTER[reg-1]);
}

void emit_mul (void) {
    pop();
    printf ("  imul %s, %s\n", REGISTER[reg], REGISTER[reg-1]);
}

void emit_div (void) {
    pop();
    if (reg == ECX) {
        printf ("  cltd\n");
        printf ("  idiv %s\n", REGISTER[reg]);
    } else {
        printf ("asm Division erro ... please use as first expression !!!\n");
    }
    // 1: value 1 in %eax
    // 2: value 2 in %REGISTER
    // ... DIVIDE ...
    // result in %eax:
}

void emit_sub (void) {
    pop();
    printf ("  sub %s, %s\n", REGISTER[reg], REGISTER[reg-1]);
}

static int expr0 (void) {
    if (tok == TOK_ID) {
        //---------------------------------------
        //
        // Expression type:
        //
        //   i = a * b + c;
        //
        //---------------------------------------
        if (see()=='=') {
            int i = 1;
            char buf[100];
            strcpy(buf, token);
            lex_save (); // save the lexer position
            if (lex() == '=') {
                lex();
                expr1();
                emit_mov_reg_var (EAX, buf);
                return i;
            } else {
                lex_restore (); // restore the lexer position
            }
        }//: if (see(l)=='=')
    }
    expr1 ();
    return -1;
}
static void expr1 (void) { // '+' '-' : ADDITION | SUBTRACTION
    int op;
    expr2();
    while ((op=tok) == '+' || op == '-') {
        lex();
        expr2();
        if (op=='+') emit_add();
        if (op=='-') emit_sub();
    }
}
static void expr2 (void) { // '*' '/' : MULTIPLICATION | DIVISION
    int op;
    expr3();
    while ((op=tok) == '*' || op == '/') {
        lex();
        expr3();
        if (op=='*') emit_mul();
        if (op=='/') emit_div();
    }
}
static void expr3 (void) { // '('
    if (tok=='(') {
        lex();
        expr0();
        if (tok != ')') {
            //Erro("ERRO )\n");
        }
        lex();
    }
    else atom(); // atom:
}
static void atom (void) { // expres
    if (tok == TOK_ID) {
        emit_push_var (token);
        lex();
    }
    else if (tok == TOK_NUMBER) {
        emit_push_number (atoi(token));
        lex();
    }
    else {
        printf ("Erro Line: %d - Expression atom - Ilegal Word (%s)\n", line, token);
    }
}// atom ()

void expression (void) {
    reg = 0; // %eax
    if (expr0() == -1) {
        printf ("\nRegister index(%s) = %d ... Result in register: %%eax\n\n", REGISTER[reg], reg);
    }
}

int stmt (void) {
    lex();
    switch (tok) {
    //case TOK_INT: word_int(); return 1;
    default: expression(); return 1;
    case 0: return 0;
    }
    return 1;
}

int parse (char *text) {
    str = text;
    erro = 0;
    line = 1;
    while (!erro && stmt()) {
        // ... compiling ...
    }
    return erro;
}

static int see (void) {
    char *s = str;
    while (*s) {
        if (*s=='\n' || *s==' ' || *s==9 || *s==13) {
            s++;
        } else {
            if (s[0]=='+' && s[1]=='+') return TOK_PLUS_PLUS;
            if (s[0]=='-' && s[1]=='-') return TOK_MINUS_MINUS;
            return *s;
        }
    }
    return 0;
}

int main (int argc, char **argv) {
    FILE *fp;

    if (argc >= 2 && (fp = fopen (argv[1], "r")) != NULL) {
        char prog [PROG_SIZE + 1];
        int c, i = 0;
        while ((c = fgetc(fp)) != EOF) prog[i++] = c;
        prog[i] = 0;
        fclose (fp);
        if (i >= PROG_SIZE) {
            printf ("\nERRO: Big Text File('%s' := size: %d) > PROG_SIZE: %d\n\n", argv[1], i, PROG_SIZE);
        }
        else parse (prog);
    } else {
    //-------------------------------------------
    // INTERACTIVE MODE:
    //-------------------------------------------

        char string [1024];

        printf ("__________________________________________________________________\n\n");
        printf (" Parse Expression ... to Assembly ( syntax AT&T ):\n\n");
        printf (" Expression Implemented:\n");
        printf ("   = * / + -\n\n");
        printf ("   Type Exemple: 10 * 20 + 3 * 5;\n");
        printf ("   Type Exemple: i = 10 * 20 + 3 * 5;\n\n");
        printf (" To exit type: 'quit' or 'q'\n");
        printf ("__________________________________________________________________\n\n");

        for (;;) {
            printf ("PARSE > ");
            gets (string);

            if (!strcmp(string, "quit") || !strcmp(string, "q")) break;

            if (!strcmp(string, "clear") || !strcmp(string, "cls")) {
                #ifdef WIN32
                system("cls");
                #endif
                #ifdef __linux__
                system("clear");
                #endif
                continue;
            }

            parse (string);
        }
    }

    return 0;
}

