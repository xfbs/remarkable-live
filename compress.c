#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define KB * 1024
#define MB * 1024 KB

#define WIDTH 1408
#define HEIGHT 1920

static uint8_t framebuffer[WIDTH * HEIGHT];

void fill_framebuffer(const char *name) {
    FILE *fbdev = fopen(name, "rb");
    assert(fbdev);

    size_t framebuffer_pos = 0;
    char buffer[1 MB];
    size_t buffer_pos = 0;
    size_t buffer_len = 0;
    size_t buffer_tot = 0;

    setbuf(fbdev, buffer);

    while(framebuffer_pos != sizeof(framebuffer)) {
        if(buffer_pos == buffer_len) {
            buffer_len = fread(buffer, 1, sizeof(buffer), fbdev);
            assert(buffer_len);
            buffer_pos = 0;
        }

        if(buffer_tot & 1) {
            framebuffer[framebuffer_pos++] = buffer[buffer_pos];
        }

        buffer_pos++;
        buffer_tot++;
    }

    fclose(fbdev);
}


int main(int argc, char *argv[]) {
    assert(argc == 2);
    fill_framebuffer(argv[1]);
    fwrite(framebuffer, 1, sizeof(framebuffer), stdout);
    return 0;
}
