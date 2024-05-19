#include "hashing.h"
#include "repos.h"
#include "stagging_area.h"
#include "object_tree.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

bool NodeTree::operator<(const NodeTree &other) {
    return this->nodeHash < other.nodeHash;
}

void StaggingArea::construct_tree(std::string curFileHash) {


    std::string fileContent = this->repos.get_repos_content(fs::path("index") / get_path_to_object(curFileHash));

    //std::string fileType = get_next_line(fileContent);
    //The line above need changing, since I've noticed that there is now 
    //a strlen stored as well on the first line

    long long nextSpace = fileContent.find(' ');
    std::string fileType = fileContent.substr(0,nextSpace);

    if (fileType == "tree") {
        try {
            get_next_line(fileContent); //Erase file name
            while(fileContent != "") {
                std::string curLine = get_next_line(fileContent);
                std::string nextFileHash = get_object_hash(curLine);
                NodeTree curNode;
                curNode.fatherHash = curFileHash;
                curNode.nodeHash = nextFileHash;
                this->treeStaggingArea[curFileHash].push_back(nextFileHash);
                this->treeStaggingAreaReversed[nextFileHash] = curFileHash;
            }
        }
        catch (const std::runtime_error &e) {
            //Rien à faire ici
        }
    }
    else if (fileType == "blob") {
        //En fait il n'y a rien à faire
    }
    else {
        std::cerr << "Error in StaggingArea::construct_tree : invalid file type (" << fileType << ")\n";
    }

}



void StaggingArea::calc_differences(bool verbose) {

    calc_differences(repos.getWorkingFolder(), repos.get_repos_content("index/INDEX"), verbose);

}

