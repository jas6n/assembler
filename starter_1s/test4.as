start   beq     0       1       2 
        jalr    6       7       
        beq     5       3       end 
        noop
end     nor     0       1       2 
apple   noop
zee      halt
        .fill    -9
        .fill    100