//-------------------------------------------------------------------
//
// Text Color C Syntax, using SDL 1.x
//
// FILE:
//   text_color.c
//
// COMPILE:
//   gcc text_color.c -o text_color -lSDL -Wall
//
// USAGE:
//   text_color test.c
//
// MAIN FUNCTION:
//   void Draw (EDITOR *o);
//
// RESERVED WORD COLOR ONLY:
//   void
//   int
//   return
//
//-------------------------------------------------------------------
//
#include <SDL/SDL.h>
#ifdef WIN32
  #undef main
#endif

#define TEXT_SIZE       50000
#define BMP             SDL_Surface
#define COLOR_ORANGE    64512
#define COLOR_WHITE     16777215
#define LINE_DISTANCE   17

// To state of sintax color
// CODE FROM: SciTE100 Editor 1.0
#define STATE_DEFAULT       0
#define STATE_COMMENT       1
#define STATE_LINE_COMMENT  2
#define STATE_DOC_COMMENT   3
#define STATE_NUMBER        4
#define STATE_WORD          5
#define STATE_STRING        6
#define STATE_CHAR          7
#define STATE_PUNCT         8
#define STATE_PRE_PROC      9
#define STATE_OPERATOR      10
#define STATE_IDENTIFIER    11

//-----------------------------------------------
// text color state:
//-----------------------------------------------
//
#define C_DEFAULT               64515 // orange
#define C_COMMENT               63519
#define C_COMMENT_LINE          63518
#define C_STRING                65504
#define C_PRE_PROC              2016
#define C_WORD                  2047

