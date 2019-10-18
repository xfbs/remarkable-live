all: compress fbtopng fb8topng

compress: compress.c
	$(CC) -o $@ $<

fbtopng: fbtopng.c
	$(CC) -o $@ $< -lpng

fb8topng: fb8topng.c
	$(CC) -o $@ $< -lpng
