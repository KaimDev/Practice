#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct Land Land;
typedef struct Departament Departament;
typedef struct Data Data;
typedef enum Antennas Antennas;

enum Antennas
{
    a = 1,  //45500m2
    b,      //16700m3
    c,      //27800m2
    d,      //7600m2
    e       //13800m2
};

struct Land
{
    size_t m2;
    size_t previus_antennas;  //2300m2
    size_t new_antennas;
    Antennas type;
};

struct Departament
{
    Land *land;
    size_t count_land;
};

struct Data
{
    Departament *departament;
    size_t size;
};

void Menu()
{
    puts("Calculate the number of remaining antennas   [1]");
    puts("List departments with the number of antennas [2]");
    puts("Estimated list of how many antennas exist    [3]");
}

void Calculate();
void List();
void Porcentage();

int32_t main(void)
{
    Data data;
    size_t count;
    int32_t option;

    printf("Numbers of departaments\n> ");
    scanf("%lu", &count);

    data.departament = malloc(sizeof(Departament) * count);
    data.size = count;

    printf("Count of lands\n> ");
    scanf("%lu", &count);

    data.departament->land = malloc(sizeof(Land) * count);
    data.departament->count_land = count;

    for (size_t i = 0 ; i < data.departament->count_land ; ++i)
    {
        do
        {
            if (data.departament->land->m2 < 0)
            {
                puts("Negative numbers are not allowed");
            }

            printf("Land size [%lu]\n> ", (i+1));
            scanf("%lu", &data.departament->land->m2);

        } while (data.departament->land->m2 < 0);


        printf("Enter the number of antennas previously installed\n> ");
        scanf("%lu", &data.departament->land->previus_antennas);

        if (data.departament->land->previus_antennas < 0)
        {
            data.departament->land->previus_antennas = 0;
        }
    }

    system("clear");

    Menu();
    scanf("> %d", &option);

    if (option == 1)
        Calculate();

    if (option == 2)
        List();

    if (option == 3)
        Porcentage();


    free(data.departament->land);
    free(data.departament);
    return EXIT_SUCCESS;
}