static const unsigned char fixed_font[14][764] = {
  "                                   xx                                                                                                                                                                                                                                                                                                                                                                                                                                                                              xx             xxx                                                                                                                                                                                                                                                      ",
  "                                   xx    xxx                                                                                                                                                                                                                                                                                                                                                                                                                                                                      xxxx             xx                                                                      xx       xx                                                                                                                                                                     ",
  "           xx    xx  xx   xx xx   xxxx  xx xx     xxx      xx       xx    xx                                                 xx    xxxx     xx    xxxx    xxxx    xx     xxxxxx    xxx   xxxxxx   xxxx    xxxx                       xx          xx       xxxx   xxxxxx    xx    xxxxx    xxxx   xxxx    xxxxxx  xxxxxx   xxxx   xx  xx   xxxx       xx  xx  xx  xx      xx   xx xx   xx  xxxx   xxxxx    xxxx   xxxxx    xxxx   xxxxxx  xx  xx  xx  xx  xx   xx xx  xx  xx  xx  xxxxxx   xxxx   xx       xxxx   xx  xx             xx           xx                  xx            xxxx          xx        xx       xx   xx      xxxx                                                             xx                                                        xx     xx     xx     xxx   x   ",
  "          xxxx   xx  xx   xx xx  xx  xx xx xx x  xx xx     xx      xx      xx                                                xx   xx  xx   xxx   xx  xx  xx  xx   xx     xx        xx        xx  xx  xx  xx  xx                     xx            xx     xx  xx xx    xx  xxxx   xx  xx  xx  xx  xx xx   xx      xx      xx  xx  xx  xx    xx        xx  xx  xx  xx      xx   xx xx   xx xx  xx  xx  xx  xx  xx  xx  xx  xx  xx    xx    xx  xx  xx  xx  xx   xx xx  xx  xx  xx      xx   xx     xx         xx                                   xx                  xx           xx             xx                      xx        xx                                                             xx                                                       xx      xx      xx   xx xx xx   ",
  "          xxxx   xx  xx  xxxxxxx xx      xxx xx  xx xx     xx      xx      xx     xx xx    xx                               xx    xx xxx xxxxx   xx  xx  xx  xx   xx xx  xx       xx        xx   xx  xx  xx  xx    xxx     xxx     xx              xx    xx  xx xx    xx xx  xx  xx  xx  xx  xx  xx  xx  xx      xx      xx  xx  xx  xx    xx        xx  xx xx   xx      xxx xxx xxx  xx xx  xx  xx  xx  xx  xx  xx  xx  xx        xx    xx  xx  xx  xx  xx   xx  xx x   xx  xx      xx   xx      xx        xx                            xxxx   xxxxx    xxxx    xxxxx   xxxx    xx      xxxxx  xxxxx   xxxx     xxxx   xx  xx    xx    xxxxxx  xxxxx    xxxx   xxxxx    xxxxx  xx  xx   xxxxx  xxxxxx  xx  xx  xx  xx  xx   xx xx  xx  xx  xx  xxxxxx    xx      xx      xx   x   xxx    ",
  "          xxxx            xx xx   xx        xx    xxx             xx        xx     xxx     xx                               xx    xx xxx    xx       xx      xx   xx xx  xx      xxxxx      xx   xxx xx  xx  xx    xxx     xxx    xx     xxxxxx     xx      xx  xx  xxxx xx  xx  xx  xx  xx      xx  xx  xx      xx      xx      xx  xx    xx        xx  xx xx   xx      xx x xx xxxx xx xx  xx  xx  xx  xx  xx  xx  xx   xx       xx    xx  xx  xx  xx  xx x xx   xx    xx  xx     xx    xx      xx        xx                               xx  xx  xx  xx  xx  xx  xx  xx  xx   xx     xx  xx  xx  xx    xx       xx   xx  xx    xx    xx x xx xx  xx  xx  xx  xx  xx  xx  xx  xx xxx  xx       xx     xx  xx  xx  xx  xx x xx xx  xx  xx  xx      xx    xx      xx      xx              ",
  "           xx             xx xx    xx      xx    xx               xx        xx   xxxxxxx xxxxxx          xxxxxx            xx     xx  xx    xx      xx     xxx    xx xx  xxxxx   xx  xx    xx     xxxx   xx  xx                  xx                  xx    xx   xx xx xx xx  xx  xxxxx   xx      xx  xx  xxxxx   xxxxx   xx      xxxxxx    xx        xx  xxxx    xx      xx x xx xx xxxx xx  xx  xxxxx   xx  xx  xxxxx     xx      xx    xx  xx  xx  xx  xx x xx   xx     xxxx     xx     xx       xx       xx                               xx  xx  xx  xx      xx  xx  xx  xx  xxxxxx  xx  xx  xx  xx    xx       xx   xx xx     xx    xx x xx xx  xx  xx  xx  xx  xx  xx  xx  xxx     xx       xx     xx  xx  xx  xx  xx x xx  xxxx   xx  xx     xx    xx       xx       xx             ",
  "           xx             xx xx     xx    xx     xx xxxx          xx        xx     xxx     xx                              xx     xxx xx    xx     xx        xx  xx  xx      xx  xx  xx    xx    xx xxx   xxxxx                   xx     xxxxxx     xx     xx   xx xx xx xxxxxx  xx  xx  xx      xx  xx  xx      xx      xx xxx  xx  xx    xx        xx  xx xx   xx      xx x xx xx  xxx xx  xx  xx      xx  xx  xx xx      xx     xx    xx  xx  xx  xx  xx x xx  x xx     xx     xx      xx       xx       xx                            xxxxx  xx  xx  xx      xx  xx  xxxxxx   xx     xx  xx  xx  xx    xx       xx   xxxx      xx    xx x xx xx  xx  xx  xx  xx  xx  xx  xx  xx       xxxx    xx     xx  xx  xx  xx  xx x xx   xx    xx  xx    xx    xx        xx        xx            ",
  "                         xxxxxxx     xx  xx xxx  xx  xx           xx        xx    xx xx    xx                             xx      xxx xx    xx    xx     xx  xx  xxxxxxx     xx  xx  xx   xx     xx  xx     xx                     xx              xx           xx  xxxx xx  xx  xx  xx  xx  xx  xx  xx  xx      xx      xx  xx  xx  xx    xx    xx  xx  xx xx   xx      xx   xx xx   xx xx  xx  xx      xx  xx  xx  xx      xx    xx    xx  xx  xx  xx   xx xx  xx  xx    xx    xx       xx        xx      xx                           xx  xx  xx  xx  xx      xx  xx  xx       xx     xx  xx  xx  xx    xx       xx   xx xx     xx    xx x xx xx  xx  xx  xx  xx  xx  xx  xx  xx          xx   xx     xx  xx  xx  xx  xx x xx  xxxx   xx  xx   xx      xx       xx       xx             ",
  "           xx             xx xx  xx  xx  x xx xx xx  xx           xx        xx                     xxx             xxx    xx      xx  xx    xx   xx      xx  xx      xx     xx   xx  xx   xx     xx  xx    xx      xxx     xxx      xx            xx       xx   xx       xx  xx  xx  xx  xx  xx  xx xx   xx      xx      xx  xx  xx  xx    xx    xx  xx  xx  xx  xx      xx   xx xx   xx xx  xx  xx      xx  xx  xx  xx  xx  xx    xx    xx  xx   xxxx    xx xx  xx  xx    xx    xx       xx        xx      xx                           xx  xx  xx  xx  xx  xx  xx  xx  xx       xx     xx  xx  xx  xx    xx       xx   xx  xx    xx    xx x xx xx  xx  xx  xx  xx  xx  xx  xx  xx          xx   xx     xx  xx   xxxx    xx xx  xx  xx  xx  xx  xx        xx      xx      xx              ",
  "           xx             xx xx   xxxx     xx xx  xxx xx           xx      xx                      xxx             xxx   xx        xxxx     xx   xxxxxx   xxxx       xx  xxxx     xxxx    xx      xxxx    xxx      xxx     xxx       xx          xx        xx    xxxxxxx xx  xx  xxxxx    xxxx   xxxx    xxxxxx  xx       xxxxx  xx  xx   xxxx    xxxx   xx  xx  xxxxxx  xx   xx xx   xx  xxxx   xx       xxxx   xx  xx   xxxx     xx     xxxx     xx     xx xx  xx  xx    xx    xxxxxx   xx         xx     xx                            xxxxx  xxxxx    xxxx    xxxxx   xxxx    xx      xxxxx  xx  xx  xxxxxx     xx   xx  xx  xxxxxx  xx   xx xx  xx   xxxx   xxxxx    xxxxx  xx      xxxxx     xxxx   xxxxx    xx     xx xx  xx  xx   xxxx   xxxxxx    xx      xx      xx              ",
  "                                   xx       xxx                    xx      xx                       xx                   xx                                                                                                 xx                                                                                                                                                                              xx                                                                            xx         xx     xx                                                                               xx                     xx                                           xx          xx                                                             xx             xx      xx      xx              ",
  "                                   xx                               xx    xx                       xx                                                                                                                      xx                                                                                                                                                                                xx                                                                           xx                xx                                                                               xx                     xx                                           xx          xx                                                            xx               xx     xx     xx               ",
  "                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          xxxx            xxxx          xxxxxxxx                                                         xxxxx                   xxxx                                            xx          xx                                                         xxxx                       xx                      ",
};
typedef struct {
    char      text [TEXT_SIZE];
    int       line_top;
    int       line_count;
    SDL_Rect  r;
}EDITOR;

