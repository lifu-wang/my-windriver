
#include "ioctl.h"
#include "ntddk.h"

#if 1
#pragma warning(disable: 6067 6068)
VOID DriverUnload(PDRIVER_OBJECT pDriverObject);

PDEVICE_OBJECT DeviceObject = NULL;
UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\mydevice123");
UNICODE_STRING SymbolicName = RTL_CONSTANT_STRING(L"\\??\\mydevicelink123");

NTSTATUS DispatchPassThru(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    PIO_STACK_LOCATION irpsp = IoGetCurrentIrpStackLocation(Irp);
    NTSTATUS status = STATUS_SUCCESS;
    switch (irpsp->MajorFunction) {
    case IRP_MJ_CREATE:
        DbgPrintEx(0, 0, "IRP_MJ_CREATE");
        break;
    case IRP_MJ_CLOSE:
        DbgPrintEx(0, 0, "IRP_MJ_CLOSE");
        break;
    default:
        DbgPrintEx(0, 0, "IRP Invald request");
        status = STATUS_INVALID_PARAMETER;
    }
    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status = STATUS_SUCCESS;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}
// Ctl #: 0x800-0xfff
#define DEVICE_SEND CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_WRITE_DATA)
#define DEVICE_REC  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_READ_DATA)

NTSTATUS DispatchDevCTL(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    PIO_STACK_LOCATION irpsp = IoGetCurrentIrpStackLocation(Irp);
    NTSTATUS status = STATUS_SUCCESS;
    PVOID buffer = Irp->AssociatedIrp.SystemBuffer;  // For METHOD_BUFFERED data 
    ULONG inLen = irpsp->Parameters.DeviceIoControl.InputBufferLength;
    ULONG outLen = irpsp->Parameters.DeviceIoControl.OutputBufferLength;
    ULONG retLen = 0;
    WCHAR* demo = L"Hello from DispatchDevCT";
    switch (irpsp->Parameters.DeviceIoControl.IoControlCode) {
    case DEVICE_SEND:
        DbgPrintEx(0, 0, "Send data is %ws\r\n", buffer);
        retLen = (wcsnlen(buffer, 511) + 1) * 2;
        break;
    case DEVICE_REC:
        wcsncpy(buffer, demo, 511);
        DbgPrintEx(0, 0, "Recv data is %ws\r\n", buffer);
        retLen = (wcsnlen(buffer, 511) + 1) * 2;
        break;
    default:
        status = STATUS_INVALID_PARAMETER;
    }
    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = retLen;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING* pRegistryPath)
{
    int i;
    NTSTATUS status;
    pDriverObject->DriverUnload = DriverUnload;

    status = IoCreateDevice(pDriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);
    if (!NT_SUCCESS(status)) {
        KdPrint(("DriverEntry IoCreateDevice failed!!\r\n"));
        return status;
    }
    status = IoCreateSymbolicLink(&SymbolicName, &DeviceName);
    if (!NT_SUCCESS(status)) {
        KdPrint(("DriverEntry IoCreateSymbolicName failed\r\n"));
        IoDeleteSymbolicLink(&SymbolicName);
        return status;
    }
    KdPrint(("DriverEntry all successful\r\n"));
    DbgPrintEx(0, 0, "From DbgPrintex - DriverEntery all OK");

    for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++) {
        pDriverObject->MajorFunction[i] = DispatchPassThru;
    }
    pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDevCTL;
    // pDriverObject->MajorFunction[IRP_MJ_READ] = f2; ...

    return STATUS_SUCCESS;
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
    IoDeleteSymbolicLink(&SymbolicName);
    IoDeleteDevice(DeviceObject);
    KdPrint(("DriverUnload successfully"));
    DbgPrintEx(0, 0, "From DbgPrintex - Driver Unload OK");

}
#endif


#if 0
VOID DriverUnload(PDRIVER_OBJECT pDriverObject);

PDEVICE_OBJECT DeviceObject = NULL;
UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\mydevice123");
UNICODE_STRING SymbolicName = RTL_CONSTANT_STRING(L"\\??\\mydevicelink123");

NTSTATUS DispatchPassThru(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	PIO_STACK_LOCATION irpsp = IoGetCurrentIrpStackLocation(Irp);
	NTSTATUS status = STATUS_SUCCESS;
	switch (irpsp->MajorFunction) {
	case IRP_MJ_CREATE:
		DbgPrintEx(0, 0, "IRP_MJ_CREATE");
		break;
	case IRP_MJ_CLOSE:
		DbgPrintEx(0, 0, "IRP_MJ_CLOSE");
		break;
	case IRP_MJ_READ:
		DbgPrintEx(0, 0, "IRP_MJ_READ");
		break;
	default:
		DbgPrintEx(0, 0, "IRP default");
		break;
	}
	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING* pRegistryPath) 
{
	int i;
	NTSTATUS status;
	pDriverObject->DriverUnload = DriverUnload;
	
	status = IoCreateDevice(pDriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);
	if (!NT_SUCCESS(status)) {
		KdPrint(("DriverEntry IoCreateDevice failed!!\r\n"));
		return status;
	}
	status = IoCreateSymbolicLink(&SymbolicName, &DeviceName);
	if (!NT_SUCCESS(status)) {
		KdPrint(("DriverEntry IoCreateSymbolicName failed\r\n"));
		IoDeleteSymbolicLink(&SymbolicName);
		return status;
	}
	KdPrint(("DriverEntry all successful\r\n"));
	DbgPrintEx(0, 0, "From DbgPrintex - DriverEntery all OK");
	
	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++) {
		pDriverObject->MajorFunction[i] = DispatchPassThru;
	}
	// pDriverObject->MajorFunction[IRP_MJ_CREATE] = f1; 
	// pDriverObject->MajorFunction[IRP_MJ_READ] = f2; ...

	return STATUS_SUCCESS;
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	IoDeleteSymbolicLink(&SymbolicName);
	IoDeleteDevice(DeviceObject);
	KdPrint(("DriverUnload successfully"));
	DbgPrintEx(0, 0, "From DbgPrintex - Driver Unload OK");

}
#endif
