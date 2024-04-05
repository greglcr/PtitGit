class Color:
    reset = "\033[0m"
    red = "\033[31m"
    green = "\033[32m"
    yellow = "\033[33m"
    blue = "\033[34m"
    magenta = "\033[35m"
    cyan = "\033[36m"
    white = "\033[37m"


nbOk = 0
nbFail = 0
print(Color.blue + "start tests" + Color.reset)

print(Color.magenta + "test 1" + Color.reset)
print("....")

print(Color.blue + "end of tests" + Color.reset)
print("ok :",nbOk)
print("fail :",nbFail)
