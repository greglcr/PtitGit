from clean import clean
import os
import subprocess

def test2():
    nbOk = 0

    clean()
    os.system("../../petitGit init .")
    if os.path.isdir(".ptitgit"):  nbOk += 1   # check if the repos is well initialized
    else: print("BAD INIT")

    p = subprocess.Popen(["../../petitGit", "server"], cwd="../server");
    r = os.popen("../../petitGit init-remote localhost 9999").read()
    if r == "repos created with id 1\n": nbOk += 1
    else: print(r)

    os.system("../../petitGit config url localhost")
    os.system("../../petitGit config port 9999")
    os.system("../../petitGit config repos-id 1")

    r = os.popen("../../petitGit push").read()
    if r == "nothing to push. This branch is already synchronized with the server.\n": nbOk += 1
    else: print(r)

    r = os.popen("../../petitGit pull").read()
    if r == "Your branch 'main'is up to date\n": nbOk += 1
    else: print(r)

    os.system('echo "v1" > a')
    os.system("../../petitGit add a")
    os.system("../../petitGit commit -m'2nd commit'")

    r = os.popen("../../petitGit push").read()
    if r == "Well received !\n": nbOk += 1
    else: print(r)

    os.system('ls -a1 | grep -xvE "README.md|\.|\.\." | xargs rm -rf')
    os.system("../../petitGit init")
    os.system("../../petitGit config url localhost")
    os.system("../../petitGit config port 9999")
    os.system("../../petitGit config repos-id 1")
    r = os.popen("../../petitGit pull").read()
    if r == "Download and update (the remote repository was a few commits ahead) on branch 'main'\n": nbOk += 1
    else: print(r)


    os.system('ls -a1 | grep -xvE "README.md|\.|\.\." | xargs rm -rf')
    os.makedirs("A")
    os.makedirs("B")
    os.system("../../petitGit init A")
    os.system("../../petitGit init B")
    os.chdir("A")
    os.system("../../../petitGit config url localhost")
    os.system("../../../petitGit config port 9999")
    os.system("../../../petitGit config repos-id 1")
    #os.system("../../../petitGit pull")
    #os.system('echo "v2" > a')
    #os.system("../../../petitGit add a")
    #os.system("../../../petitGit commit -m'3rd commit'")
    #os.chdir("../B")
    os.chdir("..")



    p.terminate();
    return (nbOk,6-nbOk)



