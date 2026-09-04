#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Protocol/Smbios.h>

EFI_STATUS EFIAPI UefiMain (IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_SMBIOS_PROTOCOL *Smbios = NULL;
  EFI_STATUS Status;
  UINTN DimmCount = 0;

  Status = gBS->LocateProtocol(&gEfiSmbiosProtocolGuid, NULL, (VOID**)&Smbios);
  if (!EFI_ERROR(Status) && Smbios != NULL)
  {
    EFI_SMBIOS_HANDLE Handle = 0;
    EFI_SMBIOS_TABLE_HEADER *Record;
    for(;;)
    {
      Status = Smbios->GetNext(Smbios, &Handle, NULL, &Record);
      if (Status != EFI_SUCCESS)
        break;
      if (Record->Type == 17)
      {
        DimmCount++;
      }
    }
  }

  Print(L"\r\n===== DIMM Check Tool =====\r\n");
  Print(L"Detected DIMM(SMBIOS Type17): %d\r\n", DimmCount);

  /*产线需求：预期2根内存，数量不对就阻塞等待按键确认*/
  if(DimmCount != 2)
  {
    Print(L"\r\n!!! ALERT: DIMM QUANTITY MISMATCH !!!\r\n");
    Print(L"Expect:2  Found:%d\r\n", DimmCount);
    Print(L"Press ENTER to continue boot >>>\r\n");

    EFI_INPUT_KEY Key;
    while(SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key) == EFI_NOT_READY);
  }
  else
  {
    Print(L"DIMM check OK, continue boot.\r\n");
  }

  gBS->Stall(1000000);
  return EFI_SUCCESS;
}
