#include "hashing.h"
#include "object.h"
#include "object_file.h"
#include "repos.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

File::File(fs::path filePath, bool create) {

    if (filePath.is_relative()) {
        filePath = fs::current_path() / filePath;
    }

    std::ifstream inputFile(filePath);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file : " << filePath << std::endl;
    }

    std::ostringstream oss;
    oss << inputFile.rdbuf();
    
    this->content += relativeToRepo(filePath);
    this->content += '\n';
    this->content += oss.str();
    this->actualFile = oss.str();
    this->filePath = filePath;

    std::string abc = this->content;
    this->content = "file " + std::to_string(abc.size()) + '\n' + abc;

    this->hashedContent = hashString(this->content);
    if(create) {
        PtitGitRepos curRepos = PtitGitRepos(filePath);
        curRepos.writeObject(*this);
    }
}

void File::updateContent() {
    
    if(filePath.is_relative()) filePath = PtitGitRepos().getWorkingFolder() / filePath;

    std::ifstream inputFile(this->filePath);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file : " << this->filePath << std::endl;
    }

    std::ostringstream oss;
    oss << inputFile.rdbuf();
    this->content = relativeToRepo(filePath);
    this->content += '\n';
    this->content += oss.str();
    this->actualFile = oss.str();
    std::string abc = this->content;
    this->content = "file " + std::to_string(abc.size()) + '\n' + abc;
    this->hashedContent = hashString(this->content);
}

fs::path File::get_file_path() {

    return this->filePath;

}

std::string File::getActualFile(){
    return this->actualFile;
}

File findFile(std::string hashedContent, bool create, PtitGitRepos repos){

    fs::path path = repos.getWorkingFolder() / ".ptitgit" / "objects" / get_path_to_object(hashedContent);
    if(!fs::exists(path)) path = repos.getWorkingFolder() / ".ptitgit" / "index" / get_path_to_object(hashedContent);

    std::ifstream fileToShow(path);


    if(!fileToShow.is_open()) {
        std::cerr << "Error : impossibe to open the file in get_object_content (" << path << ")" << std::endl;
        exit(0);
    }

    std::stringstream buffer;
    buffer << fileToShow.rdbuf();
    std::string content = buffer.str();
    fileToShow.close();
    return File(repos.getWorkingFolder()).createFileFromContent(content,create,repos);
}

File File::createFileFromContent(std::string content, bool create, PtitGitRepos repos){
    this->content = content;
    long long findSpace = content.find(' ');
    long long findEndl = content.find('\n');
    
    if(content.substr(0,findSpace) != "file") std::cerr<<"Not a file here\n";
    if((long long) std::stoi(content.substr(findSpace+1,findEndl-findSpace-1)) != (long long) content.size()-findEndl-1) std::cerr<<"Bad size!\n";

    long long findNextEndl = content.find('\n',findEndl+1);
    this->filePath = content.substr(findEndl + 1, findNextEndl - findEndl - 1);

    this->hashedContent = hashString(this->content);

    if(create) this->writeObject(repos.getWorkingFolder());
    return *this;
}

bool compareFile(File F1, File F2){
    if(F1.get_file_path().filename().string() < F2.get_file_path().filename().string()) return true;
    else return false;
}
