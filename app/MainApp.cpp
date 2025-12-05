#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <chrono>

#include "Base.h"

int main(int argc, char *argv[])
{
    COUT << "Simulator Midware!" << endl;
    printf("hello world!  %s\n", Base::currentTime().c_str());
    return 0;
}