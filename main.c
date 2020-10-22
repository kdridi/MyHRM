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

typedef enum e_value_type t_value_type;

enum e_value_type
{
    CHARACTER,
    INTEGER,
};

typedef struct s_value t_value;

struct s_value
{
    t_value_type type;
    int integer;
    char character;
};

typedef struct s_conveyor t_conveyor;

struct s_conveyor
{
    size_t length;
    t_value array[256];
};


// ajouter en queue : push
// retire en queue : pop
// ajouter en tete : unshift
// retire en tete : shift

void conveyor_initalize(t_conveyor *c)
{
    c->length = 0;
}

void conveyor_print(t_conveyor *c)
{
    for (size_t i = 0; i < c->length; i++)
    {
        switch (c->array[i].type) {
            case INTEGER:
                printf("%d\n", c->array[i].integer);
                break;
            case CHARACTER:
                printf("%c\n", c->array[i].character);
                break;
            default:
                break;
        }
    }
}

void conveyor_push(t_conveyor *c, t_value *value)
{
    c->array[c->length].type = value->type;
    c->array[c->length].character = value->character;
    c->array[c->length].integer = value->integer;
    c->length = c->length + 1;
}

void conveyor_pop(t_conveyor *c, t_value *value)
{
    c->length = c->length - 1;
    value->type = c->array[c->length].type;
    value->character = c->array[c->length].character;
    value->integer = c->array[c->length].integer;
}

void conveyor_unshift(t_conveyor *c, t_value *value)
{
    // [3, 7, 9]  ajoute 1 => [_ , 3, 7, 9]
    memmove(c->array + 1, c->array + 0, c->length * sizeof(c->array[0]));
    c->array[0].type = value->type;
    c->array[0].character = value->character;
    c->array[0].integer = value->integer;
    c->length = c->length + 1;
}

void conveyor_shift(t_conveyor *c, t_value *value)
{
    value->type = c->array[0].type;
    value->character = c->array[0].character;
    value->integer = c->array[0].integer;

    c->length = c->length - 1;
    memmove(c->array + 0, c->array + 1, c->length * sizeof(c->array[0]));
}

void conveyor_push_integer(t_conveyor *c, int integer)
{
    t_value value = { .type = INTEGER, .integer = integer };
    conveyor_push(c, &value);
}

void conveyor_push_character(t_conveyor *c, char character)
{
    t_value value = { .type = CHARACTER, .character = character };
    conveyor_push(c, &value);
}

t_conveyor in;
t_conveyor out;
t_value player;
t_value ground[3];
bool is_holding;

void game_print()
{
    printf("IN:\n");
    conveyor_print(&in);

    printf("OUT:\n");
    conveyor_print(&out);
    printf("###################\n");
}

enum {
    INBOX,
    OUTBOX,
    JUMP,
    COPYFROM,
    COPYTO,
    ADD,
};

void game_execute(int *program, size_t program_size)
{
    t_value *value;
    is_holding = false;
    int pc = 0;
    while (pc < program_size)
    {
        int command = program[pc];
        
        switch (command) {
            case INBOX:
                if (in.length == 0)
                    return;
                conveyor_shift(&in, &player);
                is_holding = true;
                break;
            case OUTBOX:
                is_holding = false;
                conveyor_unshift(&out, &player);
                break;
            case JUMP:
                pc += program[pc + 1];
                break;
            case COPYFROM:
                value = ground + program[++pc];
                player.type = value->type;
                player.integer = value->integer;
                player.character = value->character;
                is_holding = true;
                break;
            case COPYTO:
                value = ground + program[++pc];
                value->type = player.type;
                value->integer = player.integer;
                value->character = player.character;
                break;
            case ADD:
                value = ground + program[++pc];
                player.integer += value->integer;
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
        COPYTO, 0,
        INBOX,
        ADD, 0,
        OUTBOX,
        JUMP, -8,
    };
    int program_size = sizeof(program) / sizeof(program[0]);

    conveyor_initalize(&in);
    conveyor_initalize(&out);
    
    conveyor_push_integer(&in, 7);
    conveyor_push_integer(&in, 9);
    conveyor_push_integer(&in, 9);
    conveyor_push_integer(&in, 6);
    conveyor_push_integer(&in, -1);
    conveyor_push_integer(&in, 7);
    conveyor_push_integer(&in, 9);
    conveyor_push_integer(&in, -6);

    game_print();
    game_execute(program, program_size);
    game_print();

    return (0);
}
