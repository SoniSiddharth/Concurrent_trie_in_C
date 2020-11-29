CC := gcc

loop:
	@$(CC) -o output main.c
	@./output loop 50 10000
	@python3 plots.py

local:
	@$(CC) -o output main.c
	@./output local 100 10000
	@python3 plots.py

random:
	@$(CC) -o output main.c
	@./output random 100 10000
	@python3 plots.py
clean:
	@rm -f output
	@rm -f plots.csv