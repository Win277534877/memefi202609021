#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <IndustryStandard/SmBios.h>

EFI_STATUS
EFIAPI
UefiMain (IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  SMBIOS_TABLE_ENTRY_POINT *SmbiosEntry;
  SMBIOS_STRUCTURE_POINTER Smbios;
  UINT8 DimmCount = 0;

  Print(L"=== DIMM Filter App: Only Report 1st DIMM ===\n");

  Status = gBS->LocateProtocol(&gEfiSmbiosTableProtocolGuid, NULL, (VOID**)&SmbiosEntry);
  if(EFI_ERROR(Status)){
    Print(L"ERROR: Cannot find SMBIOS table, Status=%r\n", Status);
    return Status;
  }

  Smbios.Hdr = (SMBIOS_HEADER *)SmbiosEntry->TableAddress;

  while(TRUE)
  {
    if(Smbios.Hdr->Type == SMBIOS_TYPE_END_OF_TABLE)
    {
      break;
    }

    if(Smbios.Hdr->Type == SMBIOS_TYPE_MEMORY_DEVICE)
    {
      DimmCount++;
      /* 只打印第一根DIMM，其余全部跳过 */
      if(DimmCount == 1)
      {
        SMBIOS_MEMORY_DEVICE *MemDev = (SMBIOS_MEMORY_DEVICE *)Smbios.Hdr;
        Print(L"------ DIMM #1 ------\n");
        Print(L"Size(MB): %d \n", MemDev->Size);
        Print(L"DeviceLocator: %s\n", Smbios.String(MemDev->DeviceLocator));
        Print(L"BankLocator: %s\n", Smbios.String(MemDev->BankLocator));
      }
    }

    /* SMBIOS结构体向后跳转 */
    Smbios.Hdr = (SMBIOS_HEADER *)((UINT8*)Smbios.Hdr + Smbios.Hdr->Length);
    while(*((UINT8*)Smbios.Hdr)!=0 || *((UINT8*)Smbios.Hdr+1)!=0){
      Smbios.Hdr = (SMBIOS_HEADER *)((UINT8*)Smbios.Hdr + 1);
    }
    Smbios.Hdr = (SMBIOS_HEADER *)((UINT8*)Smbios.Hdr + 2);
  }

  Print(L"HW total DIMM count: %d , Report only DIMM#1\n", DimmCount);
  Print(L"MemFilterApp Exit\n");
  return EFI_SUCCESS;
}
