#ifndef STAGGING_AREA_H
#define STAGGING_AREA_H

#include "repos.h"
#include "object_tree.h"

#include <map>
#include <string>
#include <vector>

struct NodeTree {

    std::string nodeHash;
    std::string fatherHash = ".";

    bool operator<(const NodeTree &other);

};

class StaggingArea {

    public:

        StaggingArea(PtitGitRepos repos);
        void construct_tree(std::string curFileHash);
        void calc_differences(bool verbose);
        void calc_differences(fs::path curPosWorkingArea, std::string curHashStaggingArea, bool verbose);
        void add(fs::path pathToAdd);
        void add_all();
        void add_all(Tree curTree);
        void write_content(Object curObject);
        void write_content(std::string content, std::string hashedContent);
        void update_node(std::string curFileHash, std::string hashToDelete, std::string hashToInsert);
        std::string get_root_tree();    
        //Besoin de deux fonctions différentes. En effet, il va être possible d'ajouter deux types de fonctions différentes lorqu'on fait un add normalement.
        //Difficulté : quand on ajoute un fichier, il faut reconstruire tout l'arbre. Est-ce qu'on a les outils pour faire ça?

    private:

        PtitGitRepos repos;
        std::string rootTree;
        std::map<std::string, std::vector<std::string> > treeStaggingArea;
        std::map<std::string, std::string> treeStaggingAreaReversed;
        std::map<fs::path, std::pair<std::string, std::string> > status;

        //std::string delete_in_file(std::string curHash, std::string hashToDelete);
        //void add_in_file(std::string curHash, std::string typeToAdd, std::string hashToAdd, std::string pathToAdd);

};

#endif