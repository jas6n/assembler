        lw	0	6	mcand
        lw      0       7       mplier
        lw      0       2       bit
        lw      0       3       shift
beg     nor     7       7       2
        nor     3       3       5
        nor     2       5       2
        beq     2       3       ans
else    add     6       6       6
        lw      0       5       inc
        add     3       3       3   
        lw      0       5       mask
        add     5       5       5
        sw      0       5       mask
        nor     7       7       4
        nor     5       5       5
        nor     5       4       4
        beq     4       0       end
        beq     0       0       beg 
end     halt
ans     add     1       6       1
        beq     0       0       else
mcand	.fill	6203
mplier	.fill	1429
bit     .fill   0
shift   .fill   1
inc     .fill   1
mask    .fill   65535