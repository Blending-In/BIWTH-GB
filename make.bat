lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o main.o main.c sprites/homie.c -Isprites
lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -o main.gb main.o homie.o