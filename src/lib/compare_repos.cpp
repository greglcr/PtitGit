#include "compare_repos.h"
#include "repos.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <deque>
#include <filesystem>
#include "object_commit.h"
#include "reftag.h"
#include <unistd.h>

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


void bfs(PtitGitRepos repos, std::string start, std::map<std::string, int> & ancestors)  {
    
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
        
        
        Commit commit = Commit ();
        //commit.fromfile(objectFind(repos, hash, true, "commit"), repos);
        commit.fromfile(hash, repos);


        for (std::string parent : commit.get_parents_hash())    {
            if (parent == "0") continue;
            queue.push_back({parent, dist});
        }
    }
}

std::string last_common_ancestor(PtitGitRepos repos, std::string a, std::string b) {
    if (a == b) return a;
    std::map<std::string, int> ancestorsA, ancestorsB;

    bfs(repos, a, ancestorsA);
    bfs(repos, b, ancestorsB);

    if (ancestorsA.find(b) != ancestorsA.end())
        return b;

    if (ancestorsB.find(a) != ancestorsB.end())
        return a;

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


result_compare compare_branch(PtitGitRepos reposA, PtitGitRepos reposB)    {
    // use the branch store in A and look for this one in B

    std::ifstream HEAD_file(reposA.getWorkingFolder() / ".ptitgit" / "HEAD");
    std::stringstream buffer_HEAD;
    buffer_HEAD << HEAD_file.rdbuf();
    std::string HEAD = buffer_HEAD.str();

    if (HEAD.find("ref: ") == 0)   {
        std::string branchName = HEAD.substr(HEAD.rfind("/")+1);
        std::cout << "You are working on the branch '" << branchName << "'" << std::endl;
        std::string commit_local = ref_resolve(reposA, "HEAD");
        std::cout << commit_local << std::endl;
    
        //std::cout << tmp_dir + "/" + HEAD.substr(HEAD.find(".ptitgit")) << std::endl;
        if ( access( (reposB.getWorkingFolder() / HEAD.substr(HEAD.find(".ptitgit"))).c_str(), F_OK ) == -1 )   {
            std::cout << "Warning, your branch does not exist in the remote repository. Therefore 'pull' did nothing" << std::endl;
            return {branchName, commit_local, "", ""};
        } else  {
            std::string commit_remote = ref_resolve(reposB, "HEAD");
            std::cout << commit_remote << std::endl;
            
            std::string lca_hash = last_common_ancestor(reposA, commit_local, commit_remote);

            return {branchName, commit_local, commit_remote, lca_hash};
        }
    } else  {
        return {"", "", "", "" };
    }
}


