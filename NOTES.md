# Notes

## Setup SSH

To find out how to connect to your remarkable, go into Settings -> About. On
the right hand side, under *GPLv3 Compliance*, is a little note containing
information of the current IP address and root password of your device.

I used this to connect to my device, copy my `id_rsa.pub` file over into
`/root/.ssh/authorized_keys`, and made sure to fix permissions. Then I added an
entry into my local `~/.ssh/config` to let me connect to my device easily.

```ssh
Host remarkable
    HostName 192.168.1.140
    User root
```

Like this, I can connect to it simply with the following.

    ssh remarkable

## Getting an image off the device

It is possible to read the contents of the screen simply by reading from the
frame buffer file `/dev/fb0`.

    cat /dev/fb0 > framebuffer
    scp remarkable:/home/root/framebuffer .

It is possible to get information about the framebuffer from the sysfs.

    $ cat /sys/class/graphics/fb0/virtual_size
    1408,3840
    $ cat /sys/class/graphics/fb0/bits_per_pixel
    16

With this information, I can analyse the framebuffer image I got off the device. The framebuffer image I got off it has a size of 10813440 bytes.

    $ ls -la framebuffer
    -rw-r--r--  1 pelsen  staff  10813440 Oct 18 18:14 framebuffer

The framebuffer presents itself with a virtual size of 1408 by 3840, and with 2 bytes (16 bits) per pixel, that sounds about right.

Turns out I can throw away half of that data, as the framebuffer is twice as
big as it needs be (which is why it is so ridiculously high). I can also
convert it from using two bytes per pixel to one — I'm not exactly sure what is
kept in the other half, but it appears that I don't need it.

I wrote some tools that can be used for working with the framebuffer. The
`fbtopng` script takes a raw framebuffer and converts it to a PNG image on
stdout. The `compress` script reads a framebuffer, spitting out only the 8
lower 8 pixel bits and cutting off the second half of the image (that isn't
used). The `fb8topng` tool takes the output of the `compress` tool and turns it
into a PNG image.

Compiling the `compress` tool and running it on the reMarkable shows that it
still is too slow to be useful, the raw reading is so slow that I can only get
9 frames per second. My idea is to rewrite it using `mmap`, which might be
faster.

I rewrite it using `mmap`, and much to my surprise, the version (`compress2`)
using `mmap` is slower than the original, by a factor of 2 (this is with it doing 30 iterations).

	remarkable: ~/ time ./compress /dev/fb0 > out.1

	real    0m2.428s
	user    0m0.900s
	sys     0m0.970s
	remarkable: ~/ time ./compress2 /dev/fb0 > out.2

	real    0m5.396s
	user    0m4.520s
	sys     0m0.340s

That is no good, no good at all. I don't really know why this is. One thing
that is good tho is that despite it taking higher overall time, the time spent
in the kernel (sys time) is lower. This means that something we do in userspace
is skewing the results, somehow.

I do manage to remedy the situation by figuring out how to add compiler flags,
and when compiling both with maximum optimisation enabled (`-O3` flag to
`gcc`), I get a much better result.

	remarkable: ~/ time ./compress /dev/fb0 > out.1

	real    0m2.288s
	user    0m0.930s
	sys     0m0.920s
	remarkable: ~/ time ./compress2 /dev/fb0 > out.2

	real    0m1.373s
	user    0m0.640s
	sys     0m0.340s

This looks very promising — the fact that I can get 30 copies of the
framebuffer compressed in a bit more than a second means that 30 fps video is
maybe possible after all. Ish.

## Run-length encoding

What I want to do is to somehow compress the raw framebuffer data even more. My
idea is that if I reduce the amount of I/O being done, I can get the speed to
be even better. For this, I propose using run-length encoding, because this
seems to be somewhat optimal for the use-case here. Most of the pages seem to
be made up of just white/black space, which is an optimal use case for
run-length encoding.
