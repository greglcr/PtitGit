#ifndef OBJECT_TREE_H
#define OBJECT_TREE_H

#include "object.h"
#include "object_file.h"

#include <vector>

class Tree : public Object {

    public:

        Tree(fs::path folderPath = ".", bool create = false);
        Tree createTreeFromContent(std::string, bool);
        fs::path get_folder_path();
        std::vector<File> get_blobs_inside();
        std::vector<Tree> get_trees_inside();

    private:

        fs::path folderPath;
        std::vector<File> filesInside;
        std::vector<Tree> treesInside;

};

Tree findTree(std::string,bool create = false, bool objectOrNot = true);
#endif