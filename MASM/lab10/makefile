.PHONY : sse fpu sin_cmp root disasm_sse disasm_fpu clean

sse :
	@echo
	@echo "SSE: "
	g++ main.cpp -DTIME -masm=intel -mno-80387 -O0 -o sse.exe
	./sse.exe

fpu :
	@echo
	@echo "FPU: "
	g++ main.cpp -DTIME -masm=intel -m80387 -mno-sse -mno-avx -O0 -o fpu.exe
	./fpu.exe

sin_cmp :
	@echo
	g++ main.cpp -DSIN -masm=intel -mno-80387 -O0 -o sin_cmp.exe
	./sin_cmp.exe

root :
	@echo
	g++ main.cpp -DROOT -masm=intel -mno-80387 -O0 -o root.exe
	./root.exe

disasm_sse:
	g++ -DTIME -masm=intel -mno-80387 -O0 -c main.cpp
	objdump -M intel-mnemonic -d main.o  > dis_sse.txt

disasm_fpu:
	g++ -DTIME -masm=intel -m80387 -mno-sse -O0 -c main.cpp
	objdump -M intel-mnemonic -d main.o  > dis_fpu.txt

clean :
	rm -rf *.o *.exe *.out dis_main.txt