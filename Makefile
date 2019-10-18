all: compress compress2 compress3 fbtopng fb8topng

compress: compress.c
	$(CC) $(CFLAGS) -o $@ $<

compress2: compress2.c
	$(CC) $(CFLAGS) -o $@ $<

compress3: compress3.c
	$(CC) $(CFLAGS) -o $@ $<

fbtopng: fbtopng.c
	$(CC) $(CFLAGS) -o $@ $< -lpng

fb8topng: fb8topng.c
	$(CC) $(CFLAGS) -o $@ $< -lpng
