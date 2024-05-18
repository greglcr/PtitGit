#ifndef OBJECT_TREE_H
#define OBJECT_TREE_H

#include "object.h"
#include "object_file.h"

#include <map>
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
std::string get_next_line(std::string &s);
std::map<std::string, std::string> cut_line(std::string contentLine);
std::string get_object_type(std::string contentLine);
std::string get_object_hash(std::string contentLine);
std::string get_object_path(std::string contentLine);
std::string insert_new_object(std::string content, std::string typeToInsert, std::string hashToInsert, std::string nameToInsert);
std::pair<std::string, std::string> delete_object(std::string content, std::string hashToDelete);

#endif