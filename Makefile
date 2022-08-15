
all: ./bin/boot.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1024 count=1 >> ./bin/os.bin

./bin/boot.bin: ./src/boot/boot.asm
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

clean:
	rm -rf ./bin/os.bin
	rm -rf ./bin/boot.bin