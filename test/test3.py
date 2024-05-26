from clean import clean
import os

def test3():
    nbOk = 0

    clean()
    os.system("../../ptitGit init")
    if os.path.isdir(".ptitgit"):  nbOk += 1   # check if the repos is well initialized

    os.system("echo 42 > a") # create a file a
    os.system("../../ptitGit add a")
    os.system("../../ptitGit commit -m'bb'")
    os.system("../../ptitGit config author Handsome")
    os.system("../../ptitGit commit -m'bb'")

    os.system("../../ptitGit branch happy")
    os.system('echo "100" > a')
    os.system("../../ptitGit add a")
    os.system("../../ptitGit commit -m'cc'")
    os.system("../../ptitGit checkout -f happy")
    os.system("../../ptitGit merge main")
    result = os.popen('cat a').read()
    if result == "There is a conflict in this file. Please merge manually!\n\nHere is the old file:\n42\n\n\nHere is the new file:\n100\n":
        nbOk += 1
    else:
        print(result)
    
    return (nbOk,2-nbOk)



