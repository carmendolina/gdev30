# run on sagemath

a = (1+sqrt(5))/2
golden = a/2
#top
v0 = vector(RR, 3, [0,-golden,0.5])
v1 = vector(RR, 3, [-golden,-0.5,0])
v2 = vector(RR, 3, [0,-golden,-0.5])
v3 = vector(RR, 3, [golden,-0.5,0])
v4 = vector(RR, 3, [0.5,0,golden])
v5 = vector(RR, 3, [-0.5,0,golden])
#bottom
v6 = vector(RR, 3, [0,golden,-0.5])
v7 = vector(RR, 3, [0.5,0,-golden])
v8 = vector(RR, 3, [-0.5,0,-golden])
v9 = vector(RR, 3, [-golden,0.5, 0])
v10 = vector(RR, 3, [0,golden,0.5])
v11 = vector(RR, 3, [golden,0.5,0])

normalt0 = (v0-v1).cross_product(v1-v2)
normalt1 = (v0-v2).cross_product(v2-v3)
normalt2 = (v0-v3).cross_product(v3-v4)
normalt3 = (v0-v4).cross_product(v4-v5)
normalt4 = (v0-v5).cross_product(v5-v1)

normalt5 = (v6-v7).cross_product(v7-v8)
normalt6 = (v6-v8).cross_product(v8-v9)
normalt7 = (v6-v9).cross_product(v9-v10)
normalt8 = (v6-v10).cross_product(v10-v11)
normalt9 = (v6-v11).cross_product(v11-v7)

normalt10 = (v1-v8).cross_product(v8-v2)
normalt11 = (v2-v8).cross_product(v8-v7)
normalt12 = (v2-v7).cross_product(v7-v3)
normalt13 = (v3-v7).cross_product(v7-v11)
normalt14 = (v3-v11).cross_product(v11-v4)
normalt15 = (v4-v11).cross_product(v11-v10)
normalt16 = (v4-v10).cross_product(v10-v5)
normalt17 = (v5-v10).cross_product(v10-v9)
normalt18 = (v5-v9).cross_product(v9-v1)
normalt19 = (v1-v9).cross_product(v9-v8)

sumv0 = normalt0+normalt1+normalt2+normalt3+normalt4
l_sumv0 = sqrt((sumv0[0]*sumv0[0]) + (sumv0[1]*sumv0[1]) + (sumv0[2]*sumv0[2]))
normalv0 = sumv0 / l_sumv0

sumv1 = normalt0+normalt4+normalt10+normalt18+normalt19
normalv1 = sumv1 / l_sumv0

sumv2 = normalt0+normalt1+normalt10+normalt11+normalt12
normalv2 = sumv2 / l_sumv0

sumv3 = normalt1+normalt2+normalt12+normalt13+normalt14
normalv3 = sumv3 / l_sumv0

sumv4 = normalt2+normalt3+normalt14+normalt15+normalt16
normalv4 = sumv4 / l_sumv0

sumv5 = normalt3+normalt4+normalt16+normalt17+normalt18
normalv5 = sumv5 / l_sumv0

sumv6 = normalt5+normalt6+normalt7+normalt8+normalt9
normalv6 = sumv6 / l_sumv0

sumv7 = normalt5+normalt9+normalt11+normalt12+normalt13
normalv7 = sumv7 / l_sumv0

sumv8 = normalt5+normalt6+normalt10+normalt11+normalt19
normalv8 = sumv8 / l_sumv0

sumv9 = normalt6+normalt7+normalt17+normalt18+normalt19
normalv9 = sumv9 / l_sumv0

sumv10 = normalt7+normalt8+normalt15+normalt16+normalt17
normalv10 = sumv10 / l_sumv0

sumv11 = normalt8+normalt9+normalt13+normalt14+normalt15
normalv11 = sumv11 / l_sumv0

print(normalv0)
print(normalv1)
print(normalv2)
print(normalv3)
print(normalv4)
print(normalv5)
print(normalv6)
print(normalv7)
print(normalv8)
print(normalv9)
print(normalv10)
print(normalv11)