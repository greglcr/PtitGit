#include "hashing.h"
#include "object.h"
#include "object_commit.h"
#include "object_tree.h"
#include "object_file.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

Commit::Commit(std::vector<Commit> parentCommits = {}, Tree parentTree, std::string commitAuthor = "", std::string committer ="PtiteGit Team", std::string message="New commit", std::string gpgsig=""){
    this->commitAuthor = commitAuthor;
    this->committer = committer;
    this->parentCommits = parentCommits;
    this->parentTree = parentTree;
    this->gpgsig = gpgsig;
    this->message = message;

    this->content = "tree " + parentTree.getHashedContent() + "\n";
    for(long long ii = 0; ii < parentCommits.size(); ii++) this->content += ("parent " + parentCommits[ii].getHashedContent() + "\n" );
    this->content += ("author " + commitAuthor + "\ncommitter " + committer + "\ngpgsig " + gpgsig + "\n" + message + "\n");
    
    this->hashedContent = hashString(this->content);
}
