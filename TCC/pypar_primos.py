def primo(n):
    if n < 2:
        return False
    if n % 2 == 0:
        return n == 2
    limite = int(n**0.5) + 1
    for i in range(3, limite, 2):
        if n % i == 0:
            return False
    return True

if __name__ == "__main__":
    NUM = 200_000
    count = sum(1 for n in range(NUM) if primo(n))
    print(f"Numeros primos menores que {NUM}: {count}")   