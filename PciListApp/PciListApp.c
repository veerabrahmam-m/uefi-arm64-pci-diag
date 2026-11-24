#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/PciIo.h>
#include <IndustryStandard/Pci.h>

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS           Status;
  UINTN                Count;
  EFI_HANDLE           *Handles;
  UINTN                I;
  UINTN                B;
  UINTN                Seg, Bus, Dev, Fn;
  EFI_PCI_IO_PROTOCOL  *PciIo;
  PCI_TYPE00           Pci;

  Status = gBS->LocateHandleBuffer (ByProtocol, &gEfiPciIoProtocolGuid, NULL, &Count, &Handles);
  if (EFI_ERROR (Status)) {
    Print (L"No PCI devices found: %r\n", Status);
    return Status;
  }

  Print (L"PciListApp: %d PCI device(s)\n", Count);
  for (I = 0; I < Count; I++) {
    Status = gBS->HandleProtocol (Handles[I], &gEfiPciIoProtocolGuid, (VOID **)&PciIo);
    if (EFI_ERROR (Status)) {
      continue;
    }
    PciIo->GetLocation (PciIo, &Seg, &Bus, &Dev, &Fn);
    PciIo->Pci.Read (PciIo, EfiPciIoWidthUint32, 0, sizeof (Pci) / sizeof (UINT32), &Pci);

    Print (L"%02x:%02x.%x  VID=%04x DID=%04x  Class=%02x.%02x.%02x\n",
           Bus, Dev, Fn, Pci.Hdr.VendorId, Pci.Hdr.DeviceId,
           Pci.Hdr.ClassCode[2], Pci.Hdr.ClassCode[1], Pci.Hdr.ClassCode[0]);
    for (B = 0; B < 6; B++) {
      if (Pci.Device.Bar[B] != 0) {
        Print (L"    BAR%d = 0x%08x\n", B, Pci.Device.Bar[B]);
      }
    }
  }

  gBS->FreePool (Handles);
  return EFI_SUCCESS;
}
