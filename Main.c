#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct Land Land;
typedef struct Departament Departament;
typedef struct Data Data;
typedef enum Antennas Antennas;

#define RANGE_PREVIUS_ANTENNAS 2300 //2300m2

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
};

struct Departament
{
    size_t   id;
    Land     *land;
    size_t   count_land;
    Antennas type;
    int32_t  new_antennas;
    float    usage;
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
    puts("Exit.                                        [4]");
}

void SelectType(Data *data, int32_t index_d, char* type)
{
    switch (type[0])
    {
        case 'a': data->departament[index_d].type = a; break;

        case 'b': data->departament[index_d].type = b; break;

        case 'c': data->departament[index_d].type = c; break;

        case 'd': data->departament[index_d].type = d; break;

        case 'e': data->departament[index_d].type = e; break;
    }
}

size_t ParseIntType(Antennas type)
{
    switch (type)
    {
        case a: return 45500;

        case b: return 16700;

        case c: return 27800;

        case d: return 7600;

        case e: return 13800;
    }
}

char ParseCharType(Antennas type)
{
    switch (type)
    {
        case a: return 'a';

        case b: return 'b';

        case c: return 'c';

        case d: return 'd';

        case e: return 'e';
    }
}

void Calculate(const Data *data)
{
    int32_t departament_selected;
    char    type_antenna[2];
    int32_t total_area = 0;
    int32_t total_area_antennas = 0;
    int32_t missing_area = 0;
    size_t  parsed_type;
    int32_t total_Antennas = 0;

    ClearBuffer();

SelectDepartament:
    printf("Select a departament in a range of [1] to [%lu] to calculate\n> ", data->size);
    scanf("%d", &departament_selected);

    if (departament_selected <= 0 || data->size < departament_selected)
    {
        puts("Select a valid option");
        goto SelectDepartament;
    }

    --departament_selected;

SelectType:
    puts("Select a type of antenna");
    puts("[a] 45500m2");
    puts("[b] 16700m3");
    puts("[c] 27800m2");
    puts("[d] 7600m2");
    puts("[e] 13800m2");

    ClearBuffer();
    printf("\n> ");
    fgets(type_antenna, 2, stdin);

    if (type_antenna[0] < 'a' || 'e' < type_antenna[0])
    {
        puts("Select a valid option");
        goto SelectType;
    }

    SelectType(data, departament_selected, type_antenna);

    for (size_t i = 0 ; i < data->departament[departament_selected].count_land ; ++i)
    {
        total_area          += data->departament[departament_selected].land[i].m2;
        total_area_antennas += data->departament[departament_selected].land[i].previus_antennas;
    }

    total_area_antennas *= RANGE_PREVIUS_ANTENNAS;
    missing_area = total_area - total_area_antennas;

    parsed_type = ParseIntType(data->departament[departament_selected].type);

    total_Antennas = missing_area / parsed_type;

    if (total_Antennas < 0)
    {
        total_Antennas = 0;
    }

    printf("The number of antennas to install's: %d\n", total_Antennas);

    data->departament[departament_selected].new_antennas = total_Antennas;

    ClearBuffer();
    getchar();
}

void List(const Data *data)
{
    Data list;
    bool flag;

    list.departament = malloc(sizeof(Departament) * data->size);

    for (size_t i = 0 ; i < data->size ; ++i)
    {
        list.departament[i].new_antennas = data->departament[i].new_antennas;
        list.departament[i].type = data->departament[i].type;
        list.departament[i].id = data->departament[i].id;
    }

loop:
    flag = false;

    for (size_t i = 0 ; i < (data->size - 1) ; ++i)
    {
        if (list.departament[i].new_antennas > list.departament[i + 1].new_antennas)
        {
            Departament first_dp;
            first_dp.new_antennas = list.departament[i].new_antennas;
            first_dp.type = list.departament[i].type;
            first_dp.id = list.departament[i].id;

            Departament second_dp;
            second_dp.new_antennas = list.departament[i + 1].new_antennas;
            second_dp.type = list.departament[i + 1].type;
            second_dp.id = list.departament[i + 1].id;

            list.departament[i].new_antennas = second_dp.new_antennas;
            list.departament[i].type = second_dp.type;
            list.departament[i].id = second_dp.id;

            list.departament[i + 1].new_antennas = first_dp.new_antennas;
            list.departament[i + 1].type = first_dp.type;
            list.departament[i + 1].id = first_dp.id;

            flag = true;
        }
    }

    if (flag)
    {
        goto loop;
    }

    puts("Departments with fewer new antennas");

    for (size_t i = 0 ; i < data->size ; ++i)
    {
        printf("[%lu] New Antennas: %d, departament number: %lu, type: %c\n", (i + 1), list.departament[i].new_antennas, list.departament[i].id, ParseCharType(list.departament[i].type));
    }


    free(list.departament);

    ClearBuffer();
    getchar();
}

