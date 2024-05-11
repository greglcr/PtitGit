from test1 import test1

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

def exec_test(name, fct):
    global nbOk, nbFail
    print(Color.magenta + "test : " + name + Color.reset)
    ok_test, fail_test = fct()
    nbOk += ok_test
    nbFail += fail_test

exec_test("test 1", test1)

print(Color.blue + "end of tests" + Color.reset)
print("ok :",nbOk)
print("fail :",nbFail)
