# jazelle-wii

This Wii homebrew executes ARM code on the Starlet with [Palapeli's exploit](https://github.com/mkwcat/saoirse/blob/master/channel/Main/IOSBoot.cpp#L86), and uses it to set the Starlet in Jazelle mode. It then jumps to the JVM bytecode specified in [bytecode/bytecode](bytecode/bytecode). In the included example, it is: 

```
bipush 19
istore_0
iload_0
ireturn
```

After the execution, it prints the state of the stack, and the 8 first local variables.

**NOTE: A large number of instructions is not natively supported by Jazelle and must be handled by predefined ARM code. As except ``ireturn`` none has been implemented here, it will fail to run most programs. Placeholders for the handlers are available in [src/arm/instr\_handlers.s](src/arm/instr_handlers.s). However, I don't really intend to implement them in the near future. This repositorty can then serve as a basis for a broader implementation.**

# References
- https://github.com/mkwcat/saoirse/blob/master/channel/Main/IOSBoot.cpp (IOSBoot::Entry) (Palapeli's exploit)
- https://mariokartwii.com/showthread.php?tid=1994
- https://hackspire.org/index.php/Jazelle
- https://github.com/SonoSooS/libjz
- https://github.com/neuschaefer/jzvm
- https://github.com/devkitPro/libogc
