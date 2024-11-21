#pragma once
#include <iostream>
#include <algorithm>
#include "node.hpp"
#include "command.hpp"

namespace text_editor
{
  using std::string;
  class rope
  {

  public:
    using handle = std::unique_ptr<rope_node>;

    rope(void);
    
    rope(const string &);
    
    rope(const rope &);

    string toString(void) const;

    size_t length(void) const;

    char at(size_t index) const;

    string substring(size_t start, size_t len) const;
 
    bool isBalanced(void) const;
   
    void balance(void);

    void insert(size_t i, const string &str);
    void insert(size_t i, const rope &r);

    void append(const string &);
    void append(const rope &);

    void rdelete(size_t start, size_t len);

    void print_tree(std::ostream &out, int level = 0) const;

    rope &operator=(const rope &rhs);
    bool operator==(const rope &rhs) const;
    bool operator!=(const rope &rhs) const;
    friend std::ostream &operator<<(std::ostream &out, const rope &r);

  private:
    handle root_;

  };

  size_t fib(size_t n);
  std::vector<size_t> buildFibList(size_t len);

} 