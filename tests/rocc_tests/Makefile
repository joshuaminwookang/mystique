charcount: charcount.c
	@gcc -Wall $^ -o $@
debug: rocc_debug.c debug.s
	@gcc -Wall $^ -o $@
ephelia: ephelia.c
	@gcc -Wall $^ -o $@
clean:
	@rm -rf debug charcount ephelia
	@rm *~
	@echo "Made clean."

