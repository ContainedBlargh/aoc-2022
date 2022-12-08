#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint16_t size = 0;
bool **visible = NULL;
uint8_t **forest = NULL;
char buf[128];

static void setup()
{
    visible = calloc(size, sizeof(bool *));
    forest = calloc(size, sizeof(uint8_t *));
    int i;
    for (i = 0; i < size; i++)
    {
        visible[i] = calloc(size, sizeof(bool));
        forest[i] = calloc(size, sizeof(uint8_t));
    }
}

#define max(a, b) a > b ? a : b

#define look_dir(dir, arg, init, cond, ite, elem)                                                                      \
    void look_##dir(uint16_t arg)                                                                                      \
    {                                                                                                                  \
        int8_t prv = -1;                                                                                               \
        uint16_t i;                                                                                                    \
        for (init; cond; ite)                                                                                          \
        {                                                                                                              \
            if (forest elem > prv)                                                                                     \
            {                                                                                                          \
                visible elem = 1;                                                                                      \
                prv = max(forest elem, prv);                                                                           \
            }                                                                                                          \
        }                                                                                                              \
    }

look_dir(east, row, i = 0, i < size, i++, [row][i]);
look_dir(west, row, i = size, i > 0, i--, [row][i - 1]);
look_dir(south, col, i = 0, i < size, i++, [i][col]);
look_dir(north, col, i = size, i > 0, i--, [i - 1][col]);

#define iterate2d(r, c, stmts) \
    for (r = 0; r < size; r++) \
    {\
        for (c = 0; c < size; c++)\
        {\
            stmts;\
        }\
    };

typedef struct _tree
{
    uint8_t height;
    uint16_t row;
    uint16_t col;
} tree;

tree *tall_trees;
uint16_t tall_trees_n;

#define dist_dir(dir, init, cond, ite, elem)\
    uint16_t dist_##dir(tree t)\
    {\
        uint32_t i, d = 0;\
        for (init; cond; ite)\
        {\
            if (forest elem < t.height)\
            {\
                d++;\
            }\
            else if (forest elem == t.height)\
            {\
                d++;\
                break;\
            }\
            else\
            {\
                d++;\
                break;\
            }\
        }\
        return d;\
    }

dist_dir(east, i = t.col, i < size - 1, i++, [t.row][i + 1]);
dist_dir(west, i = t.col, i > 0, i--, [t.row][i - 1]);
dist_dir(south, i = t.row, i < size - 1, i++, [i + 1][t.col]);
dist_dir(north, i = t.row, i > 0, i--, [i - 1][t.col]);

#define score(t) dist_east(t) * dist_west(t) * dist_south(t) * dist_north(t)

int main(void)
{
    uint16_t row = 0, col;
    while (fgets(buf, 128, stdin) != NULL)
    {
        if (forest == NULL)
        {
            size = (uint16_t)(strlen(buf) - 1);
            setup();
        }
        int col;
        for (col = 0; col < size; col++)
        {
            forest[row][col] = buf[col] - '0';
        }
        row++;
    }
    uint16_t i, j;
    for (j = 0; j < size; j++)
    {
        look_east(j);
        look_north(j);
        look_west(j);
        look_south(j);
    }
    uint32_t sum = 0;

    iterate2d(
        row, col, if (visible[row][col]) { sum++; });
    printf("part1: %d\n", sum);

    // Finding good views...
    tall_trees = calloc(size * size, sizeof(tree));
    tall_trees_n = 0;
    iterate2d(row, col, if (visible[row][col]) {tall_trees[tall_trees_n++] = ((tree){.height = forest[row][col], .row = row, .col = col});});
    uint32_t max_score = 0, scene = 0;
    tree t;
    for (i = 0; i < tall_trees_n; i++)
    {
        t = tall_trees[i];
        scene = score(t);
        printf("%d, scoring tree: {h: %hd, r: %hd, c: %hd}, ", scene, t.height, t.row, t.col);
        printf("up: %d, left: %d, right: %d, down: %d, score: %d\n", dist_north(t), dist_west(t), dist_east(t),
               dist_south(t), scene);
        max_score = max(scene, max_score);
    }
    printf("part2: %d\n", max_score);
}