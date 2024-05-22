import os

def clean():
    try:
        os.chdir("test")
    except:
        pass
    
    if os.getcwd().split("/")[-1] != "result-test":
        os.chdir("result-test")

    os.system('ls -a1 | grep -xvE "README.md|\.|\.\." | xargs rm -rf')
    
    if not os.path.exists("../server"):
        os.makedirs("../server")
    os.system("rm -rf ../server/*")



