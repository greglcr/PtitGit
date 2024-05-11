from clean import clean
import os

def test1():
    nbOk = 0

    clean()
    os.system("../../petitGit init")
    if os.path.isdir(".ptitgit"):  nbOk += 1   # check if the repos is well initialized

    os.system('echo "42" > a') # create a file a
    os.system("../../petitGit add a")
    os.system("../../petitGit commit -m'bb'")
    

    # TODO

    return (nbOk,1-nbOk)



