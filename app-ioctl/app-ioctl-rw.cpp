// app-ioctl-rw.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <winioctl.h>
                             
#define DEVICE_SEND CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_WRITE_DATA)
#define DEVICE_REC CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_WRITE_DATA)

int main(int argc, char **argv)
{
    HANDLE devicehandle = NULL;
    char ch;
    ULONG retLen;
    const wchar_t* mesg = L"Ok la";
    const WCHAR* smsg = L"Send mesg from app";
    WCHAR rbuf[1024] = { 0 };
    
    while (1) {
        std::cout << "" << std::endl;
        std::cout << "Enter [1-4,0] 1 : Open, 2 : close, 3 : IOCTL read, 4 : IOCTL write, 0 : Exit -> " ;
        ch = getchar();
        switch (ch) {
            
        case '1':
            devicehandle = CreateFile(L"\\\\.\\mydevicelink123", 
                GENERIC_ALL, 
                0, 
                0, 
                OPEN_EXISTING, 
                FILE_ATTRIBUTE_SYSTEM, 
                0);
            if (devicehandle == INVALID_HANDLE_VALUE || devicehandle == INVALID_HANDLE_VALUE) {
                std::cout << "invalid handle" << std::endl;
                return -1;
            }
            std::cout << "Open successfully" << std::endl;
            break;
        
        case '2':
            if (devicehandle == NULL || devicehandle == INVALID_HANDLE_VALUE) {
                std::cout << "Cannot close NULL device handle! Open first." << std::endl;
                break;
            }
            CloseHandle(devicehandle);
            std::cout << "Close" << std::endl;
            break;

        case '3': // Send
            if (devicehandle == NULL || devicehandle == INVALID_HANDLE_VALUE) {
                std::cout << "Cannot IOCTL Read NULL device handle! Open first." << std::endl;
                break;
            }
            if (!DeviceIoControl(devicehandle,
                            DEVICE_SEND, 
                            (PVOID)smsg,
                            (wcslen(smsg)+1)*2,
                            NULL,
                            0,
                            &retLen,
                            0)) {
                std::cout << "DeviceIoControl send error" << retLen << std::endl;
            }
            std::cout << "IOCTL Send successfully! " << retLen << std::endl;
            break;

        case '4':
            if (devicehandle == NULL) {
                std::cout << "Cannot IOCTL Write NULL device handle! Open first." << std::endl;
                break;
            }
            if (!DeviceIoControl(devicehandle,
                        DEVICE_REC,
                        NULL,
                        0,
                        rbuf,
                        1024,
                        &retLen, 
                        0)) {
                std::cout << "DeviceIoControl recv error" << std::endl;
            }
            std::cout << "IOCTL Recv successfully! " << retLen << std::endl;
            break;
        default:
            break;
        }
        if (ch == '0')
            break;
    }
    return 0;
}