void StaggingArea::calc_differences(fs::path curPathInWorkingArea, std::string curHashStaggingArea, bool verbose) {

    std::cout << curPathInWorkingArea << std::endl;

    Tree curTreeInWorkingArea = Tree(curPathInWorkingArea);

    std::string curContentStaggingArea = repos.get_repos_content(fs::path("index") / get_path_to_object(curHashStaggingArea));
    get_next_line(curContentStaggingArea);

    fs::path curPathStaggingArea = fs::path(get_next_line(curContentStaggingArea));
    std::cout << curPathStaggingArea << std::endl;

    std::vector<File> blobsInCurWorkingFolder = curTreeInWorkingArea.get_blobs_inside();
    std::vector<std::pair<fs::path, std::string> > pathToBlobsInCurWorkingFolder;
    for (size_t i = 0; i < blobsInCurWorkingFolder.size(); i++) {
        pathToBlobsInCurWorkingFolder.push_back(std::pair(blobsInCurWorkingFolder[i].get_file_path(), blobsInCurWorkingFolder[i].getHashedContent()));
    }

    std::vector<Tree> treesInCurWorkingFolder = curTreeInWorkingArea.get_trees_inside();
    std::vector<std::pair<fs::path, std::string> > pathToTreesInCurWorkingFolder;
    for (size_t i = 0; i < treesInCurWorkingFolder.size(); i++) {
        pathToTreesInCurWorkingFolder.push_back(std::pair(treesInCurWorkingFolder[i].get_folder_path(), treesInCurWorkingFolder[i].getHashedContent()));
    }

    std::vector<std::pair<fs::path, std::string> > pathToBlobsInCurStaggingArea;
    std::vector<std::pair<fs::path, std::string> > pathToTreesInCurStaggingArea;
    while(curContentStaggingArea != "") {
        std::string curLine = get_next_line(curContentStaggingArea);
        std::cout << curLine << std::endl;
        
        if (get_object_type(curLine) == "file") {
            pathToBlobsInCurStaggingArea.push_back(std::pair(curPathStaggingArea / get_object_path(curLine), get_object_hash(curLine)));
        }
        else {
            pathToTreesInCurStaggingArea.push_back(std::pair(curPathStaggingArea / get_object_path(curLine), get_object_hash(curLine)));
        }
    }

    std::sort(pathToBlobsInCurWorkingFolder.begin(), pathToBlobsInCurWorkingFolder.end());
    std::sort(pathToBlobsInCurStaggingArea.begin(), pathToBlobsInCurStaggingArea.end());
    int posWorkingFolder = 0, posStaggingArea = 0;
    while (posWorkingFolder < (int)pathToBlobsInCurWorkingFolder.size() || posStaggingArea < (int)pathToBlobsInCurStaggingArea.size()) {

        std::cout << posWorkingFolder << " " << posStaggingArea << std::endl;

        if (posWorkingFolder == (int)pathToBlobsInCurWorkingFolder.size()) {
            std::cout << 1 << std::endl;
            if(verbose){std::cout << "Fichier supprimé : " << pathToBlobsInCurStaggingArea[posStaggingArea].first << std::endl;}
            this->status[pathToBlobsInCurStaggingArea[posStaggingArea].first].first = "deleted";
            this->status[pathToBlobsInCurStaggingArea[posStaggingArea].first].second = pathToBlobsInCurStaggingArea[posStaggingArea].second;
            posStaggingArea++;
            
        }
        else if (posStaggingArea == (int)pathToBlobsInCurStaggingArea.size()) {
            std::cout << 2 << std::endl;
            if(verbose){std::cout << "Fichier ajouté : " << pathToBlobsInCurWorkingFolder[posWorkingFolder].first << std::endl;}
            this->status[pathToBlobsInCurWorkingFolder[posWorkingFolder].first].first = "added";
            posWorkingFolder++;
        }
        else if (pathToBlobsInCurWorkingFolder[posWorkingFolder].first > pathToBlobsInCurStaggingArea[posStaggingArea].first) {
            std::cout << 3 << std::endl;
            if(verbose){std::cout << "Fichier supprimé : " << pathToBlobsInCurStaggingArea[posStaggingArea].first << std::endl;}
            this->status[pathToBlobsInCurStaggingArea[posStaggingArea].first].first = "deleted";
            this->status[pathToBlobsInCurStaggingArea[posStaggingArea].first].second = pathToBlobsInCurStaggingArea[posStaggingArea].second;
            posStaggingArea++;
        }
        else if (pathToBlobsInCurStaggingArea[posStaggingArea].first > pathToBlobsInCurWorkingFolder[posWorkingFolder].first) {
            std::cout << 4 << std::endl;
            if(verbose){std::cout << "Fichier ajouté : " << pathToBlobsInCurWorkingFolder[posWorkingFolder].first << std::endl;}
            this->status[pathToBlobsInCurWorkingFolder[posWorkingFolder].first].first = "added";
            posWorkingFolder++;
        }   
        else if (pathToBlobsInCurStaggingArea[posStaggingArea].second != pathToBlobsInCurWorkingFolder[posWorkingFolder].second) {
            std::cout << 5 << std::endl;
            if(verbose){std::cout << "Fichier modifié : " << pathToBlobsInCurWorkingFolder[posWorkingFolder].first << std::endl;}
            this->status[pathToBlobsInCurWorkingFolder[posWorkingFolder].first].first = "modified";
            this->status[pathToBlobsInCurWorkingFolder[posWorkingFolder].first].second = pathToBlobsInCurStaggingArea[posStaggingArea].second;
            posWorkingFolder++;
            posStaggingArea++;
        }
        else {
            std::cout << 6 << std::endl;
            this->status[pathToBlobsInCurWorkingFolder[posWorkingFolder].first].first = "unchanged";
            posWorkingFolder++;
            posStaggingArea++;
        }

    }

    std::cout << 555 << std::endl;

    std::sort(pathToTreesInCurWorkingFolder.begin(), pathToTreesInCurWorkingFolder.end());
    std::sort(pathToTreesInCurStaggingArea.begin(), pathToTreesInCurStaggingArea.end());
    posWorkingFolder = 0;
    posStaggingArea = 0;
    while (posWorkingFolder < (int)pathToTreesInCurWorkingFolder.size() || posStaggingArea < (int)pathToTreesInCurStaggingArea.size()) {

        if (posWorkingFolder == (int)pathToTreesInCurWorkingFolder.size() || pathToTreesInCurWorkingFolder[posWorkingFolder].first > pathToTreesInCurStaggingArea[posStaggingArea].first) {
            if(verbose){std::cout << "Dossier supprimé : " << pathToTreesInCurStaggingArea[posStaggingArea].first << std::endl;}
            this->status[pathToTreesInCurStaggingArea[posStaggingArea].first].first = "deleted";
            this->status[pathToTreesInCurStaggingArea[posStaggingArea].first].second = pathToTreesInCurStaggingArea[posStaggingArea].second;
            posStaggingArea++;
        }
        else if (posStaggingArea == (int)pathToTreesInCurStaggingArea.size()) {
            if(verbose){std::cout << "Dossier ajouté : " << pathToTreesInCurWorkingFolder[posWorkingFolder].first << std::endl;}
            this->status[pathToTreesInCurWorkingFolder[posStaggingArea].first].first = "added";
            posWorkingFolder++;
        }
        else if (pathToTreesInCurWorkingFolder[posWorkingFolder].first > pathToTreesInCurStaggingArea[posStaggingArea].first) {
            if(verbose){std::cout << "Dossier supprimé : " << pathToTreesInCurStaggingArea[posStaggingArea].first << std::endl;}
            this->status[pathToTreesInCurStaggingArea[posStaggingArea].first].first = "deleted";
            this->status[pathToTreesInCurStaggingArea[posStaggingArea].first].second = pathToTreesInCurStaggingArea[posStaggingArea].second;
            posStaggingArea++;
        }
        else if (pathToTreesInCurStaggingArea[posStaggingArea].first > pathToTreesInCurWorkingFolder[posWorkingFolder].first) {
            if(verbose){std::cout << "Dossier ajouté : " << pathToTreesInCurWorkingFolder[posWorkingFolder].first << std::endl;}
            this->status[pathToTreesInCurWorkingFolder[posWorkingFolder].first].first = "added";
            posWorkingFolder++;
        }
        else if (pathToTreesInCurStaggingArea[posStaggingArea].second != pathToTreesInCurWorkingFolder[posWorkingFolder].second) {
            if(verbose){std::cout << "Dossier modifié : " << pathToTreesInCurWorkingFolder[posWorkingFolder].first << std::endl;}
            calc_differences(pathToTreesInCurWorkingFolder[posWorkingFolder].first, pathToTreesInCurStaggingArea[posStaggingArea].second, verbose);
            this->status[pathToTreesInCurWorkingFolder[posWorkingFolder].first].first = "modified";
            this->status[pathToTreesInCurWorkingFolder[posWorkingFolder].first].second = pathToTreesInCurStaggingArea[posStaggingArea].second;
            posWorkingFolder++;
            posStaggingArea++;
        }
        else {
            calc_differences(pathToTreesInCurWorkingFolder[posWorkingFolder].first, pathToTreesInCurStaggingArea[posStaggingArea].second, verbose);
            this->status[pathToTreesInCurWorkingFolder[posWorkingFolder].first].first = "unchanged";
            posWorkingFolder++;
            posStaggingArea++;
        }

    }

}