char *WORDS[] = {
  "asm", "auto",
  "break",
  "case", "char", "const", "continue",
  "default", "do", "double",
  "else", "enum", "extern",
  "float", "for",
  "goto",
  "if", "int",
  "long",
  "register", "return",
  "short", "signed", "sizeof", "static", "struct", "switch",
  "typedef",
  "union", "unsigned",
  "void", "volatile",
  "while",
  0
};

SDL_Surface *screen;
char *str;
int key;
int state, color;
int is_reserved_word;
int C_RED = 12345;

void DrawChar (BMP *bmp, char ch, int x, int y, int color);

inline char iswordchar (char ch) {
    return isalnum(ch) || ch == '.' || ch == '_';
}

//-------------------------------------------------------------------
// This function is part of Scintilla:
//
// Copyright 1998-1999 by Neil Hodgson <neilh@hare.net.au>
// The License.txt file describes the conditions under which this software may be distributed.
//
// PROJECT  : SciTE100 Editor - 1.0
// FILE     : KeyWords.cc
// FUNCTION : inline bool isoperator(char ch)
//
//-------------------------------------------------------------------
//
inline char isperator (char ch) {

  if (isalnum(ch)) return 0;

	// '.' left out as it is used to make up numbers
  if (ch == '%' || ch == '^' || ch == '&' || ch == '*' ||
     ch == '(' || ch == ')' || ch == '-' || ch == '+' ||
     ch == '=' || ch == '|' || ch == '{' || ch == '}' ||
     ch == '[' || ch == ']' || ch == ':' || ch == ';' ||
     ch == '<' || ch == '>' || ch == ',' || ch == '/' ||
     ch == '?' || ch == '!' || ch == '.' || ch == '~'
  )
	return 1;

	return 0;
}

