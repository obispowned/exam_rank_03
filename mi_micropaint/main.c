#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_background
{
    char       typr_bg;
    int        width;
    int        height;
}               t_backgound;

typedef struct s_form
{
    char        type_r;
    float       x;
    float       y;
    float       width;
    float       height;
    char        pencil;
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

    if ((ret = fscanf(file, "%d %d %c\n", &bg->width, &bg->height, &bg->typr_bg) != 3))
        return (0);
    if (bg->width < 1 || bg->width > 300 || bg->height < 1|| bg->height > 300) 
        return (0);
    return (1);
}

int     paint_piece(char **screen)
{
    int i, j;

    i = 0;

    while (screen[i])
    {
        j = 0;
        while (screen[i][j] != '\0')
        {
            write(1, &screen[i][j], 1);
            j++;
        }
        write(1,"\n",1);
        i++; 
    }
    return(1);
}

void            double_kill(char **str)
{
    int         i;

    i = 0;
    while (str[i])
    {
        free(str[i]);
        i++;
    }
    free(str);
}

void            focus_perimetro()
{

}

void            focus_area()
{

}

void            save_forms(FILE *file, char **screen, t_form *forma)
{
    int         ret;

    while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &forma->type_r, &forma->x, &forma->y, &forma->width, &forma->height, &forma->pencil)) == 6)
    {
		/**/
		printf("Ret es: |%d|\n", ret);
		printf("\n|%c-%f-%f-%f-%f-%c|\n", forma->type_r, forma->x, forma->y, forma->width, forma->height, forma->pencil);
        printerror("Un error al leer del archivo\n");
		exit(0);
    }
	printf("\n|%c-%f-%f-%f-%f-%c|\n", forma->type_r, forma->x, forma->y, forma->width, forma->height, forma->pencil);
    if (forma->type_r == 'r')
        focus_perimetro();
    else if (forma->type_r == 'R')
        focus_area();
    else
    {
        printerror("Un error ya que el primer caractwr no es ni r ni R\n");
    }
}

int             main(int argc, char **argv)
{
    FILE        *file;
    t_backgound bg;
	t_form		forma;
    char        **screen;
    int         i, j;

    if (argc != 2)
	{
        printerror("Error: Argument\n");
		return(0);
	}
    if (!(file = fopen(argv[1], "r")))
        printerror("Error: File Corrupted\n");
    if (get_backgound(file, &bg) == 0)
	{
        printerror("Error: File Corrupted\n");
		return (0);
	}
    printf("aqui hay un with de %d, y height de %d\n", bg.width, bg.height);
    screen = (char **)malloc(sizeof(char *) * bg.height + 1);
	i = 0;
    while(i < bg.height)
    {
        screen[i] = (char *)malloc(sizeof(char) * bg.width + 1);
		j = 0;
		while (j < bg.width)
		{
			screen[i][j] = bg.typr_bg;
			j++;
		}
		screen[i][j] = '\0';
       i++;
    }
	screen[i] = 0;
    save_forms(file, screen, &forma);
    /*AQUI FALTA DE METER UN BUCLE CON SCANF PARA CUALQUIER NUM DE FIGURAS*/
    paint_piece(screen);
    double_kill(screen);
    return (0);
}