default: csr cycles_sleep cycles_time infinite wfi_on wfi_off

counter: counter.c csr_0x802.s
	@gcc -Wall counter.c csr.s -o counter
csr: csr_main.c csr_0x802.s
	@gcc -Wall $^ -o $@
wfi: wfi_counter.c csr_lo_pw.s csr_hi_pw.s
	@gcc -Wall $^ -o $@
cycles_sleep: cycle_tests.c
	@gcc -Wall $^ -o $@ -D SLEEP=1
cycles_time: cycle_tests.c
	@gcc -Wall $^ -o $@
infinite: infinite.c wfi.s
	@gcc -Wall $^ -o $@
wfi_on: set_wfi.c csr_0x803.s
	@gcc -Wall $^ -o $@
wfi_off: free_wfi.c csr_0x803.s
	@gcc -Wall $^ -o $@

clean:
	@rm -rf counter csr test_csr wfi cycles_sleep cycles_time infinite
	@rm *~
	@echo "Made clean."

