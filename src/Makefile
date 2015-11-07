cc = gcc

of = ../Bin

exe = SeedCup.exe

make:
	$(cc) thread.c cssismp_send.c ../utils.c ../parser/parser.c -o $(exe) -levent -lpthread
	
clean:
	rm -rf $(exe)
