from clean import clean
import os

def test1():
    nbOk = 0

    clean()
    os.system("../../petitGit init")
    if os.path.isdir(".ptitgit"):  nbOk += 1   # check if the repos is well initialized

    os.system('echo "42" > a') # create a file a
    os.system("../../petitGit add a")
    os.system("../../petitGit commit -m'2nd commit'")

    os.system('mkdir b')
    os.system('mkdir b/c')
    os.system('echo "43" > b/c/d')
    os.system('echo "44" > b/c/e')

    os.system("../../petitGit add b/c/d")
    os.system("../../petitGit commit -m'3rd commit'")



    

    # TODO

    return (nbOk,1-nbOk)



