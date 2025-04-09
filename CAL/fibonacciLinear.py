def fib(n):
    if n == 1 or n == 2:
        return 1
    fib0 = 1
    fib1 = 1
    sum_return = 1
    for i in range (n - 2):
        sum_return = fib0 + fib1
        fib0 = fib1
        fib1 = sum_return

    return sum_return
