// SPDX-License-Identifier: MIT
// EFI bootloader for MyOS
// Author: KernelKraze

#include <efi.h>
#include <efilib.h>

extern void kernel_main(void);

// 声明 LoadKernel 函数
// Declaration of LoadKernel function
EFI_STATUS LoadKernel(EFI_HANDLE ImageHandle, CHAR16 *KernelPath, EFI_PHYSICAL_ADDRESS *KernelAddress, UINTN *KernelSize);

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    // 初始化库
    // Initialize the library
    InitializeLib(ImageHandle, SystemTable);

    // 清屏并打印启动信息
    // Clear the screen and print the boot message
    ST->ConOut->ClearScreen(ST->ConOut);
    Print(L"MyOS EFI Bootloader\n");

    // 将内核加载到内存中
    // Load the kernel into memory
    EFI_PHYSICAL_ADDRESS kernel_address = 0x100000; // 内核的目标地址 // Target address for the kernel
    UINTN kernel_size = 0;
    EFI_STATUS status = LoadKernel(ImageHandle, L"\\EFI\\BOOT\\kernel.bin", &kernel_address, &kernel_size);
    if (EFI_ERROR(status)) {
        // 打印加载失败信息
        // Print failure message if loading the kernel fails
        Print(L"Failed to load kernel: %r\n", status);
        return status;
    }

    // 跳转到内核主函数
    // Jump to the kernel main function
    void (*kernel_entry)(void) = (void (*)(void))kernel_address;
    kernel_entry();

    // 如果内核返回，挂起系统
    // Hang the system if the kernel returns
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

    // 获取已加载图像协议
    // Get the loaded image protocol
    status = uefi_call_wrapper(BS->HandleProtocol, 3, ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&loaded_image);
    if (EFI_ERROR(status)) return status;

    // 获取简单文件系统协议
    // Get the simple file system protocol
    status = uefi_call_wrapper(BS->HandleProtocol, 3, loaded_image->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&file_io);
    if (EFI_ERROR(status)) return status;

    // 打开文件系统卷
    // Open the file system volume
    status = uefi_call_wrapper(file_io->OpenVolume, 2, file_io, &root);
    if (EFI_ERROR(status)) return status;

    // 打开内核文件
    // Open the kernel file
    status = uefi_call_wrapper(root->Open, 5, root, &file, KernelPath, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status)) return status;

    // 获取文件大小
    // Get the file size
    buffer_size = SIZE_OF_EFI_FILE_INFO + 200;
    file_info = AllocatePool(buffer_size);
    if (!file_info) return EFI_OUT_OF_RESOURCES;

    status = uefi_call_wrapper(file->GetInfo, 4, file, &gEfiFileInfoGuid, &buffer_size, file_info);
    if (EFI_ERROR(status)) {
        FreePool(file_info);
        return status;
    }

    *KernelSize = file_info->FileSize; // 获取内核文件的大小 // Get the size of the kernel file
    FreePool(file_info);

    // 分配内存并读取内核文件
    // Allocate memory and read the kernel file
    status = uefi_call_wrapper(BS->AllocatePages, 4, AllocateAddress, EfiLoaderData, EFI_SIZE_TO_PAGES(*KernelSize), KernelAddress);
    if (EFI_ERROR(status)) return status;

    buffer_size = *KernelSize;
    status = uefi_call_wrapper(file->Read, 3, file, &buffer_size, (void*)(*KernelAddress));
    if (EFI_ERROR(status)) {
        uefi_call_wrapper(BS->FreePages, 2, *KernelAddress, EFI_SIZE_TO_PAGES(*KernelSize));
        return status;
    }

    // 关闭文件
    // Close the file
    uefi_call_wrapper(file->Close, 1, file);
    return EFI_SUCCESS;
}
