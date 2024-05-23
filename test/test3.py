from clean import clean
import os

def test3():
    nbOk = 0

    clean()
    os.system("../../petitGit init")
    if os.path.isdir(".ptitgit"):  nbOk += 1   # check if the repos is well initialized

    os.system("echo 42 > a") # create a file a
    os.system("../../petitGit add a")
    os.system("../../petitGit commit -m'bb'")
    os.system("../../petitGit config author Handsome")
    os.system("../../petitGit commit -m'bb'")

    os.system("../../petitGit branch happy")
    os.system('echo "100" > a')
    os.system("../../petitGit add a")
    os.system("../../petitGit commit -m'cc'")
    os.system("../../petitGit checkout -f happy")
    os.system("../../petitGit merge main")
    result = os.popen('cat a').read()
    if result == "42\n":
        nbOk += 1
    else:
        print(result)
    

    # TODO

    return (nbOk,2-nbOk)



