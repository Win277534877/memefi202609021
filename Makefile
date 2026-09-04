CC = gcc
LD = ld
OBJCOPY = objcopy

CFLAGS  = -I/usr/include/efi -I/usr/include/efi/x86_64 -fshort-wchar -mno-red-zone -Wall -DEFI_FUNCTION_WRAPPER
LDFLAGS = -nostdlib -znocombreloc -T /usr/lib/gnuefi/elf_x86_64_gnuefi.lds -shared -Bsymbolic -L/usr/lib -lgnuefi -lefi

all: memcheck.efi

memcheck.so: memcheck.c
	$(CC) $(CFLAGS) -c memcheck.c -o memcheck.o
	$(LD) memcheck.o $(LDFLAGS) -o memcheck.so

memcheck.efi: memcheck.so
	$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 memcheck.so memcheck.efi

clean:
	rm -f *.o *.so *.efi
