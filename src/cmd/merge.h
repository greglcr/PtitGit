#ifndef MERGE_H
#define MERGE_H

#include "../lib/object.h"
#include "../lib/object_commit.h"
#include "../lib/object_tree.h"

void merge(std::string, std::string = "");
Tree mergeTree(Tree,Tree);
#endif