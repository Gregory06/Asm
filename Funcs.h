//
// Created by Greg Postnikoff on 30.10.17.
//

#ifndef COMILELITE_FUNCS_H
#define COMILELITE_FUNCS_H
#include "Consts.h"

int PushArg(FILE * file, float * MachineCode, int * current, int StrNum) {

    int input = 0;
    char ClPar = '_';
    char str[StrSize] = {};

    if (fscanf( file, "%d", &input))
    {

        MachineCode[(* current)++] = (float) NEXT_IS_NUM;

        MachineCode[(* current)++] = (float) input;

    } else

        if (fscanf(file, " [%d]", &input)) {

            fscanf(file, "%c", &ClPar);

            MachineCode[(*current)++] = (float) NEXT_IS_RAM;

            MachineCode[(*current)++] = (float) input;

        } else {

            fseek(file, -1, SEEK_CUR);

            if (fscanf(file, " [%s]", str)) {

                str[2] = 0;

#define REG(name, num) \
        if (strcmp(str, #name) == 0) {\
            MachineCode[(* current)++] = NEXT_IS_RAM_REG;\
            MachineCode[(* current)++] = num;\
        } else

#include "registr.h"

                    printf("ERR\n");

#undef REG
            } else {

                int n = 0;
                fscanf(file, "%s %n", str, &n);

#define REG(name, num) \
        if (strcmp(str, #name) == 0) {\
            MachineCode[(* current)++] = NEXT_IS_REG;\
            MachineCode[(* current)++] = num;\
        } else

#include "registr.h"

                    fseek(file, -n, SEEK_CUR);

#undef REG

            }

        }
        return SUCCESS;


}

int PopArg(FILE * file, float * MachineCode, int * current, int StrNum) {

    char ClPar = '_';
    char str[StrSize] = {};
    int input = 0;

    if (fscanf( file, " [%d]", &input)) {

        fscanf( file, "%c", &ClPar);

        MachineCode[(* current)++] = (float) NEXT_IS_RAM;

        MachineCode[(* current)++] = (float) input;

    } else {

        fseek(file, -1, SEEK_CUR);

        if (fscanf(file, " [%s]", str)) {

            str[2] = 0;

#define REG(name, num) \
        if (strcmp(str, #name) == 0) {\
            MachineCode[(* current)++] = NEXT_IS_RAM_REG;\
            MachineCode[(* current)++] = num;\
        } else

#include "registr.h"

                printf("ERR\n");

#undef REG
        } else {

            printf("###\n");
            int n;
            printf("'%s'\n", str);

            fscanf(file, "%s %n", str, &n);

            printf("'%s'\n", str);

#define REG(name, num) \
        if (strcmp(str, #name) == 0) {\
            MachineCode[(* current)++] = NEXT_IS_REG;\
            MachineCode[(* current)++] = num;\
        } else

#include "registr.h"

#undef REG
                fseek(file, -n, SEEK_CUR);

            if (MachineCode[(*current) - 1] == CMD_POP)
                MachineCode[(*current)++] = NEXT_IS_NUM;

        }
    }
    return SUCCESS;
}

void Sign(float * MachineCode) {

    MachineCode[0] = 'G';
    MachineCode[1] = 'Y';
    MachineCode[2] = 'P';
    MachineCode[3] = (char) ProgVers;

}

int CheckArg(float * MachineCode, int *i, FILE * file, int StrNum) {

    if (MachineCode[*i - 1] == CMD_PUSH)
        if (PushArg(file, MachineCode, i, StrNum))
            return ERROR;
    if (MachineCode[*i - 1] == CMD_POP)
        if (PopArg(file, MachineCode, i, StrNum))
            return ERROR;

    return SUCCESS;

}

void PrintToFile(float * MachineCode, FILE * file, int Size) {

    for ( int k = 0; k < Size; k++) {
        fprintf(file, "%lg\n", MachineCode[k]);
    }


}

void FindLabels(FILE * file, Labels * MyLabels) {

    char str[10] = {};

    int Adress = 0, Pos = 0, Cmd = 0;

    while (!feof(file)) {

        str[0] = '\0';

        fscanf(file, "%s", str);

#define DEF_CMD(name, num, code) \
        if ( strcmp(str, #name) == 0 ) {\
            Cmd = CMD_##name;\
            Adress++; \
        }

#include "commands.h"

#undef DEF_CMD

        if ((Cmd >= CMD_JMP) && (Cmd <= CMD_CALL)) {
            Adress++;
            fscanf(file,"%[ :]", str);
        }

        if (Cmd == CMD_PUSH)
            Adress += 2;

        if (Cmd == CMD_POP)
            Adress += 2;

        if (str[0] == ':')
            if ((Cmd < CMD_JMP) || (Cmd > CMD_CALL)) {
                MyLabels[Pos].Label = (int) str[1];
                MyLabels[Pos++].Adress = Adress + 4;
            }

        Cmd = -1;
    }

}

void AddLbls(FILE * file, float * MachineCode, Labels * MyLabels, int * Pos) {

    char lbl[4] = {};

    fscanf( file, "%s", lbl);

    int int_lbl = (int) lbl[1];

    for (int i = 0; i < LblSize; i++)
        if (MyLabels[i].Label == int_lbl) {
            MachineCode[(*Pos)++] = MyLabels[i].Adress;
        }
}

#endif //COMILELITE_FUNCS_H