StaggingArea::StaggingArea(PtitGitRepos repos) {

    this->repos = repos;
    this->rootTree = repos.get_repos_content("index/INDEX");

    this->construct_tree(this->rootTree);
}

std::string StaggingArea::get_root_tree() {

    return this->rootTree;

}

void StaggingArea::add(fs::path pathToAdd) {

    if (this->status[pathToAdd].first == "unchanged") {
        std::cout << "Dernière version de cet objet déjà ajoutée" << std::endl;
        return;
    }
    if (this->status[pathToAdd].first == "") {
        std::cout << "Object invalide" << std::endl;
        return;
    }

    if (fs::is_regular_file(pathToAdd)) {

        if (this->status[pathToAdd].first == "deleted") {

            //Dans ce cas là, même pas besoin de regarder le contenu, juste on récupère son hash, on le supprime du parent puis on appel update_node
            //Mais justement on ne pas accèder au contenu vu que ça  a été supprimé.
            std::string hashedContent = this->status[pathToAdd].second;
            std::string fatherHash = this->treeStaggingAreaReversed[hashedContent];
            std::string updatedFather = delete_object(this->repos.get_repos_content(fs::path(".ptitgit/index") / get_path_to_object(fatherHash)), hashedContent).second;
            std::string hashUpdatedFather = hashString(updatedFather);
            this->write_content(updatedFather, hashUpdatedFather);
            std::string grandFatherHash = this->treeStaggingAreaReversed[fatherHash];
            update_node(grandFatherHash, fatherHash, hashUpdatedFather);

        }

        else if (this->status[pathToAdd].first == "modified") {

            //Dans ce cas là, on a besoin de l'ancien hash qu'on récupère dans status
            std::string pastHashedContent = this->status[pathToAdd].second; //C'est le hash correspondant actuellement stocké dans la stagging area
            std::string curContent = this->repos.get_repos_content(pathToAdd);
            std::string curHashedContent = hashString(curContent);
            this->write_content(curContent, curHashedContent);
            //On doit maintenant retirer l'ancien hash du père du fichier
            std::string pastFatherHash = this->treeStaggingAreaReversed[pastHashedContent];
            std::pair<std::string, std::string> infoDelete = delete_object(this->repos.get_repos_content(fs::path(".ptitgit/index") / get_path_to_object(pastFatherHash)), pastHashedContent);
            std::string lineDelete = infoDelete.first;
            std::string newFatherContent1 = infoDelete.second;
            //Maintenant il faut écrire le nouveau fichier dans la stagging area, puis update le father
            std::string newFatherContent2 = insert_new_object(newFatherContent1, "file", curHashedContent, pathToAdd.filename());
            std::string newFatherHash = hashString(newFatherContent2);
            this->write_content(newFatherContent2, newFatherHash);
            update_node(this->treeStaggingAreaReversed[pastFatherHash], pastFatherHash, newFatherHash);

        }

        else if (this->status[pathToAdd].first == "added") {
            //Dans ce cas là, on a rien à supprimer, tout est à ajouter
            std::string curContent = this->repos.get_repos_content(pathToAdd); //On récupère le contenu directement dans le working folder
            std::string curHashedContent = hashString(curContent);
            this->write_content(curContent, curHashedContent);
            //Il faut trouver le hash du père, et ça c'est compliqué...
            //En fait tous les trucs avant étaient dans la stagging area déjà donc c'était simple. La ça devient plus compliqué. 
            //Première idéee : On construit l'arbre en partant du father, puis on supprime le hahs actuel pour retrouver l'ancienne version qui est dans la stagging area
            //Puis on fait les maj comme on a besoin
            fs::path fatherPath = pathToAdd.parent_path();
            Tree fatherTree = Tree(this->repos.getWorkingFolder() / fatherPath);
            std::string pastFatherContent = fatherTree.getContent();
            std::string pastFatherHash = fatherTree.getHashedContent();
            std::string newFatherContent = insert_new_object(pastFatherContent, "file", curHashedContent, pathToAdd.filename());
            std::string newFatherHash = hashString(newFatherContent);
            this->write_content(newFatherContent, newFatherHash);
            this->update_node(this->treeStaggingAreaReversed[pastFatherHash], pastFatherHash, newFatherHash);
        }
    }

}



