#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_background
{
    char       typr_bg;
    int        width;
    int        height;
}               t_backgound;

typedef struct s_form
{
    char        type_f;
    float       x;
    float       y;
    float       width;
    float       height;
}               t_form;


int             ft_strlen(char *str)
{
    int         i;

    while (str[i] != '\0')
        i++;
    return (i);
}

int             printerror(char *str)
{
    write(1, str, ft_strlen(str));
    return(1);
}

int             get_backgound(FILE *file, t_backgound *bg)
{ //Sacamos el width y height del backgound y lo validamos
    int         ret;

    if ((ret = fscanf(file, "%d %d %c", &bg->width, &bg->height, &bg->typr_bg) != 3))
        return (0);
    if (bg->width < 1 || bg->width > 300 || bg->height < 1|| bg->height > 300) 
        return (0);
    return (1);
}

int             save_bg(char **screen, t_backgound *bg)
{
    int         i, j;

    i = 0;
    while (i < bg->height)
    {
        j = 0;
        while (j < bg->width)
        {
            screen[i][j] = bg->typr_bg;
            j++;
        }
        screen[i][j] = '\0';
        i++; 
    }
    screen[i] = NULL;
    return (1);
}

int     paint_piece(char **screen)
{
    int i, j;

    i = 0;
    while (screen[i])
    {
        j = 0;
        while (screen[i][j])
        {
            write(1, &screen[i][j],1);
            j++;
        }
        write(1,"\n",1);
        i++; 
    }

    return(1);
}

int             main(int argc, char **argv)
{
    FILE        *file;
    t_backgound bg;
    char        **screen;
    int         i;

    if (argc != 2)
        printerror("Error: Argument\n");
    if (!(file = fopen(argv[1], "r")))
        printerror("Error: File Corrupted\n");
    if (!get_backgound(file, &bg))
        printerror("Error: File Corrupted\n");
    printf("aqui hay un with de %d, y height de %d\n", bg.width, bg.height);
    screen = (char **)malloc(sizeof(char *) * bg.height + 1);
    while( i < bg.width)
    {
        screen[i] = (char *)malloc(sizeof(char) * bg.width + 1);
       i++;
    }
    save_bg(screen , &bg);
    /*AQUI FALTA DE METER UN BUCLE CON SCANF PARA CUALQUIER NUM DE FIGURAS*/
    paint_piece(screen);
    return (0);
}