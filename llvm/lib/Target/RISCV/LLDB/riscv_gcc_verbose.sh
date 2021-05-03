/opt/homebrew/Cellar/riscv-gnu-toolchain/master/libexec/gcc/riscv64-unknown-elf/10.2.0/cc1 \
    main.c\
    -mtune=rocket\
    -march=rv64imafdc\
    -mabi=lp64d\
    -march=rv64imafdc\
    -auxbase main\
    -o ccyXVBp5.s

/opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/../../../../riscv64-unknown-elf/bin/as\
    --traditional-format\
    -march=rv64imafdc\
    -march=rv64imafdc\
    -mabi=lp64d\
    -o ccu0pQ2R.o\
    ccyXVBp5.s


/opt/homebrew/Cellar/riscv-gnu-toolchain/master/libexec/gcc/riscv64-unknown-elf/10.2.0/collect2\
    -plugin /opt/homebrew/Cellar/riscv-gnu-toolchain/master/libexec/gcc/riscv64-unknown-elf/10.2.0/liblto_plugin.so\
    -plugin-opt=/opt/homebrew/Cellar/riscv-gnu-toolchain/master/libexec/gcc/riscv64-unknown-elf/10.2.0/lto-wrapper\
    -plugin-opt=-pass-through=-lgcc\
    -plugin-opt=-pass-through=-lc\
    -plugin-opt=-pass-through=-lgloss\
    -plugin-opt=-pass-through=-lgcc\
    --sysroot=/opt/homebrew/Cellar/riscv-gnu-toolchain/master/riscv64-unknown-elf\
    -melf64lriscv\
    -o main_gcc\
    /opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/../../../../riscv64-unknown-elf/lib/crt0.o\
    /opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/crtbegin.o\
    -L/opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0\
    -L/opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/../../../../riscv64-unknown-elf/lib\
    -L/opt/homebrew/Cellar/riscv-gnu-toolchain/master/riscv64-unknown-elf/lib\
    ccu0pQ2R.o\
    -lgcc\
    --start-group\
    -lc\
    -lgloss\
    --end-group\
    -lgcc\
    /opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/crtend.o





riscv64-unknown-elf-gcc --verbose -o main_gcc main.c
Using built-in specs.
COLLECT_GCC=riscv64-unknown-elf-gcc
COLLECT_LTO_WRAPPER=/opt/homebrew/Cellar/riscv-gnu-toolchain/master/libexec/gcc/riscv64-unknown-elf/10.2.0/lto-wrapper
Target: riscv64-unknown-elf
Configured with: /private/tmp/riscv-gnu-toolchain-20210407-70096-1vihgq5/riscv-gcc/configure --target=riscv64-unknown-elf --prefix=/opt/homebrew/Cellar/riscv-gnu-toolchain/master --disable-shared --disable-threads --enable-languages=c,c++ --with-system-zlib --enable-tls --with-newlib --with-sysroot=/opt/homebrew/Cellar/riscv-gnu-toolchain/master/riscv64-unknown-elf --with-native-system-header-dir=/include --disable-libmudflap --disable-libssp --disable-libquadmath --disable-libgomp --disable-nls --disable-tm-clone-registry --src=.././riscv-gcc --disable-multilib --with-abi=lp64d --with-arch=rv64imafdc --with-tune=rocket 'CFLAGS_FOR_TARGET=-Os   -mcmodel=medlow' 'CXXFLAGS_FOR_TARGET=-Os   -mcmodel=medlow'
Thread model: single
Supported LTO compression algorithms: zlib
gcc version 10.2.0 (GCC)

COLLECT_GCC_OPTIONS='-v' '-o' 'main_gcc' '-mtune=rocket' '-march=rv64imafdc' '-mabi=lp64d' '-march=rv64imafdc'
/opt/homebrew/Cellar/riscv-gnu-toolchain/master/libexec/gcc/riscv64-unknown-elf/10.2.0/cc1 \
    -quiet\
    -v main.c\
    -quiet\
    -dumpbase main.c\
    -mtune=rocket\
    -march=rv64imafdc\
    -mabi=lp64d\
    -march=rv64imafdc\
    -auxbase main\
    -version\
    -o /var/folders/jw/1_6fksp930zdby0pkq6ybdd00000gp/T//ccyXVBp5.s

GNU C17 (GCC) version 10.2.0 (riscv64-unknown-elf)
	compiled by GNU C version Apple LLVM 12.0.0 (clang-1200.0.32.28), GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.23-GMP

GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
ignoring nonexistent directory "/opt/homebrew/Cellar/riscv-gnu-toolchain/master/riscv64-unknown-elf/usr/local/include"
ignoring duplicate directory "/opt/homebrew/Cellar/riscv-gnu-toolchain/master/riscv64-unknown-elf/include"
ignoring duplicate directory "/opt/homebrew/include"
  as it is a non-system directory that duplicates a system directory
ignoring duplicate directory "."
  as it is a non-system directory that duplicates a system directory
#include "..." search starts here:
#include <...> search starts here:
 /opt/homebrew/include
 .
 /opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/include
 /opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/include-fixed
 /opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/../../../../riscv64-unknown-elf/include
End of search list.
GNU C17 (GCC) version 10.2.0 (riscv64-unknown-elf)
	compiled by GNU C version Apple LLVM 12.0.0 (clang-1200.0.32.28), GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.23-GMP

GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
Compiler executable checksum: 993af2ed1067ee4620f4a197377344f2

