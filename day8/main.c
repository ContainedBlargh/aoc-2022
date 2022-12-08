#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BSZ 65535

size_t size = 0;
bool **visible = NULL;
uint8_t **forest = NULL;
char buf[BSZ];

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
    void look_##dir(size_t arg)                                                                                      \
    {                                                                                                                  \
        int8_t prv = -1;                                                                                               \
        size_t i;                                                                                                    \
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

#define iterate2d(r, c, stmts)                                                                                         \
    for (r = 0; r < size; r++)                                                                                         \
    {                                                                                                                  \
        for (c = 0; c < size; c++)                                                                                     \
        {                                                                                                              \
            stmts;                                                                                                     \
        }                                                                                                              \
    };

typedef struct _tree
{
    uint8_t height;
    size_t row;
    size_t col;
} tree;

tree *tall_trees;
size_t tall_trees_n;

#define dist_dir(dir, init, cond, ite, elem)                                                                           \
    size_t dist_##dir(tree t)                                                                                        \
    {                                                                                                                  \
        size_t i, d = 0;                                                                                             \
        for (init; cond; ite)                                                                                          \
        {                                                                                                              \
            if (forest elem < t.height)                                                                                \
            {                                                                                                          \
                d++;                                                                                                   \
            }                                                                                                          \
            else if (forest elem == t.height)                                                                          \
            {                                                                                                          \
                d++;                                                                                                   \
                break;                                                                                                 \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                d++;                                                                                                   \
                break;                                                                                                 \
            }                                                                                                          \
        }                                                                                                              \
        return d;                                                                                                      \
    }

dist_dir(east, i = t.col, i < size - 1, i++, [t.row][i + 1]);
dist_dir(west, i = t.col, i > 0, i--, [t.row][i - 1]);
dist_dir(south, i = t.row, i < size - 1, i++, [i + 1][t.col]);
dist_dir(north, i = t.row, i > 0, i--, [i - 1][t.col]);

#define score(t) dist_east(t) * dist_west(t) * dist_south(t) * dist_north(t)

int cmp(const void *p, const void *q)
{
    tree a = *((tree *)p);
    tree b = *((tree *)q);
    if (a.height > b.height)
    {
        return -1;
    }
    else if (a.height < b.height)
    {
        return 1;
    }
    return 0;
}

#define VIS
#undef VIS

int main(void)
{
    size_t row = 0, col;
    while (fgets(buf, BSZ, stdin) != NULL)
    {
        if (forest == NULL)
        {
            size = (size_t)(strlen(buf) - 1);
            setup();
        }
        int col;
        for (col = 0; col < size; col++)
        {
            forest[row][col] = buf[col] - '0';
        }
        row++;
    }
#ifdef VIS
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
#endif

    size_t i, j;
    for (j = 0; j < size; j++)
    {
        look_east(j);
        look_north(j);
        look_west(j);
        look_south(j);
#ifdef VIS
        addstr("\r");
        clear();
        for (row = 0; row < size; row++)
        {
            if (row == 0)
            {
                addstr("\n\r");
            }
            for (col = 0; col < size; col++)
            {
                if (row == j || col == j)
                {
                    attrset(COLOR_PAIR(1));
                }
                if (row <= j && col <= j)
                {
                    addch(' ');
                    addch(visible[row][col] + '0');
                    addch(' ');
                }
                else
                {
                    attrset(COLOR_PAIR(2));
                    addch(' ');
                    addch(forest[row][col] + '0');
                    addch(' ');
                    attroff(COLOR_PAIR(2));
                }
                if (row == j || col == j)
                {
                    attroff(COLOR_PAIR(1));
                }
            }
            addstr("\n");
            if (row == size - 1)
            {
                addstr("\n\r");
            }
        }
        usleep(166666);
        refresh();
#endif
    }
#ifdef VIS
    usleep(166666 * 2);
    refresh();
    endwin();
#endif
    size_t sum = 0;
    iterate2d(row, col, if (visible[row][col]) { sum++; });
    printf("part1: %ld\n", sum);
    // Finding good views...
    tall_trees = calloc(size * size, sizeof(tree));
    tall_trees_n = 0;
    iterate2d(row, col, if (visible[row][col]) {tall_trees[tall_trees_n++] = ((tree){.height = forest[row][col], .row = row, .col = col});});
    qsort(tall_trees, tall_trees_n, sizeof(tree), cmp);
    size_t max_score = 0, scene;
    uint8_t prv = 0;
    tree t;
    for (i = 0; i < tall_trees_n; i++)
    {
        t = tall_trees[i];
        scene = score(t);
        if (max_score < scene) {
            max_score = scene;
            prv = t.height;
        }
        if (t.height < prv) {
            break;
        }
    }
    double p1 = ((double)(i))/((double)tall_trees_n) * 100.0f;
    double p2 = ((double)(i))/((double)size * size) * 100.0f;
    printf("part2: %ld, computed scenic scores for %ld/%ld (%0.02f%%) tall trees.\nThat's only %ld/%ld (%0.02f%%) of the trees!\n", max_score, i, tall_trees_n, p1, i, size * size, p2);
}