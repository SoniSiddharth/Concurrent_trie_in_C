CC      := gcc
CCFLAGS := -Wall -Werror -pthread -g

B_TARGETS := test_trie_single_threaded test_trie_s_lock test_trie_rw_lock test_trie_hoh_lock
R_TARGETS := single_threaded s_lock rw_lock hoh_lock
ST_TARGETS := single_threaded
MT_TARGETS := s_lock rw_lock hoh_lock

test_all: $(R_TARGETS)

test_trie_rw_lock.c: test_trie_s_lock.c
	@tail -n +2 $< > $@
	@echo Updated $@

test_trie_hoh_lock.c: test_trie_s_lock.c
	@tail -n +3 $< > $@
	@echo Updated $@

$(B_TARGETS): % : %.c
	@echo Building
	$(CC) -o $@ $^ $(CCFLAGS)

$(ST_TARGETS): % : test_trie_%
	@rm -f -r ./.testout
	@mkdir .testout
	@echo "\n-------------------------------------------"
	@echo "Program Output($<):"
	@echo "-------------------------------------------"
	@-valgrind --leak-check=full --show-leak-kinds=all --log-file="./.testout/memtest_out_$<" ./$<
	@echo "\n-------------------------------------------"
	@echo "Valgrind Output($<):"
	@echo "-------------------------------------------"
	@cat ./.testout/memtest_out_$<

$(MT_TARGETS): % : test_trie_%
	@rm -f -r ./.testout
	@mkdir .testout
	@echo "\n-------------------------------------------"
	@echo "Program Output($<):"
	@echo "-------------------------------------------"
	@-valgrind --leak-check=full --show-leak-kinds=all --log-file="./.testout/memtest_out_$<" ./$<
	@echo "\n-------------------------------------------"
	@echo "Valgrind Memcheck Output($<):"
	@echo "-------------------------------------------"
	@cat ./.testout/memtest_out_$<
	@-valgrind --tool=helgrind --log-file="./.testout/heltest_out_$<" ./$< > /dev/null
	@echo "\n-------------------------------------------"
	@echo "Valgrind Helgrind Output($<):"
	@echo "-------------------------------------------"
	@cat ./.testout/heltest_out_$<

clean: 
	rm -f $(B_TARGETS)
	rm -f -r ./.testout
	rm -f -r ./test_trie_rw_lock.c
	rm -f -r ./test_trie_hoh_lock.c

# Add your rules after this point. Don't edit the things above:
COMPARE_FOLDER := ./comparison/
generate_workload:
	@echo "generating workload....."
	@cd comparison && python3 script_generator.py

read_write:
	@echo "running on read-write 50-50 workload and generating plot......"
	@$(CC) -o $(COMPARE_FOLDER)s_lock $(COMPARE_FOLDER)s_lock.c $(CCFLAGS)
	@cd comparison && ./s_lock 0 && cd ..
	@$(CC) -o $(COMPARE_FOLDER)rw_lock $(COMPARE_FOLDER)rw_lock.c $(CCFLAGS)
	@cd comparison && ./rw_lock 0 && cd ..
	@$(CC) -o $(COMPARE_FOLDER)hoh_lock $(COMPARE_FOLDER)hoh_lock.c $(CCFLAGS)
	@cd comparison && ./hoh_lock 0 && python3 plots.py

read_intensive:
	@echo "running on read intensive workload and generating plot......"
	@$(CC) -o $(COMPARE_FOLDER)s_lock $(COMPARE_FOLDER)s_lock.c $(CCFLAGS)
	@cd comparison && ./s_lock 1 && cd ..
	@$(CC) -o $(COMPARE_FOLDER)rw_lock $(COMPARE_FOLDER)rw_lock.c $(CCFLAGS)
	@cd comparison && ./rw_lock 1 && cd ..
	@$(CC) -o $(COMPARE_FOLDER)hoh_lock $(COMPARE_FOLDER)hoh_lock.c $(CCFLAGS)
	@cd comparison && ./hoh_lock 1 && python3 plots.py

write_intensive:
	@echo "running on write intensive workload and generating plot......"
	@$(CC) -o $(COMPARE_FOLDER)s_lock $(COMPARE_FOLDER)s_lock.c $(CCFLAGS)
	@cd comparison && ./s_lock 2 && cd ..
	@$(CC) -o $(COMPARE_FOLDER)rw_lock $(COMPARE_FOLDER)rw_lock.c $(CCFLAGS)
	@cd comparison && ./rw_lock 2 && cd ..
	@$(CC) -o $(COMPARE_FOLDER)hoh_lock $(COMPARE_FOLDER)hoh_lock.c $(CCFLAGS)
	@cd comparison && ./hoh_lock 2 && python3 plots.py