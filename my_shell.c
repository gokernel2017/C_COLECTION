//-------------------------------------------------------------------
//
// part o code: /lib/shell.c
//
//-------------------------------------------------------------------
//

// ...

struct shell *my_shell;

void hello (void) {
    shell_printf (my_shell, "Hello World -> FUNCTION HELLO\n");
}

#define LONG_TO_BYTE(l) l, (l >> 8), (l >> 16), (l >> 24)

static void shell_cb_hello (struct shell *shell, int argc, char *argv[]) {
    long l = *(long*)hello;
    
    my_shell = shell;

    unsigned char code[] = {
    0x55,         // push  %ebp
    0x89, 0xe5,   // mov   %esp,%ebp

    // call hello()
    //-----------------------------
    0xb8, LONG_TO_BYTE(l),
    0xff, 0xd0,
    //-----------------------------

    0xc9,         // leave
    0xc3          // ret
    };

    (void)argc;
    (void)argv;

    //
    // execute the SHELL CODE
    //
    ( (void(*)()) code ) ();
}

static struct shell_cmd shell_default_cmds[] = {
    SHELL_CMD_INITIALIZER("help", shell_cb_help,
                          "help [command]",
                          "obtain help about shell commands"),
    SHELL_CMD_INITIALIZER("history", shell_cb_history,
                          "history",
                          "display history list"),

    SHELL_CMD_INITIALIZER("hello", shell_cb_hello,
                          "Hello World",
                          "display history list"),
};


