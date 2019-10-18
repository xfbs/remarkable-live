#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/mman.h>

#define KB * 1024
#define MB * 1024 KB

#define WIDTH 1408
#define HEIGHT 1920

static uint8_t framebuffer[WIDTH * HEIGHT];

void fill_framebuffer(uint8_t *buffer) {
    for(size_t i = 0; i < sizeof(framebuffer); i++) {
        framebuffer[i] = buffer[2 * i + 1];
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

    // output data
    fwrite(framebuffer, 1, sizeof(framebuffer), stdout);
    return 0;
}
