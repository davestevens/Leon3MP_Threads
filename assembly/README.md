#Modified Assembly Bootloader#

Taken from sparc-elf-4.4.2, crt0.S is the modified assembly which includes stack pointer setting and a conditional to only set .bss data section if CPU0.

##Compilation##
```
sparc-elf-as -v -V -Qy -s -o crt0.o crt0.S
```

Then copy the generated file to the sparc-elf-*/sparc-elf/lib directory.
This is then used during compilation to modify behaviour of the Leon3MP.

**Note: Depending on the configuration being used the crt0.o file will be pulled from alternative directories.**
**sparc-elf/lib is used for default running of sparc-elf-gcc**
**sparc-elf/lib/soft is used when compiling with -msoft-float**

**Use -v flag with sparc-elf-gcc to find which crt0.o file is being used in the current compilation.**