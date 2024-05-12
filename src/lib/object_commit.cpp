#include "hashing.h"
#include "object.h"
#include "object_commit.h"
#include "object_tree.h"
#include "object_file.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

Commit::Commit(Tree parentTree, std::vector<std::string> parentCommitsHash, std::string commitAuthor, std::string committer, std::string message, std::string gpgsig){
    this->commitAuthor = commitAuthor;
    this->committer = committer;
    this->parentCommitsHash = parentCommitsHash;
    this->parentTree = parentTree;
    this->gpgsig = gpgsig;
    this->message = message;

    this->content = "tree " + parentTree.getHashedContent() + "\n";
    for(long long ii = 0; ii < (long long)parentCommitsHash.size(); ii++) this->content += ("parent " + parentCommitsHash[ii] + "\n" );
    this->content += ("author " + commitAuthor + "\ncommitter " + committer + "\ngpgsig " + gpgsig + "\n" + message + "\n");
    
    this->hashedContent = hashString(this->content);
}

Commit Commit::fromfile(std::string hashedContent){
    const std::ifstream input_stream(get_path_to_object(hashedContent), std::ios_base::binary);

    if (input_stream.fail()) {
        throw std::runtime_error("Failed to open file");
    }

    std::stringstream buffer;
    buffer << input_stream.rdbuf();

    return Commit::fromstring(buffer.str());
}

Commit Commit::fromstring(std::string commitContent){
    Tree A = Tree();
    Commit X = Commit(A);
    long long abc = commitContent.find(' ');
    long long cba = commitContent.find('\n');
    X.parentTree = Tree(get_path_to_object(commitContent.substr(abc+1,cba-abc-1)));
    long long mm = cba+1;
    abc = commitContent.find(' ',mm);cba = commitContent.find('\n',mm);
    while(commitContent.substr(mm,abc-mm)=="parent"){
        std::string xyz = commitContent.substr(abc+1,cba-abc-1);
        X.parentCommitsHash.push_back(xyz);
        mm = cba+1;abc = commitContent.find(' ',mm);cba = commitContent.find('\n',mm);
    }
    X.commitAuthor = commitContent.substr(abc+1,cba-abc-1);
    mm = cba+1;abc = commitContent.find(' ',mm);cba = commitContent.find('\n',mm);
    X.committer = commitContent.substr(abc+1,cba-abc-1);
    mm = cba+1;abc = commitContent.find(' ',mm);cba = commitContent.find('\n',mm);
    X.gpgsig = commitContent.substr(abc+1,cba-abc-1);
    mm = cba+1;cba = commitContent.find('\n',mm);
    X.message = commitContent.substr(mm,cba-mm);

    return X;
}