static void SetTextColor (void) {
    register char ch   = str[0];
    register char next = str[1];

    //---------------------------------------------------------------
    // CODE BASED: SciTE100 Editor 1.0
    //
    //     FILE: KeyWords.cc
    // FUNCTION: static void ColouriseCppDoc ( ... );
    //
    //   state := text_color
    //
    //---------------------------------------------------------------
    //
    if (state == STATE_DEFAULT) {

        color = C_DEFAULT;

        if (ch == '/' && next == '*') {  // COMMENT
            state = STATE_COMMENT; color = C_COMMENT;
        } else if (ch == '/' && next == '/') {  // LINE_COMMENT
            state = STATE_LINE_COMMENT; color = C_COMMENT;
        } else if (ch == '\"') { // STRING
            state = STATE_STRING; color = C_STRING;
        } else if (ch =='\'') { // CHAR
            state = STATE_CHAR; color = C_RED;
        } else if (ch == '#') { // PRE_PROC
            state = STATE_PRE_PROC; color = C_PRE_PROC;
        }
    } else {
        if (state == STATE_PRE_PROC){
            if ((ch == '\r' || ch == '\n') && (str[-1] != '\\')) {
                state = STATE_DEFAULT; color = C_DEFAULT;
            }
            if (ch=='/' && next=='/') {
                state = STATE_LINE_COMMENT; color = C_COMMENT;
            }
        } else if (state==STATE_COMMENT && str[-2]=='*' && str[-1]=='/'){
            state = STATE_DEFAULT; color = C_DEFAULT;
        } else if (state==STATE_LINE_COMMENT && (ch == '\r' || ch == '\n')) {
            state = STATE_DEFAULT; color = C_DEFAULT;
        } else if (state == STATE_STRING && (ch == '"'|| ch=='\n')) {
            state = STATE_DEFAULT; //color = data->color;
        } else if (state == STATE_CHAR && (ch =='\'' || ch=='\n')){
            state = STATE_DEFAULT; color = C_DEFAULT;
        }
    }
}

void Draw (EDITOR *o) {
    int pos_x = o->r.x + 10;
    int pos_y = o->r.y + 10;
    int line_top = 0;

    if (key == SDLK_UP && o->line_top > 0) {
        o->line_top--;
    }
    else
    if (key == SDLK_DOWN && o->line_top < o->line_count-1) {
        o->line_top++;
    }

    state = STATE_DEFAULT;

    // NO DRAW ... find the first line displayed:
    str = o->text;
    while (*str) {
        if (line_top == o->line_top)
      break;
        if (*str == '\n') { // <-- new line
            line_top++;
        }
        SetTextColor();
        str++;
    }

    //
    // HERE DRAW: DrawChar (...);
    //
    SDL_FillRect (screen, &o->r, 8); // bg blue
    while (*str) {
        // size h:
        if (pos_y > (o->r.y + o->r.h)-LINE_DISTANCE)
      break;

        SetTextColor();

        // Draw char in area of editor
        if (pos_x < o->r.x+o->r.w-8) {

            if (state == STATE_DEFAULT) {

                if (isperator(*str)) {
                    color = COLOR_WHITE;
                }
                else
                if (!is_reserved_word && !iswordchar(str[-1])) {

                    // if text[ch] == (First char of RESERVEDs WORDs): [b]reak, [c]ase, [s]truct ... etc
                    if ((*str >= 'a' && *str <= 'g') || *str=='i' || *str=='l' || *str=='o' || (*str >= 'r' && *str <= 'v') || *str=='w') {
                        char *s = str;
                        int count = 0, i;
                        char word [20];
                        while (*s) {
                            word[count++] = *s++;
                            if (!iswordchar(*s) || count > 8) break;
                        }
                        word[count] = 0;
                        for (i = 0; WORDS[i]; i++)
                            if (!strcmp(WORDS[i], word)) {
                                is_reserved_word = count; // <-- HERE: increment from size of WORD.
                          break;
                            }
                    }

                }// if (!is_reserved_word && !iswordchar(str[-1]))
                if (is_reserved_word) {
                    color = C_WORD; // Torn color of sintax
                    is_reserved_word--;
                }

            } // if (state == STATE_DEFAULT)

            DrawChar (screen, *str, pos_x, pos_y, color);
        }
        pos_x += 8;

        if (*str == '\n') {
            pos_x = o->r.x + 10;
            pos_y += LINE_DISTANCE;
        }
        str++;
    }
    SDL_UpdateRect (screen, o->r.x, o->r.y, o->r.w, o->r.h);
}

