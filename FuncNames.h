//
// Created by Greg Postnikoff on 30.10.17.
//

#ifndef COMILELITE_FUNCNAMES_H
#define COMILELITE_FUNCNAMES_H

int PushArg(FILE * file, float * MachineCode, int * current, int StrNum);
int PopArg(FILE * file, float * MachineCode, int * current, int StrNum);
void Sign(float * MachineCode);
int CheckArg( float * MachineCode, int *i, FILE * file, int StrNum );
void PrintToFile(float * MachineCode, FILE * file, int Size);
void FindLabels(FILE * file, Labels * MyLabels);
void AddLbls(FILE * file, float * MachineCode, Labels * MyLabels, int * Pos);

#endif //COMILELITE_FUNCNAMES_H
