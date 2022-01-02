#pragma warning(disable: 4100)
#include "hello.h"
#include "message.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegisterPath) 
{
	pDriverObject->DriverUnload = UnloadDriver;
	KdPrint(("KdPrint here!\r\n"));
	DebugMessage("Hello World from DriverEntry!");
	return STATUS_SUCCESS;


}
NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject)
{
	DebugMessage("Bye from UnloadDriver"); 
	return STATUS_SUCCESS;
}