void Porcentage(Data *data)
{
    float total_antennas = 0;
    Data    list;
    bool    flag;

    list.departament = malloc(sizeof(Departament) * data->size);

    for (size_t i = 0 ; i < data->size ; ++i)
    {
        total_antennas += data->departament[i].new_antennas;
        list.departament[i].new_antennas = data->departament[i].new_antennas;
        list.departament[i].type = data->departament[i].type;
        list.departament[i].id = data->departament[i].id;
    }

    for (size_t i = 0 ; i < data->size ; ++i)
    {
        list.departament[i].usage = (list.departament[i].new_antennas / total_antennas) * 100;
        printf("Porcentaje: %.1f\n", list.departament[i].usage);
    }

loop:
    flag = false;

    for (size_t i = 0 ; i < (data->size - 1) ; ++i)
    {
        if (list.departament[i].usage > list.departament[i + 1].usage)
        {
            Departament first_dp;
            first_dp.new_antennas = list.departament[i].new_antennas;
            first_dp.type = list.departament[i].type;
            first_dp.id = list.departament[i].id;
            first_dp.usage = list.departament[i].usage;

            Departament second_dp;
            second_dp.new_antennas = list.departament[i + 1].new_antennas;
            second_dp.type = list.departament[i + 1].type;
            second_dp.id = list.departament[i + 1].id;
            second_dp.usage = list.departament[i + 1].usage;

            list.departament[i].new_antennas = second_dp.new_antennas;
            list.departament[i].type = second_dp.type;
            list.departament[i].id = second_dp.id;
            list.departament[i].usage = second_dp.usage;

            list.departament[i + 1].new_antennas = first_dp.new_antennas;
            list.departament[i + 1].type = first_dp.type;
            list.departament[i + 1].id = first_dp.id;
            list.departament[i + 1].usage = first_dp.usage;

            flag = true;
        }
    }

    if (flag)
    {
        goto loop;
    }

    puts("Total percentage of new antennas");
    for (size_t i = 0 ; i < data->size ; ++i)
    {
        printf("[%lu] Usage: %f, departament number: %lu, type: %c\n", (i + 1), list.departament[i].usage, list.departament[i].id, ParseCharType(list.departament[i].type));
    }

    free(list.departament);

    ClearBuffer();
    getchar();
}

void FreeData(Data *data)
{
    for (size_t i = 0 ; i < data->size ; ++i)
    {
        free(data->departament[i].land);
    }

    free(data->departament);
}

int32_t main(void)
{
    Data    data;
    int32_t count;
    int32_t option = 0;
    size_t  departament_id = 1;

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

    for (size_t i = 0 ; i < data.size ; ++i)
    {
        data.departament[i].id = departament_id++;
    }

    for (size_t i = 0 ; i < data.size ; ++i)
    {
        printf("Enter the numbers of lands of departament [%lu]\n", (i + 1));

        do
        {
            if(count < 0)
            {
                puts("Negative numbers aren't allowed");
            }

            printf("Count of lands\n> ");
            scanf("%d", &count);

        } while (count < 0);

        data.departament[i].land = malloc(sizeof(Land) * count);
        data.departament[i].count_land = count;
    }

    for (size_t d = 0 ; d < data.size ; ++d)
    {
        for (size_t i = 0 ; i < data.departament[d].count_land ; ++i)
        {
            printf("Departament [%lu]\n", (d + 1));
            do
            {
                if (data.departament[d].land[i].m2 < 0)
                {
                    puts("Negative numbers aren't allowed");
                }

                printf("Land size [%lu]\n> ", (i+1));
                scanf("%d", &data.departament[d].land[i].m2);

            } while (data.departament[d].land[i].m2 < 0);

            if (data.departament[d].land[i].m2 < RANGE_PREVIUS_ANTENNAS)
            {
                puts("\nArea of the land is less than the minimun range of the antenna.");
                puts("You can't put antennas here.\n");
                data.departament[d].land[i].previus_antennas = 0;
                continue;
            }

NumberAntennas:
            printf("Enter the number of antennas previously installed\n> ");
            scanf("%d", &data.departament[d].land[i].previus_antennas);

            if ((data.departament[d].land[i].previus_antennas * RANGE_PREVIUS_ANTENNAS) > data.departament[d].land[i].m2)
            {
                puts("The number of antennas exceeds the size of the terrain");
                puts("Enter again");
                goto NumberAntennas;
            }

            if (data.departament[d].land[i].previus_antennas < 0)
            {
                data.departament[d].land[i].previus_antennas = 0;
            }
        }
    }

    do
    {
        system("clear");

        Menu();
        printf("> ");
        scanf("%d", &option);

        if (option == 1)
            Calculate(&data);

        if (option == 2)
            List(&data);

        if (option == 3)
            Porcentage(&data);

    } while (option < 4);

    FreeData(&data);

    return EXIT_SUCCESS;
}