#include <stdint.h>
#include <stdio.h>

uint64_t deinterleave(uint64_t x) {
  x = x & 0x5555555555555555;
  x = (x | (x >> 1)) & 0x3333333333333333;
  x = (x | (x >> 2)) & 0x0f0f0f0f0f0f0f0f;
  x = (x | (x >> 4)) & 0x00ff00ff00ff00ff;
  x = (x | (x >> 8)) & 0x0000ffff0000ffff;
  x = (x | (x >> 16)) & 0x00000000ffffffff;
  return x;
}

uint64_t hilbert(uint64_t x) {
  /* make `inversion mark` (lower bit) and `+2 mark` (upper bit) */
  uint64_t lo = x & 0x5555555555555555;
  uint64_t hi = x & 0xaaaaaaaaaaaaaaaa;
  uint64_t marks = (lo ^ hi >> 1) | (hi & lo << 1);

  /* cummulative xor the marks to the right */
  marks ^= (marks & 0xcccccccccccccccc) >> 2;
  marks ^= (marks & 0x3030303030303030) >> 4;
  marks ^= (marks & 0x0300030003000300) >> 8;
  marks ^= (marks & 0x0003000000030000) >> 16;
  marks ^= (marks & 0x0000000300000000) >> 32;
  marks ^= (marks & 0x0000000300000000) >> 16;
  marks ^= (marks & 0x0003000300030000) >> 8;
  marks ^= (marks & 0x0303030303030300) >> 4;
  marks ^= (marks & 0x3333333333333330) >> 2;

  /* flip the 2's */
  x ^= marks & 0xaaaaaaaaaaaaaaaa;

  /* convert to U curve */
  x ^= (x & 0xaaaaaaaaaaaaaaaa) >> 1;

  /* flip even coords (this is merged with the other flipping below) */
  /*x = x & 0x3333333333333333 | (((x & 0x4444444444444444) << 1) |
                                ((x & 0x8888888888888888) >> 1));*/

  /* flip the coords where the lower bit in mark is set, also all even coords */
  uint64_t fmask = 0x4444444444444444 ^ (marks & 0x5555555555555555);
  fmask |= fmask << 1;
  x = (x & ~fmask) | (fmask & (((x & 0x5555555555555555) << 1) |
                               ((x & 0xaaaaaaaaaaaaaaaa) >> 1)));
  return x;
}

int main(int argc, char **argv) {
  int T = 4;

  if (argc == 2)
    sscanf(argv[1], "%d", &T);
  if (T > 63)
    T = 63;

  for (uint64_t i = 0; i < (1 << T); ++i)
    printf("%ld\t%ld\t%ld\t%ld\n", i, hilbert(i), deinterleave(hilbert(i)),
           deinterleave(hilbert(i) >> 1));
  return 0;
}
