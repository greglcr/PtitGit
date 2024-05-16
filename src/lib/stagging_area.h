#ifndef STAGGING_AREA_H
#define STAGGING_AREA_H

#include "repos.h"
#include "object_tree.h"

#include <map>
#include <vector>

class StaggingArea {

    public:

        StaggingArea(PtitGitRepos repos);
        void construct_tree(std::string curFileHash);
        void show_differences();
        void show_differences(fs::path curPosWorkingArea, std::string curHashStaggingArea);
        void add_tree(Tree treeToAdd);
        void add_file(File fileToAdd);
        std::string get_root_tree();
        //Besoin de deux fonctions différentes. En effet, il va être possible d'ajouter deux types de fonctions différentes lorqu'on fait un add normalement.
        //Difficulté : quand on ajoute un fichier, il faut reconstruire tout l'arbre. Est-ce qu'on a les outils pour faire ça?

    private:

        PtitGitRepos repos;
        std::string rootTree;
        std::map<std::pair<std::string, fs::path> , std::vector<std::string> > treeStaggingArea; //Need to store the file path to check if there is a creation of something

};

#endif