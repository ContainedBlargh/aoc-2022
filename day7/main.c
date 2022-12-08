#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct file
{
    char name[34];
    size_t size;
} file;

typedef struct _dir dir;

typedef struct _dir
{
    char name[34];
    file files[64];
    uint8_t files_n;
    dir *subdirs[64];
    uint8_t subdirs_n;
    dir *parent;
    size_t size;
} dir;

dir *handle_cd(dir *current, char *dst)
{
    if (strncmp(dst, "..", 2) == 0)
    {
        return current->parent;
    }
    else
    {
        dir *out = malloc(sizeof(dir));
        strcpy(out->name, dst);
        out->files_n = 0;
        out->subdirs_n = 0;
        if (current != NULL)
        {
            current->subdirs[current->subdirs_n++] = out;
        }
        out->parent = current;
        return out;
    }
}

size_t compute_size_leq(size_t *acc, dir *current, size_t max)
{
    int i;
    size_t own_size = 0;
    for (i = 0; i < current->files_n; i++)
    {
        own_size += current->files[i].size;
    }
    if (current->subdirs_n > 0)
    {
        for (i = 0; i < current->subdirs_n; i++)
        {
            own_size += compute_size_leq(acc, current->subdirs[i], max);
        }
    }
    current->size = own_size;
    if (own_size <= max)
    {
        (*acc) += own_size;
    }
    return own_size;
}

size_t dirs_n = 0;
void flatten(dir **dirs, dir *current)
{
    int i;
    dirs[dirs_n++] = current;
    for (i = 0; i < current->subdirs_n; i++)
    {
        flatten(dirs, current->subdirs[i]);
    }
}

static int dircmp(const void *p1, const void *p2)
{
    dir *a = *((dir **)p1);
    dir *b = *((dir **)p2);
    if (a->size > b->size)
    {
        return 1;
    }
    if (a->size < b->size)
    {
        return -1;
    }
    return 0;
}

int main(void)
{
    char buf[36], name[33];
    size_t size, size_sum = 0;
    dir *root = NULL;
    dir *current = NULL;
    while (fgets(buf, 64, stdin))
    {
        if (strncmp(buf, "$ cd", 4) == 0)
        {
            strcpy(name, buf + 5);
            name[strlen(name) - 1] = '\0';
            current = handle_cd(current, name);
            if (root == NULL)
            {
                root = current;
            }
            continue;
        }
        if (strncmp(buf, "$ ls", 4) == 0)
        {
            continue;
        }
        if (strncmp(buf, "dir", 3) == 0)
        {
            continue;
        }
        if (sscanf(buf, "%ld %s", &size, name) > 0)
        {
            strcpy(name, current->files[current->files_n].name);
            current->files[current->files_n].size = size;
            current->files_n++;
        }
    }
    compute_size_leq(&size_sum, root, 100000);
    printf("All files under 100K combined: %ld\n", size_sum);
    dir **dirs = calloc(1024, sizeof(dir *));
    flatten(dirs, root);
    printf("Read %ld directories\n", dirs_n);
    int i;
    size_t used_space = root->size, free_space = 0, required_space;
    printf("Total used space: (%ld/%ld)\n", used_space, 70000000L);
    free_space = 70000000L - used_space;
    printf("Free space: (%ld)\n", free_space);
    required_space = 30000000L - free_space;
    printf("Required space: (%ld)\n", required_space);
    qsort(dirs, dirs_n, sizeof(dir *), dircmp);
    for (i = 0; i < dirs_n; i++)
    {
        if (dirs[i]->size >= required_space)
        {
            printf("Delete directory '%s'(%ld)\n", dirs[i]->name, dirs[i]->size);
            break;
        }
    }
    return 0;
}