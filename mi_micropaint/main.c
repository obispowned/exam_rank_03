#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_RESET    "\x1b[0m"

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

int            check_negative(t_form *forma)
{
    if (forma->x < 0)
    {
        forma->width -= fabs(forma->x);
        forma->x = 0;
    }
    if (forma->y < 0)
    {
        forma->height -= fabs(forma->y);
        forma->y = 0;
    }
    if (forma->height < 0 || forma->width < 0)
        return (-1); //si llega a este if, no deberia pintar ya que el cuadrado se sale del lienzo
    else
        return(0);
}

void            print_me_p(char **screen, t_form *forma, t_backgound *bg)
{
	int         i;
    int         j;

    i = 0;
    while (i < (int)forma->height)
    {
        j = 0;
        while (j < (int)forma->width)
        {
            if ((i+(int)forma->y < bg->height) && (i+(int)forma->y >= 0) &&
			(j+(int)forma->x < bg->width) && (j+(int)forma->x) >= 0)
			{
                if ((i+forma->y == forma->y) && ((j + forma->x == forma->x) || j+forma->x <= forma->x+forma->width))
					screen[i+(int)forma->y][j+(int)forma->x] = forma->pencil;
				if ((j+forma->x == forma->x) && ((i + forma->y == forma->y) || (i+forma->y <= forma->y+forma->height)))
					screen[i+(int)forma->y][j+(int)forma->x] = forma->pencil;
				if ((i == forma->y+forma->height) && (((j + forma->x) >= (forma->x)) && ((j+forma->x) <= (forma->x+forma->width))))
					screen[i+(int)forma->y][j+(int)forma->x] = forma->pencil;
					//printf("aiudbf iuabfiuoabn\n");*/
				/*if((i == forma->y+forma->height) && (j+forma->x) <= (forma->x + forma->width))
					screen[i+(int)forma->y][j+(int)forma->x] = forma->pencil;*/
				if ((j+forma->x == forma->x+forma->width) && ((i + forma->y == forma->y) || (i+forma->y <= forma->y+forma->height)))
					screen[i+(int)forma->y][j+(int)forma->x] = forma->pencil;
			}	
            j++;
        }
        i++;
    }
}

void            focus_perimetro(char **screen, t_form *forma, t_backgound *bg)
{
    float       i;
    float       j;

    i = forma->x;
    j = forma->y;
    if (check_negative(forma) == 0)
    {
        printf(ANSI_COLOR_GREEN "PINTABLE" ANSI_COLOR_RESET "\n");
        print_me_p(screen, forma, bg);
    }
    else if (check_negative(forma) == -1)
        printf(ANSI_COLOR_RED "NO PINTABLE" ANSI_COLOR_RESET "\n");
    else
        printf("** Error al calcular si es pintable **\n");
}

void            print_me_a(char **screen, t_form *forma, t_backgound *bg)
{
    int         i;
    int         j;

    i = 0;
    while (i < (int)forma->height)
    {
        j = 0;
        while (j < (int)forma->width)
        {
            if (((i+(int)forma->y) < bg->height) && ((j+(int)forma->x) < bg->width))
                screen[i+(int)forma->y][j+(int)forma->x] = forma->pencil;
            j++;
        }
        i++;
    }
}

void            focus_area(char **screen, t_form *forma, t_backgound *bg)
{
    if (check_negative(forma) == 0)
    {
        printf(ANSI_COLOR_GREEN "PINTABLE" ANSI_COLOR_RESET "\n");
        print_me_a(screen, forma, bg);
    }
    else if (check_negative(forma) == -1)
        printf(ANSI_COLOR_RED "NO PINTABLE" ANSI_COLOR_RESET "\n");
    else
        printf("** Error al calcular si es pintable **\n");
}


void            save_forms(FILE *file, char **screen, t_form *forma, t_backgound *bg)
{
    int         ret;

    while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &forma->type_r, &forma->x, &forma->y, &forma->width, &forma->height, &forma->pencil)) == 6)
    {
		if (forma->type_r == 'r')
            focus_perimetro(screen, forma, bg);
        else if (forma->type_r == 'R')
            focus_area(screen, forma, bg);
        else
        {
            printerror("Un error ya que el primer caractwr no es ni r ni R\n");
            exit (0);
        }
    }
    if (ret != 6 && ret != -1) //si no da -1 que es que ya no hay na o 6 que son mas valores, error.
    {
        printf("ERROR: Hay algo mal escrito en el archivo\n");
        exit (0);
    }
    printf("Ret es: |%d|", ret);
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
    save_forms(file, screen, &forma, &bg);
    paint_piece(screen);
    double_kill(screen);
    return (0);
}