alpha = 0xc0ac29b7c97c50dd

a = RealField(prec=2000)(pi)-3
rcs = []
for i in range(1, 9):
    b = (floor(a*2^(64*i)) + 2^64) % 2^64
    rcs.append(b)
    print("%016x" % (b))
