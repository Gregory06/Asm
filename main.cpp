#include <iostream>
#include "Consts.h"

#define DEF_CMD(name, num, code) \
            CMD_##name = num,

enum Commands {

#include "commands.h"
    SOME_NUM = 30

};

#undef DEF_CMD

const int NEXT_IS_RAM_REG = 3;
const int NEXT_IS_RAM = 2;
const int NEXT_IS_NUM = 1;
const int NEXT_IS_REG = 0;

struct Labels {

    int Label = -1;

    int Adress = -10;

};

#include "FuncNames.h"

int main() {

    FILE * file = NULL;
    char FileName[MaxFileNameSize] = {};

    //printf("ENTER FILENAME\n");

    //scanf("%s", FileName);

    strcpy(FileName, "/Users/MyMac/CLionProjects/TopProgramming/ComileLite/1.txt");

    file = fopen( FileName, "r");

    float MachineCode[CodeSize] = {};

    Sign(MachineCode);

    char str[StrSize] = {};

    int StrNum = 0, Pos = SgnPos;

    Labels MyLabels[LblSize] = {};

    FindLabels(file, MyLabels);

    fseek(file, 0, SEEK_SET);

    while (!feof(file)) {

        StrNum++;

        fscanf(file, "%s", str);

        printf("'%s'\n", str);

#define DEF_CMD(name, num, code) \
    if ( strcmp(str, #name) == 0 ) \
        MachineCode[Pos++] = CMD_##name; \
    else
#include "commands.h"

#undef DEF_CMD

        if (str[0] != ':') {
            printf("'%s' -- ERROR... UNKNOWN CMD ON STR %d\n"
                           "I'm dying\n",str, StrNum);
            return 0;
        }

        if ((MachineCode[Pos - 1] >= CMD_JMP) &&
            (MachineCode[Pos - 1] <= CMD_CALL))
        {
            AddLbls(file, MachineCode, MyLabels, &Pos);
        }

        if (((MachineCode[Pos - 1] == CMD_PUSH) ||
             (MachineCode[Pos - 1] == CMD_POP)))
            {
            if (CheckArg(MachineCode, &Pos, file, StrNum) == ERROR)
                //printf("NET\n");
                return 0;
            }
    }

    fclose(file);

    file = fopen("/Users/MyMac/CLionProjects/TopProgramming/Procesor/1.txt", "w");


    PrintToFile(MachineCode, file, Pos);

    printf("Compilation finished. Run file is /Users/MyMac/CLionProjects/TopProgramming/Procesor/1.txt\n");

    return 0;
}

#include "Funcs.h"
