#pragma once
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

namespace text_editor
{
  class rope_node
  {

  public:
    using handle = std::unique_ptr<rope_node>;

    rope_node(handle l, handle r);
    
    rope_node(const std::string &str);
    
    rope_node(const rope_node &);

    size_t getLength(void) const;

    char getCharByIndex(size_t) const;
  
    std::string getSubstring(size_t start, size_t len) const;

    std::string treeToString(void) const;

    friend std::pair<handle, handle> splitAt(handle, size_t);

    size_t getDepth(void) const;

    void getLeaves(std::vector<rope_node *> &v);

    void print_tree(std::ostream &out, int level) const;

  private:
    size_t weight_;
    handle left_;
    handle right_;
    std::string fragment_;
    
    bool isLeaf(void) const;

  };
}