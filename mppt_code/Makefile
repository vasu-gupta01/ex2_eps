
SRC = src/
BUILD = build/

$(BUILD)mppt:	$(SRC)mppt.c
	mkdir -p $(BUILD)
	gcc $< -o $@

# TODO pipe in mppt values

run:
	$(BUILD)mppt

clean:
	rm -rf $(BUILD)
