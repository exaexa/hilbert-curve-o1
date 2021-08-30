
# Const-time Hilbert curve with integer operations

The code in `intbert.c` computes the position of *n*-th "node" of the 2D Hilbert
curve in *O(1)* time, using only bitwise integer operation.

The method is loosely based on transforming the U-curve (aka Gray code) with
consecutive accumulated rotations and flips, then deinterleaving (as in Morton
Z-curve).

To run a demo (order 14 curve):
```sh
make intbert
./intbert 14 > hilbert14.tsv
```

To plot the results (here using `R`):
```r
d <- read.table('hilbert14.tsv')[,3:4]

# plot interactively
plot(d, pch=20, cex=.5, col=rainbow(2^14))
lines(d)

# plot to SVG
svg('hilbert14.svg', 20, 20)
plot(d, type='l', lwd=2, xlab='', ylab='')
points(d, pch=20, col=rainbow(2^14))
dev.off()
```

**TODOs:**

- reverse transform (2D coords to 1D Hilbert)
- optimized versions (x86 asm, shaders, lower-than-64bit versions, ...)

## License

Public domain.

(But I'll be happy if you link back to this repo.)
