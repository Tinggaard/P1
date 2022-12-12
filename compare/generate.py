#!/usr/bin/env python3

import random

N = 10

t = open("files/coordinates.csv", "w")
for n in range(N-1):
    lat = random.uniform(50,60)
    lon = random.uniform(8,10)
    t.write(f"store{n+1}, {lat}, {lon}\n")
lat = random.uniform(50,60)
lon = random.uniform(8,10)
t.write(f"store{N}, {lat}, {lon}")
t.close()

t = open("files/discounts.csv", "w")
for n in range(N-1):
    t.write(f"store{n+1}, vare{n+1}, {n+1}\n")
t.write(f"store{N}, vare{N}, {N+1}")
t.close()

t = open("files/normal_prices.csv", "w")
for n in range(N-1):
    t.write(f"vare{n+1}, {n+2}\n")
t.write(f"vare{N}, {N+2}")
t.close()

t = open("files/shopping_list.csv", "w")
for n in range(N-1):
    t.write(f"vare{n+1}\n")
t.write(f"vare{N}")
t.close()