COLLECT_GCC_OPTIONS='-v' '-o' 'main_gcc' '-mtune=rocket' '-march=rv64imafdc' '-mabi=lp64d' '-march=rv64imafdc'
/opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/../../../../riscv64-unknown-elf/bin/as\
    -v\
    --traditional-format\
    -march=rv64imafdc\
    -march=rv64imafdc\
    -mabi=lp64d\
    -o /var/folders/jw/1_6fksp930zdby0pkq6ybdd00000gp/T//ccu0pQ2R.o\
    /var/folders/jw/1_6fksp930zdby0pkq6ybdd00000gp/T//ccyXVBp5.s

GNU assembler version 2.35 (riscv64-unknown-elf) using BFD version (GNU Binutils) 2.35
COMPILER_PATH=/opt/homebrew/Cellar/riscv-gnu-toolchain/master/libexec/gcc/riscv64-unknown-elf/10.2.0/:/opt/homebrew/Cellar/riscv-gnu-toolchain/master/libexec/gcc/riscv64-unknown-elf/10.2.0/:/opt/homebrew/Cellar/riscv-gnu-toolchain/master/libexec/gcc/riscv64-unknown-elf/:/opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/:/opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/:/opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/../../../../riscv64-unknown-elf/bin/
LIBRARY_PATH=/opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/:/opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/../../../../riscv64-unknown-elf/lib/:/opt/homebrew/Cellar/riscv-gnu-toolchain/master/riscv64-unknown-elf/lib/


COLLECT_GCC_OPTIONS='-v' '-o' 'main_gcc' '-mtune=rocket' '-march=rv64imafdc' '-mabi=lp64d' '-march=rv64imafdc'
/opt/homebrew/Cellar/riscv-gnu-toolchain/master/libexec/gcc/riscv64-unknown-elf/10.2.0/collect2\
    -plugin /opt/homebrew/Cellar/riscv-gnu-toolchain/master/libexec/gcc/riscv64-unknown-elf/10.2.0/liblto_plugin.so\
    -plugin-opt=/opt/homebrew/Cellar/riscv-gnu-toolchain/master/libexec/gcc/riscv64-unknown-elf/10.2.0/lto-wrapper\
    -plugin-opt=-fresolution=/var/folders/jw/1_6fksp930zdby0pkq6ybdd00000gp/T//ccqSI8xw.res\
    -plugin-opt=-pass-through=-lgcc\
    -plugin-opt=-pass-through=-lc\
    -plugin-opt=-pass-through=-lgloss\
    -plugin-opt=-pass-through=-lgcc\
    --sysroot=/opt/homebrew/Cellar/riscv-gnu-toolchain/master/riscv64-unknown-elf\
    -melf64lriscv\
    -o main_gcc\
    /opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/../../../../riscv64-unknown-elf/lib/crt0.o\
    /opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/crtbegin.o\
    -L/opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0\
    -L/opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/../../../../riscv64-unknown-elf/lib\
    -L/opt/homebrew/Cellar/riscv-gnu-toolchain/master/riscv64-unknown-elf/lib\
    /var/folders/jw/1_6fksp930zdby0pkq6ybdd00000gp/T//ccu0pQ2R.o\
    -lgcc\
    --start-group\
    -lc\
    -lgloss\
    --end-group\
    -lgcc\
    /opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/crtend.o
COLLECT_GCC_OPTIONS='-v' '-o' 'main_gcc' '-mtune=rocket' '-march=rv64imafdc' '-mabi=lp64d' '-march=rv64imafdc'


/opt/homebrew/Cellar/riscv-gnu-toolchain/master/libexec/gcc/riscv64-unknown-elf/10.2.0/cc1 \
    -quiet\
    -v main.c\
    -quiet\
    -dumpbase main.c\
    -mtune=rocket\
    -march=rv64imafdc\
    -mabi=lp64d\
    -march=rv64imafdc\
    -auxbase main\
    -version\
    -o ccyXVBp5.s

/opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/../../../../riscv64-unknown-elf/bin/as\
    -v\
    --traditional-format\
    -march=rv64imafdc\
    -march=rv64imafdc\
    -mabi=lp64d\
    -o ccu0pQ2R.o\
    ccyXVBp5.s


/opt/homebrew/Cellar/riscv-gnu-toolchain/master/libexec/gcc/riscv64-unknown-elf/10.2.0/collect2\
    -plugin /opt/homebrew/Cellar/riscv-gnu-toolchain/master/libexec/gcc/riscv64-unknown-elf/10.2.0/liblto_plugin.so\
    -plugin-opt=/opt/homebrew/Cellar/riscv-gnu-toolchain/master/libexec/gcc/riscv64-unknown-elf/10.2.0/lto-wrapper\
    -plugin-opt=-fresolution=/var/folders/jw/1_6fksp930zdby0pkq6ybdd00000gp/T//ccqSI8xw.res\
    -plugin-opt=-pass-through=-lgcc\
    -plugin-opt=-pass-through=-lc\
    -plugin-opt=-pass-through=-lgloss\
    -plugin-opt=-pass-through=-lgcc\
    --sysroot=/opt/homebrew/Cellar/riscv-gnu-toolchain/master/riscv64-unknown-elf\
    -melf64lriscv\
    -o main_gcc\
    /opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/../../../../riscv64-unknown-elf/lib/crt0.o\
    /opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/crtbegin.o\
    -L/opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0\
    -L/opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/../../../../riscv64-unknown-elf/lib\
    -L/opt/homebrew/Cellar/riscv-gnu-toolchain/master/riscv64-unknown-elf/lib\
    /var/folders/jw/1_6fksp930zdby0pkq6ybdd00000gp/T//ccu0pQ2R.o\
    -lgcc\
    --start-group\
    -lc\
    -lgloss\
    --end-group\
    -lgcc\
    /opt/homebrew/Cellar/riscv-gnu-toolchain/master/lib/gcc/riscv64-unknown-elf/10.2.0/crtend.o
