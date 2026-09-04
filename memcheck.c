#include <efi.h>
#include <efilib.h>

EFI_SYSTEM_TABLE *ST;
EFI_BOOT_SERVICES *BS;

VOID WaitUserConfirm(CHAR16 *Msg)
{
    EFI_INPUT_KEY key;
    UINTN event_idx;

    ST->ConOut->ClearScreen(ST->ConOut);
    ST->ConOut->SetCursorPosition(ST->ConOut, 0,2);
    Print(L"===== MEMORY CHECK WARNING =====\n");
    Print(Msg);
    Print(L"\nPress ANY KEY to continue boot ...\n");

    for(;;)
    {
        BS->WaitForEvent(1, &ST->ConIn->WaitForKey, &event_idx);
        if(!ST->ConIn->ReadKeyStroke(ST->ConIn, &key))
        {
            break;
        }
    }
    ST->ConOut->ClearScreen(ST->ConOut);
}

UINT32 GetMemoryDimmCount(VOID)
{
    EFI_MEMORY_DESCRIPTOR *MemDesc;
    UINTN MapSize, DescriptorSize, MapKey;
    UINT32 DescriptorVer;
    UINT32 count = 0;
    UINTN i;

    MapSize = 0;
    BS->GetMemoryMap(&MapSize, NULL, &MapKey, &DescriptorSize, &DescriptorVer);
    MapSize += DescriptorSize * 4;
    BS->AllocatePool(EfiBootServicesData, MapSize, (VOID**)&MemDesc);
    BS->GetMemoryMap(&MapSize, MemDesc, &MapKey, &DescriptorSize, &DescriptorVer);

    for(i=0; i < MapSize; i += DescriptorSize)
    {
        EFI_MEMORY_DESCRIPTOR *p = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)MemDesc + i);
        if(p->Type == EfiConventionalMemory)
        {
            count++;
        }
    }
    BS->FreePool(MemDesc);
    return count;
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    ST = SystemTable;
    BS = ST->BootServices;
    InitializeLib(ImageHandle, SystemTable);

    UINT32 mem_seg_cnt = GetMemoryDimmCount();

    if(mem_seg_cnt <= 1)
    {
        WaitUserConfirm(L"WARNING: Memory detect abnormal!\nExpected 2 DIMM, one may missing/unrecognized.\nCheck RAM module!");
    }
    return EFI_SUCCESS;
}
