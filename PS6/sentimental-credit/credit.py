import sys
from re import match


def main():
    number = input("Number: ")

    if not len(number) in [13, 15, 16]:
        print("INVALID")
        sys.exit(404)

    if not initial_check(number):
        print("INVALID")
        sys.exit(405)

    if not luhn(number):
        print("INVALID")
        sys.exit(406)

    print(initial_check(number))


def get_int(prompt):
    while True:
        try:
            return int(input(prompt))
        except ValueError:
            print("input must be of type int")


def initial_check(num):

    if not match(r'3[4, 7]', num) == None:
        return 'AMEX'
    elif not match(r'5[1-5]', num) == None:
        return 'MASTERCARD'
    elif not match(r'4', num) == None:
        return 'VISA'
    else:
        return False


def luhn(num):
    sum = 0
    helper = 1
    for _ in range(len(num) - 1, -1, -1):
        if helper == 0 and (int(num[_]) * 2) < 10:
            sum += (int(num[_]) * 2)
            helper += 1
        elif helper == 0 and (int(num[_]) * 2) > 9:
            sum += 1 + (int(num[_]) * 2) % 10
            helper += 1
        else:
            sum += int(num[_])
            helper -= 1

    sum = str(sum)
    if sum[len(sum) - 1] == '0':
        return True
    else:
        return False


main()
