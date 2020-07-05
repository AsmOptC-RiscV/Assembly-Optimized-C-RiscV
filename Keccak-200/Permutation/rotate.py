#! /usr/env python3

def index(x, y):
    return (((x) % 5) + 5 * ((y) % 5))


def dest(x, y):
    return index(0 * x + 1 * y, 2 * x + 3 * y)


destinations = [x for x in range(25)]
print(destinations)
for x in range(0, 5):
    for y in range(0, 5):
        destinations[index(x, y)] = dest(x, y)
        # print(x, y, ": ", index(x, y), " => ", dest(x, y))

print(destinations)

start = 1
print('x = A[{}]'.format(1))
for x in range(24):
    save = start
    start = destinations.index(start)
    if x < 23:
        print('A[{}] = A[{}];'.format(save, start))
    else:
        print('A[{}] = x;'.format(save))

print('--------------------------------------')
for x in range(5):
    x1 = (x + 1) % 5
    x2 = (x + 4) % 5
    print('D[{}] = ROL32(C[{}], 4) ^ C[{}]'.format(
        x, x1, x2))

print('---------------------------------------')

for y in range(5):
    for x in range(5):
        print('C[{}] = A[{}] ^ ((~A[{}]) & A[{}]);'.format(x,
                                                           index(x, y),
                                                           index(x + 1, y),
                                                           index(x + 2, y)))
    for x in range(5):
        print('A[{}] = C[{}];'.format(index(x, y),
                                      x))
