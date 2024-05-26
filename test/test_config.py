from clean import clean
import os

def test_config():
    nbFails = 0

    clean()
    os.system("../../ptitGit init")

    os.system("../../ptitGit config foo 42")
    result = os.popen('../../ptitGit config foo').read()
    if result != "42\n":
        nbFails += 1

    os.system("../../ptitGit config foo 1234")
    os.system("../../ptitGit config aa bb")
    result = os.popen('../../ptitGit config aa').read()
    if result != "bb\n":
        nbFails += 1
    result = os.popen('../../ptitGit config foo').read()
    if result != "1234\n":
        nbFails += 1

    return (3-nbFails,nbFails)



