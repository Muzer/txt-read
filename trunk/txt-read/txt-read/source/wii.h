#ifndef WII_H
#define WII_H
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "wii.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <wiiuse/wpad.h>
#include <fstream>
#include <unistd.h>
#include <sys/dir.h>
#define clrscr() cout << "\033[2J\033[2;0f";

int boxDrawing();
void init();
#endif
