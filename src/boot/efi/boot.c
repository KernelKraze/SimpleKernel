// SPDX-License-Identifier: MIT
// EFI bootloader for MyOS
// Author: KernelKraze

#include <efi.h>
#include <efilib.h>

extern void kernel_main(void);

EFI_STATUS LoadKernel(EFI_HANDLE ImageHandle, CHAR16 *KernelPath, EFI_PHYSICAL_ADDRESS *KernelAddress, UINTN *KernelSize);
EFI_STATUS PrintMemoryMap(void);

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    EFI_STATUS status;

    // 初始化库
    InitializeLib(ImageHandle, SystemTable);

    // 确保显示输出有效
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"MyOS EFI Bootloader\n");

    // 打印系统表地址以确认进入 efi_main
    Print(L"SystemTable: %p\n", SystemTable);

    // 输出内存映射信息
    status = PrintMemoryMap();
    if (EFI_ERROR(status)) {
        Print(L"Failed to print memory map: %r\n", status);
        while (1); // 防止系统重复尝试
        return status;
    }

    // 将内核加载到内存中
    EFI_PHYSICAL_ADDRESS kernel_address = 0x200000; // 内核的目标地址
    UINTN kernel_size = 0;
    status = LoadKernel(ImageHandle, L"\\boot\\kernel.bin", &kernel_address, &kernel_size);
    if (EFI_ERROR(status)) {
        // 打印加载失败信息
        Print(L"Failed to load kernel: %r\n", status);
        while (1); // 防止系统重复尝试
        return status;
    }

    // 打印内核加载成功信息
    Print(L"Kernel loaded successfully at address %lx with size %d bytes\n", kernel_address, kernel_size);

    // 跳转到内核主函数
    Print(L"Jumping to kernel at address %lx\n", kernel_address);
    void (*kernel_entry)(void) = (void (*)(void))kernel_address;
    kernel_entry();

    // 如果内核返回，挂起系统
    Print(L"Kernel returned control to bootloader, halting system.\n");
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
    status = uefi_call_wrapper(BS->HandleProtocol, 3, ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&loaded_image);
    if (EFI_ERROR(status)) {
        Print(L"Error getting loaded image protocol: %r\n", status);
        return status;
    }

    // 获取简单文件系统协议
    status = uefi_call_wrapper(BS->HandleProtocol, 3, loaded_image->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&file_io);
    if (EFI_ERROR(status)) {
        Print(L"Error getting file system protocol: %r\n", status);
        return status;
    }

    // 打开文件系统卷
    status = uefi_call_wrapper(file_io->OpenVolume, 2, file_io, &root);
    if (EFI_ERROR(status)) {
        Print(L"Error opening root volume: %r\n", status);
        return status;
    }

    // 打开内核文件
    status = uefi_call_wrapper(root->Open, 5, root, &file, KernelPath, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status)) {
        Print(L"Error opening kernel file: %r\n", status);
        return status;
    }

    // 获取文件大小
    buffer_size = SIZE_OF_EFI_FILE_INFO + 200;
    file_info = AllocatePool(buffer_size);
    if (!file_info) {
        Print(L"Failed to allocate memory for file info\n");
        return EFI_OUT_OF_RESOURCES;
    }

    status = uefi_call_wrapper(file->GetInfo, 4, file, &gEfiFileInfoGuid, &buffer_size, file_info);
    if (EFI_ERROR(status)) {
        Print(L"Error getting file info: %r\n", status);
        FreePool(file_info);
        return status;
    }

    *KernelSize = file_info->FileSize; // 获取内核文件的大小
    FreePool(file_info);

    // 输出文件大小信息
    Print(L"Kernel file size: %d bytes\n", *KernelSize);

    // 分配内存并读取内核文件
    Print(L"Requesting %d pages of memory\n", EFI_SIZE_TO_PAGES(*KernelSize));
    status = uefi_call_wrapper(BS->AllocatePages, 4, AllocateAnyPages, EfiLoaderData, EFI_SIZE_TO_PAGES(*KernelSize), KernelAddress);
    if (EFI_ERROR(status)) {
        Print(L"Memory allocation failed: %r\n", status);
        return status;
    }

    buffer_size = *KernelSize;
    status = uefi_call_wrapper(file->Read, 3, file, &buffer_size, (void*)(*KernelAddress));
    if (EFI_ERROR(status)) {
        uefi_call_wrapper(BS->FreePages, 2, *KernelAddress, EFI_SIZE_TO_PAGES(*KernelSize));
        Print(L"Failed to read kernel: %r\n", status);
        return status;
    }

    // 关闭文件
    uefi_call_wrapper(file->Close, 1, file);
    return EFI_SUCCESS;
}

EFI_STATUS PrintMemoryMap(void) {
    EFI_STATUS status;
    EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
    EFI_MEMORY_DESCRIPTOR *MemoryMapEnd;
    EFI_MEMORY_DESCRIPTOR *Entry;
    UINTN MemoryMapSize = 0;
    UINTN MapKey;
    UINTN DescriptorSize;
    UINT32 DescriptorVersion;

    status = uefi_call_wrapper(BS->GetMemoryMap, 5, &MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    if (status == EFI_BUFFER_TOO_SMALL) {
        MemoryMap = AllocatePool(MemoryMapSize);
        status = uefi_call_wrapper(BS->GetMemoryMap, 5, &MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    }

    if (EFI_ERROR(status)) {
        Print(L"Failed to get memory map: %r\n", status);
        return status;
    }

    MemoryMapEnd = (EFI_MEMORY_DESCRIPTOR *)((UINT8 *)MemoryMap + MemoryMapSize);
    for (Entry = MemoryMap; Entry < MemoryMapEnd; Entry = (EFI_MEMORY_DESCRIPTOR *)((UINT8 *)Entry + DescriptorSize)) {
        Print(L"Type: %d, PhysicalStart: %lx, NumberOfPages: %lx, Attribute: %lx\n",
            Entry->Type, Entry->PhysicalStart, Entry->NumberOfPages, Entry->Attribute);
    }

    FreePool(MemoryMap);
    return EFI_SUCCESS;
}
