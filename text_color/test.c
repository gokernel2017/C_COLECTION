//-------------------------------------------------------------------
//
// Simple Object Example:
//
// Create a simple object and enable the CallBack in ( proc_simple ):
//   01: MSG_MOUSE_DOWN
//   02: MSG_MOUSE_UP
//   03: MSG_ENTER
//   04: MSG_LEAVE
//
// FILE:
//   simple_object.c
//
// COMPILE:
//   gcc simple.c -o simple libsgui.a -lSDL -Wall
//
//-------------------------------------------------------------------
//
#include "src/app.h"

#define ID_SIMPLE   10000

/*
typedef struct {
    char  *text;
    int   fg;
    int   bg;
}DATA_SIMPLE;
*/


//
// The Procedure:
//
int proc_simple (OBJECT *o, int msg, int value) {

    switch (msg) {

printf ("Hello World /* inicio */ final\n");

    case MSG_DRAW: {
        DATA_SIMPLE *data = app_GetData (o);
        SDL_Rect r;

        app_GetRect (o, &r);

        DrawRect (screen, r.x, r.y, r.w-1, r.h-1, data->fg); // border
        SDL_FillRect (screen, &(SR){ r.x+1, r.y+1, r.w-2, r.h-2}, data->bg); // bg Hello_World_testando

        // on mouse top
        if (value)
            DrawRect (screen, r.x+1, r.y+1, r.w-3, r.h-3, data->fg);

        int x = r.x + r.w / 2 - ((strlen(data->text)*8)/2);
        DrawText (screen, data->text, x, (r.y+r.h/2)-6, data->fg);

        } break;

    case MSG_MOUSE_DOWN:
    case MSG_MOUSE_UP:
        return RET_CALL; // enable to callback

    case MSG_FOCUS:
        return 0; // object no focused

    case MSG_ENTER:
    case MSG_LEAVE:
        //return 1;         // enable to REdraw
        return RET_CALL;  // enable to callback

    }// switch (msg)

    return 0;
}

OBJECT * my_SimpleObject (OBJECT *parent, int id, int x, int y, char *text) {
    OBJECT *o;
    DATA_SIMPLE *data;

    if ((data = (DATA_SIMPLE*)malloc(sizeof(DATA_SIMPLE))) == NULL)
  return NULL;

    if (text)
        data->text = strdup (text);
    else
        data->text = strdup ("Simple");
    data->fg = COLOR_ORANGE;
    data->bg = COLOR_WHITE;

    o = app_ObjectNew (proc_simple, x, y, 150, 150, id, OBJECT_TYPE_OBJECT, data);

    app_ObjectAdd (parent, o);

    return o;
}

void call (ARG *arg) {
    static int count = 0;

    switch (arg->msg) {
    case MSG_ENTER:
        printf ("MSG_ENTER: %d\n", count++);
        break;
    case MSG_LEAVE:
        printf ("MSG_LEAVE: %d\n", count++);
        break;
    case MSG_MOUSE_DOWN:
        printf ("MSG_MOUSE_DOWN: %d\n", count++);
        break;
    case MSG_MOUSE_UP:
        printf ("MSG_MOUSE_UP: %d\n", count++);
        break;
    }
}

int main (int argc, char **argv) {
    if (app_Init(argc,argv)) {

        OBJECT * o = my_SimpleObject (NULL, ID_SIMPLE, 100, 20, "Simple Object");

        app_SetCall (o, call);

        app_Run (NULL);
    }
    return 0;
}
