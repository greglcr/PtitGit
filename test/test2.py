from clean import clean
import os
import subprocess

def test2():
    nbOk = 0

    clean()
    os.system("../../petitGit init .")
    if os.path.isdir(".ptitgit"):  nbOk += 1   # check if the repos is well initialized

    p = subprocess.Popen(["../../petitGit", "server"], cwd="../server");
    r = os.popen("../../petitGit init-remote localhost 9999").read()
    if r == "repos created with id 1\n": nbOk += 1

    os.system("../../petitGit config url localhost")
    os.system("../../petitGit config port 9999")
    os.system("../../petitGit config repos-id 1")

    r = os.popen("../../petitGit push").read()
    if r == "nothing to push. This branch is already synchronized with the server.\n": nbOk += 1

    r = os.popen("../../petitGit pull").read()
    if r == "Your branch 'main'is up to date\n": nbOk += 1

    os.system('echo "v1" > a')
    os.system("../../petitGit add a")
    os.system("../../petitGit commit -m'2nd commit'")

    r = os.popen("../../petitGit push").read()
    print("client : ",r)

    p.terminate();
    return (nbOk,4-nbOk)



