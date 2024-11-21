
#include "rope.hpp"

namespace text_editor
{
  std::invalid_argument ERROR_OOB_ROPE = std::invalid_argument("Error: string index out of bounds");

  rope::rope(void) : rope("")
  {
  }

  rope::rope(const string &str)
  {
    this->root_ = std::make_unique<rope_node>(str);
  }

  rope::rope(const rope &r)
  {
    rope_node newRoot = rope_node(*r.root_);
    this->root_ = std::make_unique<rope_node>(newRoot);
  }

  string rope::toString(void) const
  {
    if (this->root_ == nullptr)
      return "";
    return this->root_->treeToString();
  }

  size_t rope::length(void) const
  {
    if (this->root_ == nullptr)
      return 0;
    return this->root_->getLength();
  }

  char rope::at(size_t index) const
  {
    if (this->root_ == nullptr)
      throw ERROR_OOB_ROPE;
    return this->root_->getCharByIndex(index);
  }

  string rope::substring(size_t start, size_t len) const
  {
    size_t actualLength = this->length();
    if (start > actualLength || (start + len) > actualLength)
      throw ERROR_OOB_ROPE;
    return this->root_->getSubstring(start, len);
  }

  void rope::insert(size_t i, const string &str)
  {
    this->insert(i, rope(str));
  }

  void rope::insert(size_t i, const rope &r)
  {
    if (this->length() < i)
    {
      throw ERROR_OOB_ROPE;
    }
    else
    {
      rope tmp = rope(r);
      std::pair<handle, handle> origRopeSplit = splitAt(move(this->root_), i);
      handle tmpConcat = std::make_unique<rope_node>(move(origRopeSplit.first), move(tmp.root_));
      this->root_ = std::make_unique<rope_node>(move(tmpConcat), move(origRopeSplit.second));
    }
  }

  void rope::append(const string &str)
  {
    rope tmp = rope(str);
    this->root_ = std::make_unique<rope_node>(move(this->root_), move(tmp.root_));
  }

  void rope::append(const rope &r)
  {
    rope tmp = rope(r);
    this->root_ = std::make_unique<rope_node>(move(this->root_), move(tmp.root_));
  }

  void rope::rdelete(size_t start, size_t len)
  {
    size_t actualLength = this->length();
    if (start > actualLength || start + len > actualLength)
    {
      throw ERROR_OOB_ROPE;
    }
    else
    {
      std::pair<handle, handle> firstSplit = splitAt(move(this->root_), start);
      std::pair<handle, handle> secondSplit = splitAt(move(firstSplit.second), len);
      secondSplit.first.reset();
      this->root_ = std::make_unique<rope_node>(move(firstSplit.first), move(secondSplit.second));
    }
  }

  void rope::print_tree(std::ostream &out, int level) const
  {
    if (root_)
    {
      root_->print_tree(out, level);
    }
    else
    {
      out << "Empty rope." << std::endl;
    }
  }

  bool rope::isBalanced(void) const
  {
    if (this->root_ == nullptr)
      return true;
    size_t d = this->root_->getDepth();
    return this->length() >= fib(d + 2);
  }

  void rope::balance(void)
  {
    if (!this->isBalanced())
    {
      std::vector<size_t> intervals = buildFibList(this->length());
      std::vector<handle> nodes(intervals.size());

      std::vector<rope_node *> leaves;
      this->root_->getLeaves(leaves);

      size_t i;
      size_t max_i = intervals.size() - 1;
      size_t currMaxInterval = 0;
      handle acc = nullptr;
      handle tmp = nullptr;

      for (auto &leaf : leaves)
      {
        size_t len = leaf->getLength();
        bool inserted = false;

        if (len > 0)
        {
          acc = std::make_unique<rope_node>(*leaf);
          i = 0;

          while (!inserted)
          {
            while (i < max_i && len >= intervals[i + 1])
            {
              if (nodes[i].get() != nullptr)
              {
                tmp = std::make_unique<rope_node>(*nodes[i].get());
                acc = std::make_unique<rope_node>(*acc.get());
                acc = std::make_unique<rope_node>(move(tmp), move(acc));

                len = acc->getLength();

                if (len >= intervals[i + 1])
                  nodes[i] = nullptr;
              }
              i++;
            }

            if (nodes[i].get() == nullptr)
            {
              nodes[i].swap(acc);
              inserted = true;
              if (i > currMaxInterval)
                currMaxInterval = i;
            }
            else
            {
              tmp = std::make_unique<rope_node>(*nodes[i].get());
              acc = std::make_unique<rope_node>(*acc.get());
              acc = std::make_unique<rope_node>(move(tmp), move(acc));

              len = acc->getLength();

              if (len >= intervals[i + 1])
                nodes[i] = nullptr;
            }
          }
        }
      }

      acc = move(nodes[currMaxInterval]);
      for (int idx = currMaxInterval; idx >= 0; idx--)
      {
        if (nodes[idx] != nullptr)
        {
          tmp = std::make_unique<rope_node>(*nodes[idx].get());
          acc = std::make_unique<rope_node>(move(acc), move(tmp));
        }
      }
      this->root_ = move(acc);
    }
  }

  rope &rope::operator=(const rope &rhs)
  {
    if (this == &rhs)
      return *this;

    this->root_.reset();

    this->root_ = std::make_unique<rope_node>(*(rhs.root_.get()));
    return *this;
  }

  bool rope::operator==(const rope &rhs) const
  {
    return this->toString() == rhs.toString();
  }

  bool rope::operator!=(const rope &rhs) const
  {
    return !(*this == rhs);
  }

  std::ostream &operator<<(std::ostream &out, const rope &r)
  {
    return out << r.toString();
  }

  size_t fib(size_t n)
  {

    int a = 0, b = 1, next;
    if (n == 0)
      return a;
    for (size_t i = 2; i <= n; i++)
    {
      next = a + b;
      a = b;
      b = next;
    }
    return b;
  };

  std::vector<size_t> buildFibList(size_t len)
  {
    int a = 0, b = 1, next;
    std::vector<size_t> intervals = std::vector<size_t>();
    while (a <= len)
    {
      if (a > 0)
      {
        intervals.push_back(b);
      }
      next = a + b;
      a = b;
      b = next;
    }
    return intervals;
  }

}