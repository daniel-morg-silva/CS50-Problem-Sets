from cs50 import get_int

while True:
    n = get_int("Height: ")

    if 0 < n and n <= 8:
        break

for _ in range(n):
    print(" " * (n - _ - 1) + "#" * (_ + 1) + "  " + "#" * (_ + 1))
