from clean import clean
import os
import subprocess

def test2():
    nbOk = 0

    clean()
    os.system("../../petitGit init")
    if os.path.isdir(".ptitgit"):  nbOk += 1   # check if the repos is well initialized

    p = subprocess.Popen(["../../petitGit", "server"], cwd="../server");
    os.system("../../petitGit init-remote localhost 9999")

    p.terminate();

    return (nbOk,1-nbOk)



