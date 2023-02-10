#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct Land Land;
typedef struct Departament Departament;
typedef struct Data Data;
typedef enum Antennas Antennas;

void ClearBuffer()
{
    char buffer[1024];
    fgets(buffer, 1024, stdin);
}

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
    int32_t  m2;
    int32_t  previus_antennas;  //2300m2
    int32_t  new_antennas;
    Antennas type;
};

struct Departament
{
    Land   *land;
    size_t count_land;
};

struct Data
{
    Departament *departament;
    size_t      size;
};

void Menu()
{
    puts("Calculate the number of remaining antennas   [1]");
    puts("List departments with the number of antennas [2]");
    puts("Estimated list of how many antennas exist    [3]");
}

void Calculate(const Data *data)
{
    int32_t select;
    char    letter[2];

    ClearBuffer();

SelectDepartament:
    printf("Select a departament in a range of [1] to [%lu] to calculate\n> ", data->size);
    scanf("%d", &select);

    if (select < 0 || data->size < select)
    {
        puts("Select a valid option");
        goto SelectDepartament;
    }

SelectType:
    puts("Select a type of antenna");
    puts("[a] 45500m2");
    puts("[b] 16700m3");
    puts("[c] 27800m2");
    puts("[d] 7600m2");
    puts("[e] 13800m2");

    ClearBuffer();
    printf("\n> ");
    fgets(letter, 2, stdin);

    if (letter[0] < 'a' || 'e' < letter[0])
    {
        puts("Select a valid option");
        goto SelectType;
    }
}

void List()
{

}

void Porcentage()
{

}

int32_t main(void)
{
    Data    data;
    int32_t count;
    int32_t option;

    do
    {
        if(count < 0)
        {
            puts("Negative numbers aren't allowed");
        }

        printf("Numbers of departaments\n> ");
        scanf("%d", &count);

    } while (count < 0);


    data.departament = malloc(sizeof(Departament) * count);
    data.size = count;

    do
    {
        if(count < 0)
        {
            puts("Negative numbers aren't allowed");
        }

        printf("Count of lands\n> ");
        scanf("%d", &count);

    } while (count < 0);

    data.departament->land = malloc(sizeof(Land) * count);
    data.departament->count_land = count;

    for (size_t d = 0 ; d < data.size ; ++d)
    {
        for (size_t i = 0 ; i < data.departament[d].count_land ; ++i)
        {
            do
            {
                if (data.departament[d].land[i].m2 < 0)
                {
                    puts("Negative numbers aren't allowed");
                }

                printf("Land size [%lu]\n> ", (i+1));
                scanf("%d", &data.departament[d].land[i].m2);

            } while (data.departament[d].land[i].m2 < 0);


            printf("Enter the number of antennas previously installed\n> ");
            scanf("%d", &data.departament[d].land[i].previus_antennas);

            if (data.departament[d].land[i].previus_antennas < 0)
            {
                data.departament[d].land[i].previus_antennas = 0;
            }
        }
    }

    system("clear");

    Menu();
    printf("> ");
    scanf("%d", &option);

    if (option == 1)
        Calculate(&data);

    if (option == 2)
        List();

    if (option == 3)
        Porcentage();

    free(data.departament->land);
    free(data.departament);
    return EXIT_SUCCESS;
}