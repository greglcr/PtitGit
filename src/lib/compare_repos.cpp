#include "compare_repos.h"
#include <iostream>
#include <string>
#include <map>
#include <deque>
#include <filesystem>
#include "object_commit.h"

namespace fs = std::filesystem;

bool copy_all_objects(std::string src, std::string target) {
    src += "/.ptitgit/objects";
    target += "/.ptitgit/objects";

    try {
        fs::copy(src, target, fs::copy_options::overwrite_existing | fs::copy_options::recursive);
    } catch (std::exception& e)   {
        std::cout << e.what();
        return false;
    }
    return true;
}


void bfs(std::string start, std::map<std::string, int> & ancestors)  {
    
    std::deque<std::pair<std::string, int> > queue;
    queue.push_back({start, 0});
    while (!queue.empty())  {
        std::string hash = queue.front().first;
        int dist = queue.front().second;
        queue.pop_front();

        if (ancestors.find(hash) != ancestors.end())
            continue;
        ancestors[hash] = dist;
        dist++;
        
        
        Commit commit;
        commit.fromfile(hash);

        for (std::string parent : commit.get_parents_hash())    {
            if (parent == "0") continue;
            queue.push_back({parent, dist});
        }
    }
}

std::string last_common_ancestor(Commit a, Commit b) {
    if (a.getHashedContent() == b.getHashedContent()) return a.getHashedContent();
    std::map<std::string, int> ancestorsA, ancestorsB;


    bfs(a.getHashedContent(), ancestorsA);
    bfs(b.getHashedContent(), ancestorsB);

    if (ancestorsA.find(b.getHashedContent()) != ancestorsA.end())
        return b.getHashedContent();

    if (ancestorsB.find(a.getHashedContent()) != ancestorsB.end())
        return a.getHashedContent();

    int mini = 1000000000;
    std::string commit_result = "??????????????????";
    for (auto it = ancestorsA.begin(); it != ancestorsA.end(); it++)    {
        if (ancestorsB.find(it->first) != ancestorsB.end())  {
            int val = std::min(it->second, ancestorsB[it->first]);
            if (val <= mini)    {
                mini = val;
                commit_result = it->first;
            }
        }
    }
    return commit_result;
}

    


