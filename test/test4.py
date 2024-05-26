from clean import clean
import os
import subprocess


def test4():
    port = "9970"
    nbOk = 0

    clean()

    p = subprocess.Popen(["../../ptitGit", "server",port], cwd="../server");
    r = os.popen("../../ptitGit init-remote localhost "+port).read()
    if r == "repos created with id 1\n": nbOk += 1
    else: print(r)

    os.system("../../ptitGit init .")
    os.system("../../ptitGit config url localhost")
    os.system("../../ptitGit config port "+port)
    os.system("../../ptitGit config repos-id 1")
    os.system("../../ptitGit config author name1")
    os.system("../../ptitGit commit -m'a'")
    os.system("../../ptitGit commit -m'b'")
    os.system("../../ptitGit branch machin")
    os.system('echo "24816" > foo')
    os.system("../../ptitGit add foo")
    os.system("../../ptitGit commit -m'c'")
    r = os.popen("../../ptitGit push").read()
    if r=="Well received !\n": nbOk += 1
    else: print("ERREUR :",r)

    os.system("rm -rf .ptitgit")
    os.system("rm foo")

    os.system("../../ptitGit init .")
    os.system("../../ptitGit config url localhost")
    os.system("../../ptitGit config port "+port)
    os.system("../../ptitGit config repos-id 1")
    os.system("../../ptitGit config author name2")
    r = os.popen("../../ptitGit pull").read()
    if r=="a new branch 'machin' was created\nDownload and update (the remote repository was a few commits ahead) on branch 'main'\nHEAD is attached to branch main\n": nbOk += 1
    else: print("ERREUR",r)

    r = os.popen("../../ptitGit checkout -f machin").read()
    if r=="HEAD is attached to branch machin\n": nbOk+=1
    else: print("ERREUR :", r)


    r = os.popen("cat foo").read()
    if r=="24816\n": nbOk+=1
    else: print("ERREUR :", r)

    p.terminate();
    return (nbOk,5-nbOk)
