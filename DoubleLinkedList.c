//-------------------------------------------------------------------
//
// Double Linked List:
//
//-------------------------------------------------------------------
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct LIST LIST;
typedef struct ITEN ITEN;

struct LIST {
    ITEN  *iten_first;
    ITEN  *iten_last;
};

struct ITEN {
    char  *text;
    ITEN  *prev;
    ITEN  *next;
};

LIST *new_List (void) {
    LIST *list;
    if ((list = (LIST*)malloc(sizeof(LIST))) != NULL) {
        list->iten_first = NULL;
        list->iten_last = NULL;
    }
    return list;
}

//
// Add Itens in THE END of List:
//
void add (LIST *list, char *text) {
    ITEN *iten;
    if (list && text && (iten = (ITEN*)malloc(sizeof(ITEN))) != NULL) {
        if ((iten->text = strdup (text)) != NULL) {
            iten->prev = NULL;
            iten->next = NULL;
            // first iten
            if (list->iten_first == NULL) {
                list->iten_first = iten;
                list->iten_last = iten;
            } else {
                iten->prev = list->iten_last;
                list->iten_last->next = iten;
                list->iten_last = iten;
            }
        }
    }
}

void list_Clear (LIST *list) {
    if (list) {
        while (list->iten_first) {
            ITEN *info = list->iten_first->next;
            if (list->iten_first->text) {
                free (list->iten_first->text);
            }
            free (list->iten_first);
            list->iten_first = info;
        }
        list->iten_first = NULL;
        list->iten_last = NULL;
    }
}

//
// Display the List Itens in ORDER:
//
void print_First (LIST *list) {
    if (list) {
        ITEN *i = list->iten_first;
        printf ("\nCrescente:\n");
        while (i) {
            printf ("Text: %s\n", i->text);
            i = i->next;
        }
    }
}

//
// Display the List Itens in REVERSE ORDER:
//
void print_Last (LIST *list) {
    if (list) {
        ITEN *i = list->iten_last;
        printf ("\nDECRESCENTE:\n");
        while (i) {
            printf ("Text: %s\n", i->text);
            i = i->prev;
        }
    }
}

int main (void) {
    LIST *my_list;
    if ((my_list = new_List())) {
        add (my_list, "0: ZERO");
        add (my_list, "1: Um");
        add (my_list, "2: Segundo");
        add (my_list, "3: Terceiro");
        add (my_list, "4: Quarto");
        add (my_list, "5: QUINTO - ULTIMO");

        print_First(my_list);
        print_Last (my_list);

        list_Clear (my_list);

        free (my_list);
        my_list = NULL;
    }
    return 0;
}

