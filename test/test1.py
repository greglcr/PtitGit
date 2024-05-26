from clean import clean
import os

def test1():
    nbOk = 0

    clean()
    os.system("../../ptitGit init")
    if os.path.isdir(".ptitgit"):  nbOk += 1   # check if the repos is well initialized

    os.system('echo "42" > a') # create a file a
    os.system("../../ptitGit add a")
    os.system("../../ptitGit config author name")
    os.system("../../ptitGit commit -m'2nd commit'")

    os.system('mkdir b')
    os.system('mkdir b/c')
    os.system('echo "43" > b/c/d')
    os.system('echo "44" > b/c/e')

    os.system("../../ptitGit add b")
    os.system("../../ptitGit commit -m'3rd commit'")
    
    return (nbOk,1-nbOk)



