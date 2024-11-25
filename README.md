# jazelle-wii

The Wii homebrew executes arbitrary ARM code on the Starlet, making use of [https://github.com/mkwcat/saoirse/blob/master/channel/Main/IOSBoot.cpp#L86](Palapeli's exploit), sets the CPU in Jazelle mode, and jumps to the JVM bytecode specified in [bytecode/bytecode](bytecode/bytecode), which is, in this example:

```
bipush 19
istore_0
iload_0
ireturn
```

After the execution, it prints the state of the stack, and of the 8 first local variables.

**NOTE**: A large number of instructions are not natively supported by Jazelle and must be handled by predefined ARM code. As except ``ireturn`` none has been implemented here, it will fail to run most programs. Placeholders for the handlers are available in [src/arm/instr\_handlers.s](src/arm/instr_handlers.s)

# References

- https://mariokartwii.com/showthread.php?tid=1994
- https://hackspire.org/index.php/Jazelle
- https://github.com/SonoSooS/libjz
- https://github.com/neuschaefer/jzvm
- https://github.com/devkitPro/libogc