void StaggingArea::add_all() {

    Tree rootTree = Tree(this->repos.getWorkingFolder());

    std::ofstream INDEX(this->repos.getWorkingFolder() / ".ptitgit/index/INDEX");
    INDEX << rootTree.getHashedContent();
    INDEX.close();

    add_all(Tree(this->repos.getWorkingFolder()));

}

void StaggingArea::add_all(Tree curTree) {

    this->write_content(curTree);

    std::vector<File> filesInside = curTree.get_blobs_inside();
    for (size_t i = 0; i < filesInside.size(); i++) {
        this->write_content(filesInside[i]);
    }

    std::vector<Tree> treesInside = curTree.get_trees_inside();
    for(size_t i = 0; i < treesInside.size(); i++) {
        this->add_all(treesInside[i]);
    }

}

void StaggingArea::write_content(Object curObject) {

    write_content(curObject.getContent(), curObject.getHashedContent());

}

void StaggingArea::write_content(std::string content, std::string hashedContent) {

    if (!fs::exists(get_folder_to_object(hashedContent))) {
        fs::create_directory(this->repos.getWorkingFolder() / ".ptitgit/index" / get_folder_to_object(hashedContent));
    }

    std::ofstream fileToComplete(this->repos.getWorkingFolder() / ".ptitgit/index" / get_path_to_object(hashedContent));
    fileToComplete << content;
    fileToComplete.close();

}

void StaggingArea::update_node(std::string curHash, std::string hashToDelete, std::string hashToInsert) {

    if (this->treeStaggingArea[curHash].empty()) {
        return;
    }

    std::string content = this->repos.get_repos_content(this->repos.getWorkingFolder() / ".ptitgit/index" / get_path_to_object(curHash));

    std::pair<std::string, std::string> infoDelete = delete_object(content, hashToDelete);
    std::string updatedContent1 = infoDelete.first;
    std::string deletedLine = infoDelete.second;

    std::string updatedContent2 = insert_new_object(content, get_object_type(deletedLine), get_object_hash(deletedLine), get_object_path(deletedLine));

    this->write_content(updatedContent2, hashString(updatedContent2));

    if (this->treeStaggingAreaReversed[curHash] == ".") { //Means that we are on the root tree, so we must add it to INDEX
        std::ofstream INDEX(this->repos.getWorkingFolder() / ".ptitgit/index/INDEX");
        INDEX << hashString(updatedContent2);
        INDEX.close();
    }
    else {
        update_node(this->treeStaggingAreaReversed[curHash], curHash, hashString(updatedContent2));
    }

}

void StaggingArea::add_file(File fileToAdd) {

    write_content(fileToAdd);

}

void StaggingArea::add_tree(Tree treeToAdd) {
    
}
