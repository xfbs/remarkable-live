all: compress compress2 fbtopng fb8topng

compress: compress.c
	$(CC) -o $@ $<

compress2: compress2.c
	$(CC) -g -o $@ $<

fbtopng: fbtopng.c
	$(CC) -o $@ $< -lpng

fb8topng: fb8topng.c
	$(CC) -o $@ $< -lpng
