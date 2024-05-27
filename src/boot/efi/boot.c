// SPDX-License-Identifier: MIT
// EFI bootloader for MyOS
// Author: KernelKraze

#include <efi.h>
#include <efilib.h>

extern void kernel_main(void);

// 声明 LoadKernel 函数
EFI_STATUS LoadKernel(EFI_HANDLE ImageHandle, CHAR16 *KernelPath, EFI_PHYSICAL_ADDRESS *KernelAddress, UINTN *KernelSize);

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);

    // Clear the screen and print the boot message
    ST->ConOut->ClearScreen(ST->ConOut);
    Print(L"MyOS EFI Bootloader\n");

    // Load the kernel into memory
    EFI_PHYSICAL_ADDRESS kernel_address = 0x100000;
    UINTN kernel_size = 0;
    EFI_STATUS status = LoadKernel(ImageHandle, L"\\EFI\\BOOT\\kernel.bin", &kernel_address, &kernel_size);
    if (EFI_ERROR(status)) {
        Print(L"Failed to load kernel: %r\n", status);
        return status;
    }

    // Jump to the kernel main function
    void (*kernel_entry)(void) = (void (*)(void))kernel_address;
    kernel_entry();

    // If kernel returns, hang the system
    while (1);
    return EFI_SUCCESS;
}

EFI_STATUS LoadKernel(EFI_HANDLE ImageHandle, CHAR16 *KernelPath, EFI_PHYSICAL_ADDRESS *KernelAddress, UINTN *KernelSize) {
    EFI_STATUS status;
    EFI_LOADED_IMAGE *loaded_image;
    EFI_FILE_IO_INTERFACE *file_io;
    EFI_FILE_HANDLE root, file;
    EFI_FILE_INFO *file_info;
    UINTN buffer_size = 0;

    // Get the loaded image protocol
    status = uefi_call_wrapper(BS->HandleProtocol, 3, ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&loaded_image);
    if (EFI_ERROR(status)) return status;

    // Get the simple file system protocol
    status = uefi_call_wrapper(BS->HandleProtocol, 3, loaded_image->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&file_io);
    if (EFI_ERROR(status)) return status;

    // Open the file system volume
    status = uefi_call_wrapper(file_io->OpenVolume, 2, file_io, &root);
    if (EFI_ERROR(status)) return status;

    // Open the kernel file
    status = uefi_call_wrapper(root->Open, 5, root, &file, KernelPath, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status)) return status;

    // Get the file size
    buffer_size = SIZE_OF_EFI_FILE_INFO + 200;
    file_info = AllocatePool(buffer_size);
    if (!file_info) return EFI_OUT_OF_RESOURCES;

    status = uefi_call_wrapper(file->GetInfo, 4, file, &gEfiFileInfoGuid, &buffer_size, file_info);
    if (EFI_ERROR(status)) {
        FreePool(file_info);
        return status;
    }

    *KernelSize = file_info->FileSize;
    FreePool(file_info);

    // Allocate memory and read the kernel file
    status = uefi_call_wrapper(BS->AllocatePages, 4, AllocateAddress, EfiLoaderData, EFI_SIZE_TO_PAGES(*KernelSize), KernelAddress);
    if (EFI_ERROR(status)) return status;

    buffer_size = *KernelSize;
    status = uefi_call_wrapper(file->Read, 3, file, &buffer_size, (void*)(*KernelAddress));
    if (EFI_ERROR(status)) {
        uefi_call_wrapper(BS->FreePages, 2, *KernelAddress, EFI_SIZE_TO_PAGES(*KernelSize));
        return status;
    }

    // Close the file
    uefi_call_wrapper(file->Close, 1, file);
    return EFI_SUCCESS;
}
