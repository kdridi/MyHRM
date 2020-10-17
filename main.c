//
//  main.c
//  MyHRM
//
//  Created by Karim DRIDI on 17/10/2020.
//

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum e_print_mode t_print_mode;

enum e_print_mode
{
    MODE_INT,
    MODE_CHAR,
};

enum {
    INBOX,
    OUTBOX,
    JUMP,
};

typedef struct s_conveyor t_conveyor;

struct s_conveyor
{
    size_t length;
    int array[256];
};


// ajouter en queue : push
// retire en queue : pop
// ajouter en tete : unshift
// retire en tete : shift

void conveyor_initalize(t_conveyor *c)
{
    c->length = 0;
}

void conveyor_print(t_conveyor *c, t_print_mode mode)
{
    for (size_t i = 0; i < c->length; i++)
    {
        switch (mode) {
            case MODE_INT:
                printf("%d\n", c->array[i]);
                break;
            case MODE_CHAR:
                printf("%c\n", c->array[i]);
                break;
            default:
                break;
        }
    }
}

void conveyor_push(t_conveyor *c, int value)
{
    c->array[c->length] = value;
    c->length = c->length + 1;
}

int conveyor_pop(t_conveyor *c)
{
    c->length = c->length - 1;
    return c->array[c->length];
}

void conveyor_unshift(t_conveyor *c, int value)
{
    // [3, 7, 9]  ajoute 1 => [_ , 3, 7, 9]
    memmove(c->array + 1, c->array + 0, c->length * sizeof(int));
    c->array[0] = value;
    c->length = c->length + 1;
}

int conveyor_shift(t_conveyor *c)
{
    int result = c->array[0];
    c->length = c->length - 1;
    memmove(c->array + 0, c->array + 1, c->length * sizeof(int));
    return (result);
}

t_conveyor in;
t_conveyor out;
int player;
bool is_holding;

void game_print(t_print_mode mode)
{
    printf("IN:\n");
    conveyor_print(&in, mode);

    printf("OUT:\n");
    conveyor_print(&out, mode);
    printf("###################\n");
}

void game_execute(int *program, size_t program_size)
{
    is_holding = false;
    int pc = 0;
    while (pc < program_size && (in.length > 0 || is_holding == true))
    {
        int command = program[pc];
        
        switch (command) {
            case INBOX:
                player = conveyor_shift(&in);
                is_holding = true;
                break;
            case OUTBOX:
                is_holding = false;
                conveyor_unshift(&out, player);
                break;
            case JUMP:
                pc += program[pc + 1];
                break;
            default:
                printf("Command not found!");
                abort();
                break;
        }
        pc = pc + 1;
    }
}

int main(void)
{
    int program[] = {
        INBOX,
        OUTBOX,
        JUMP, -3,
    };
    int program_size = sizeof(program) / sizeof(program[0]);

    conveyor_initalize(&in);
    conveyor_initalize(&out);

    conveyor_push(&in, 'i');
    conveyor_push(&in, 'n');
    conveyor_push(&in, 'i');
    conveyor_push(&in, 't');
    conveyor_push(&in, 'i');
    conveyor_push(&in, 'a');
    conveyor_push(&in, 'l');
    conveyor_push(&in, 'i');
    conveyor_push(&in, 'z');
    conveyor_push(&in, 'e');

    game_print(MODE_CHAR);
    game_execute(program, program_size);
    game_print(MODE_CHAR);

    return (0);
}
