#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/mman.h>

#define KB * 1024
#define MB * 1024 KB

#define WIDTH 1408
#define HEIGHT 1920

void output_rle(uint8_t *buffer) {
    size_t pos = 0;

    size_t max = 2 * sizeof(framebuffer);

    while(pos < max) {
        uint8_t pixel = buffer[pos + 1];
        uint8_t length = 1;
        pos += 2;

        while(pos < max && buffer[pos + 1] == pixel && length < 255) {
            pos += 2;
            length++;
        }

        fputc(pixel, stdout);
        fputc(length, stdout);
    }
}


int main(int argc, char *argv[]) {
    assert(argc == 2);

    // get a file descriptor on the framebuffer
    int fd = open(argv[1], O_RDONLY);
    assert(0 <= fd);

    // map framebuffer dev into memory
    uint8_t *addr = mmap(
            NULL,
            2 * WIDTH * HEIGHT,
            PROT_READ,
            MAP_SHARED,
            fd,
            0);

    assert(addr);

    // grab data
    fill_framebuffer(addr);

    return 0;
}
