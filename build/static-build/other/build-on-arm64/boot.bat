@echo on

:: -nographic

"C:\Program Files\qemu\qemu-system-aarch64.exe" -smp 2 -m 1024 -M virt -bios QEMU_EFI.fd ^
       -device virtio-blk-device,drive=image ^
       -drive if=none,id=image,file=xenial-server-cloudimg-arm64-uefi1.img ^
       -netdev user,id=user0 -device virtio-net-device,netdev=user0 -redir tcp:2222::22 ^
       -cpu cortex-a57 ^
	   -vga std ^
	   -cdrom ubuntu-16.04.3-server-arm64.iso
	   
	   