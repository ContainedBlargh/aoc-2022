from random import randint
from sys import argv

for i in range(int(argv[1])):
    l1 = randint(0, 100)
    r1 = randint(0, 100)
    if l1 > r1:
        (l1, r1) = r1, l1
    l2 = randint(0, 100)
    r2 = randint(0, 100)
    if l2 > r2:
        (l2, r2) = r2, l2
    print(f"{l1}-{r1},{l2}-{r2}")
    
