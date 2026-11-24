# MyPkg: UEFI PCI diagnostics on ARM64 (EDK II, QEMU)

A small EDK II package with UEFI applications that inspect hardware before an OS boots.

## What it does
- `PciListApp`: lists all PCI devices using `EFI_PCI_IO_PROTOCOL`
  (bus:dev.fn, vendor/device ID, class code, raw BARs).

## Environment
- EDK II (latest master), ArmVirtQemu platform, AARCH64, GCC cross-compiler
- QEMU `virt` machine, cortex-a57, with virtio-net-pci and NVMe devices
- Tested in QEMU only (not on real hardware)

## Result
The app found 3 devices. IDs and class codes match the UEFI Shell `pci`
command and the firmware's own PCI scan log:

| BDF     | VID:DID   | Class    | Device      |
|---------|-----------|----------|-------------|
| 00:00.0 | 1B36:0008 | 06.00.00 | Host bridge |
| 00:01.0 | 1AF4:1000 | 02.00.00 | virtio-net  |
| 00:02.0 | 1B36:0010 | 01.08.02 | NVMe        |

## Problems I fixed
- Newer EDK II renamed toolchain tag `GCC5` to `GCC`.
- QEMU pflash needs both firmware files padded to 64 MB.
- A module must be listed under `[Components.common]` in the platform DSC
  before it can be built.

## Build
    export GCC_AARCH64_PREFIX=aarch64-linux-gnu-
    export PACKAGES_PATH=$HOME/uefi_lab
    build -a AARCH64 -t GCC -b DEBUG -p ArmVirtPkg/ArmVirtQemu.dsc \
      -m MyPkg/PciListApp/PciListApp.inf
    # add MyPkg/PciListApp/PciListApp.inf under [Components.common] in the DSC

## Next
- Decode BAR type and size
- ACPI table reader (RSDP, XSDT, MADT, MCFG)

## About this repo
An independent, public reproduction of a UEFI PCIe/ACPI diagnostic task, built from public specs (UEFI, ACPI, EDK II) on QEMU. It contains no client code.

## Status
- [x] PciListApp: enumerates PCIe devices via EFI_PCI_IO_PROTOCOL (ArmVirtQemu AARCH64)
- [ ] BAR decoding and sizes (GetBarAttributes)
- [ ] ACPI reader: RSDP, XSDT, MCFG, MADT
- [ ] gdb-multiarch debugging notes
