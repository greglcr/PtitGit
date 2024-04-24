#include "object.h"
#include "object_file.h"

#include <vector>

class Tree : public Object {

    public:

        Tree(fs::path folderPath);

    private:

        fs::path folderPath;
        std::vector<File> filesInside;
        std::vector<Tree> treesInside;
        std::string readableContent;

};