// color 16
void DrawPixel (BMP *bmp, int x, int y, int color) {
    Uint8 *p;

    // Get clip
    if (
        x < bmp->clip_rect.x ||
        x > bmp->clip_rect.x + bmp->clip_rect.w ||
        y < bmp->clip_rect.y ||
        y >= bmp->clip_rect.y + bmp->clip_rect.h
    )
  return;

    // Here p is the address to the pixel we want to set
    p = (Uint8 *)bmp->pixels + y * bmp->pitch + x * 2;

    *(Uint16 *)p = color;

}// DrawPixel()

// 8 x 13
void DrawChar (BMP *bmp, char ch, int x, int y, int color) {

  if (ch > 32) {
    register unsigned char count;
    register int xx;

      xx = (ch - SDLK_SPACE) * 8;

      // insert color
      for (count=0; count < 8; count++) {
          if ( fixed_font[ 0][xx] == 'x' ) { DrawPixel(bmp, x, y+0,  color); }
          if ( fixed_font[ 1][xx] == 'x' ) { DrawPixel(bmp, x, y+1,  color); }
          if ( fixed_font[ 2][xx] == 'x' ) { DrawPixel(bmp, x, y+2,  color); }
          if ( fixed_font[ 3][xx] == 'x' ) { DrawPixel(bmp, x, y+3,  color); }
          if ( fixed_font[ 4][xx] == 'x' ) { DrawPixel(bmp, x, y+4,  color); }
          if ( fixed_font[ 5][xx] == 'x' ) { DrawPixel(bmp, x, y+5,  color); }
          if ( fixed_font[ 6][xx] == 'x' ) { DrawPixel(bmp, x, y+6,  color); }
          if ( fixed_font[ 7][xx] == 'x' ) { DrawPixel(bmp, x, y+7,  color); }
          if ( fixed_font[ 8][xx] == 'x' ) { DrawPixel(bmp, x, y+8,  color); }
          if ( fixed_font[ 9][xx] == 'x' ) { DrawPixel(bmp, x, y+9,  color); }
          if ( fixed_font[10][xx] == 'x' ) { DrawPixel(bmp, x, y+10, color); }
          if ( fixed_font[11][xx] == 'x' ) { DrawPixel(bmp, x, y+11, color); }
          if ( fixed_font[12][xx] == 'x' ) { DrawPixel(bmp, x, y+12, color); }
          if ( fixed_font[13][xx] == 'x' ) { DrawPixel(bmp, x, y+13, color); }
          xx++; x++;
      }
  }
}

void Init (EDITOR *o) {
    char *s;

    SDL_Init (SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode (800,600,16,0);
    SDL_EnableUNICODE (1);
    SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL); // For keypressed

    o->line_top = 0;
    o->line_count = 0;
    o->r.x = 10;
    o->r.y = 10;
    o->r.w = 640;
    o->r.h = 480;

    /* get: o->line_count */
    //
    s = o->text;
    while (*s) {
        if (*s == '\n') { // <-- new line
            o->line_count++;
        }
        s++;
    }
    SDL_FillRect (screen, &(SDL_Rect){ 9, 9, 642, 482}, COLOR_ORANGE);
    Draw (o);
}

void Run (EDITOR *o) {
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if ((key = e.key.keysym.unicode)==0)
                    key = e.key.keysym.sym;
                if (key == SDLK_ESCAPE)
                    quit = 1;
                if (key == SDLK_UP || key == SDLK_DOWN)
                    Draw (o);
            }// if (e.type == SDL_KEYDOWN)
        }// while (SDL_PollEvent(&e))
        SDL_Delay (10);
    }
    SDL_Quit ();
}

int main (int argc, char **argv) {
    FILE *fp;
    EDITOR o;

    if (argc >= 2 && (fp = fopen (argv[1], "r")) != NULL) {
        int c, i = 0;
        while ((c = fgetc(fp)) != EOF) {
            o.text[i++] = c;
            if (i >= TEXT_SIZE) break;
        }
        o.text[i] = 0;
        fclose (fp);
        Init (&o);
    } else {
        printf ("USAGE: %s <file.c>\n", argv[0]);
        return -1;
    }
    SDL_Flip (screen);
    Run (&o);
    return 0;
}

