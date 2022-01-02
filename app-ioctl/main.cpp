// app-ioctl.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <stdlib.h>

HANDLE devicehandle = NULL;
int main(int argc, char **argv)
{
    char ch;
    std::cout << "Hello World for ioctl app!\n";

    devicehandle = CreateFile(L"\\\\.\\mydevicelink123", GENERIC_ALL, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
    if (devicehandle == INVALID_HANDLE_VALUE) {
        std::cout << "invalid handle" << std::endl;
            return -1;
    }
    std::cout << "successfully open symbolic link" << std::endl;
    std::cout << "Enter return to continue..." << std::endl;
    ch = getchar();
    CloseHandle(devicehandle);
    return 0;
}

