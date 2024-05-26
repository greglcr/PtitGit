from clean import clean
import os
import subprocess

def test2():
    nbOk = 0

    clean()
    os.system("../../ptitGit init .")
    if os.path.isdir(".ptitgit"):  nbOk += 1   # check if the repos is well initialized
    else: print("BAD INIT")

    p = subprocess.Popen(["../../ptitGit", "server"], cwd="../server");
    r = os.popen("../../ptitGit init-remote localhost 9999").read()
    if r == "repos created with id 1\n": nbOk += 1
    else: print(r)

    os.system("../../ptitGit config url localhost")
    os.system("../../ptitGit config port 9999")
    os.system("../../ptitGit config repos-id 1")
    os.system("../../ptitGit config author name1")

    r = os.popen("../../ptitGit push").read()
    if r == "nothing to push. This branch is already synchronized with the server.\n": nbOk += 1
    else: print(r)

    r = os.popen("../../ptitGit pull").read()
    if r == "Your branch 'main'is up to date\n": nbOk += 1
    else: print(r)

    os.system('echo "v1" > a')
    os.system("../../ptitGit add a")
    os.system("../../ptitGit commit -m'2nd commit'")

    r = os.popen("../../ptitGit push").read()
    if r == "Well received !\n": nbOk += 1
    else: print(r)

    os.system('ls -a1 | grep -xvE "README.md|\.|\.\." | xargs rm -rf')
    os.system("../../ptitGit init")
    os.system("../../ptitGit config url localhost")
    os.system("../../ptitGit config port 9999")
    os.system("../../ptitGit config repos-id 1")
    os.system("../../ptitGit config author name2")
    r = os.popen("../../ptitGit pull").read()
    if r == "Download and update (the remote repository was a few commits ahead) on branch 'main'\nHEAD is attached to branch main\n": nbOk += 1
    else: print(r)


    os.system('ls -a1 | grep -xvE "README.md|\.|\.\." | xargs rm -rf')
    os.makedirs("A")
    os.makedirs("B")
    os.system("../../ptitGit init A")
    os.system("../../ptitGit init B")
    os.chdir("A")
    os.system("../../../ptitGit config url localhost")
    os.system("../../../ptitGit config port 9999")
    os.system("../../../ptitGit config repos-id 1")
    os.system("../../../ptitGit config author name3")
    os.system("../../../ptitGit pull")
    os.system('echo "v2" > a')
    os.system("../../../ptitGit add a")
    os.system("../../../ptitGit commit -m'3rd commit'")
    os.chdir("../B")
    os.system("../../../ptitGit config url localhost")
    os.system("../../../ptitGit config port 9999")
    os.system("../../../ptitGit config repos-id 1")
    os.system("../../../ptitGit pull")
    os.system('echo "v3" > a')
    os.system('echo "bb" > b')
    os.system("../../../ptitGit add a")
    os.system("../../../ptitGit add b")
    os.system("../../../ptitGit config author name4")
    os.system("../../../ptitGit commit -m '4th commit'")
    r = os.popen("../../../ptitGit push").read()
    if r == "Well received !\n": nbOk += 1
    else: print(r)

    os.chdir("../A")
    r = os.popen("../../../ptitGit pull").read()
    if r == "MERGE CONFLICT on your branch 'main'!\nlocal last commit :  1d6c6bc195a8e64eaac26bb9d70d26fec3c63916bd81531a21d8f4d10be6037f\nremote last commit : 7cd11ba8ef6d60e3a134a19d286e32d894524ef4cf146691aae9061be2892b17\nlca commit :         5d87ecb636537104a249a85980dd6f848f8e0372ecc791db48f6262ea39e5ab7\nplease, merge them\n": nbOk += 1
    else: print("ERROR -->", r)
    
    r = os.popen("../../../ptitGit merge 7cd11ba8ef6d60e3a134a19d286e32d894524ef4cf146691aae9061be2892b17").read()
    if r == "There is a conflict in file \"a\"\nThe hash of the commit for branch \"main\" is now a4c5fbee8aedb226287574b203e8dea24a92788c9c157374050c080645194e88\n": nbOk += 1
    else: print("ERROR -->", r)

    r = os.popen("cat a").read()
    if r == "There is a conflict in this file. Please merge manually!\n\nHere is the old file:\nv2\n\n\nHere is the new file:\nv3\n": nbOk += 1
    else: print("ERROR : ", r)
    
    r = os.popen("cat b").read()
    if r == "bb\n": nbOk += 1
    else: print("ERROR : ", r)




    r = os.popen("../../../ptitGit push").read()
    if r == "Well received !\n": nbOk += 1
    
    os.chdir("../B")
    r = os.popen("../../../ptitGit pull").read()
    if r=="Download and update (the remote repository was a few commits ahead) on branch 'main'\nHEAD is attached to branch main\n": nbOk+=1


    r = os.popen("cat a").read()
    if r == "There is a conflict in this file. Please merge manually!\n\nHere is the old file:\nv2\n\n\nHere is the new file:\nv3\n": nbOk += 1
    else: print("ERROR : ", r)
    
    r = os.popen("cat b").read()
    if r == "bb\n": nbOk += 1
    else: print("ERROR : ", r)

    os.chdir("..")
    p.terminate();
    return (nbOk,15-nbOk)
