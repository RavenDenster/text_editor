#include <iostream>
#include <sstream>
#include "node.hpp"
namespace text_editor
{
  using handle = rope_node::handle;
  std::invalid_argument ERROR_OOB_NODE = std::invalid_argument("Error: string index out of bounds");

  rope_node::rope_node(handle left, handle right) : fragment_("")
  {
    this->left_ = move(left);
    this->right_ = move(right);
    this->weight_ = this->left_->getLength();
  }

  rope_node::rope_node(const std::string &str) : weight_(str.length()), left_(nullptr), right_(nullptr), fragment_(str) {}

  rope_node::rope_node(const rope_node &aNode) : weight_(aNode.weight_), fragment_(aNode.fragment_)
  {
    rope_node *tmpLeft = aNode.left_.get();
    rope_node *tmpRight = aNode.right_.get();

    if (tmpLeft == nullptr)
      this->left_ = nullptr;
    else
      this->left_ = std::make_unique<rope_node>(*tmpLeft);

    if (tmpRight == nullptr)
      this->right_ = nullptr;
    else
      this->right_ = std::make_unique<rope_node>(*tmpRight);
  }

  bool rope_node::isLeaf(void) const
  {
    return this->left_ == nullptr && this->right_ == nullptr;
  }

  size_t rope_node::getLength() const
  {
    if (this->isLeaf())
      return this->weight_;
    size_t tmp = (this->right_ == nullptr) ? 0 : this->right_->getLength();
    return this->weight_ + tmp;
  }

  char rope_node::getCharByIndex(size_t index) const
  {
    size_t w = this->weight_;
    if (this->isLeaf())
    {
      if (index >= this->weight_)
      {
        throw ERROR_OOB_NODE;
      }
      else
      {
        return this->fragment_[index];
      }
    }
    else
    {
      if (index < w)
      {
        return this->left_->getCharByIndex(index);
      }
      else
      {
        return this->right_->getCharByIndex(index - w);
      }
    }
  }

  std::string rope_node::getSubstring(size_t start, size_t len) const
  {
    size_t w = this->weight_;
    if (this->isLeaf())
    {
      if (len < w)
      {
        return this->fragment_.substr(start, len);
      }
      else
      {
        return this->fragment_;
      }
    }
    else
    {
      if (start < w)
      {
        std::string lResult = (this->left_ == nullptr) ? "" : this->left_->getSubstring(start, len);
        if ((start + len) > w)
        {
          size_t tmp = w - start;
          std::string rResult = (this->right_ == nullptr) ? "" : this->right_->getSubstring(w, len - tmp);
          return lResult.append(rResult);
        }
        else
        {
          return lResult;
        }
      }
      else
      {
        return (this->right_ == nullptr) ? "" : this->right_->getSubstring(start - w, len);
      }
    }
  }

  std::string rope_node::treeToString(void) const
  {
    if (this->isLeaf())
    {
      return this->fragment_;
    }
    std::string lResult = (this->left_ == nullptr) ? "" : this->left_->treeToString();
    std::string rResult = (this->right_ == nullptr) ? "" : this->right_->treeToString();
    return lResult.append(rResult);
  }

  std::pair<handle, handle> splitAt(handle node, size_t index)
  {
    size_t w = node->weight_;

    if (node->isLeaf())
    {
      return std::pair<handle, handle>{
          std::make_unique<rope_node>(node->fragment_.substr(0, index)),
          std::make_unique<rope_node>(node->fragment_.substr(index, w - index))
        };
    }

    handle oldRight = move(node->right_);
    if (index < w)
    {
      node->right_ = nullptr;
      node->weight_ = index;
      std::pair<handle, handle> splitLeftResult = splitAt(move(node->left_), index);
      node->left_ = move(splitLeftResult.first);
      return std::pair<handle, handle>{
          move(node),
          std::make_unique<rope_node>(move(splitLeftResult.second), move(oldRight))};
    }
    else if (w < index)
    {
      std::pair<handle, handle> splitRightResult = splitAt(move(oldRight), index - w);
      node->right_ = move(splitRightResult.first);
      return std::pair<handle, handle>{
          move(node),
          move(splitRightResult.second)};
    }
    else
    {
      return std::pair<handle, handle>{
          move(node->left_), move(oldRight)};
    }
  }

  size_t rope_node::getDepth(void) const
  {
    if (this->isLeaf())
      return 0;
    size_t lResult = (this->left_ == nullptr) ? 0 : this->left_->getDepth();
    size_t rResult = (this->right_ == nullptr) ? 0 : this->right_->getDepth();
    return std::max(++lResult, ++rResult);
  }

  void rope_node::getLeaves(std::vector<rope_node *> &v)
  {
    if (this->isLeaf())
    {
      v.push_back(this);
    }
    else
    {
      rope_node *tmpLeft = this->left_.get();
      if (tmpLeft != nullptr)
        tmpLeft->getLeaves(v);
      rope_node *tmpRight = this->right_.get();
      if (tmpRight != nullptr)
        tmpRight->getLeaves(v);
    }
  }

  void rope_node::print_tree(std::ostream &out, int level) const
{
  if (this->isLeaf())
  {
    out << std::string(level * 4, ' ') << "Leaf: \"" << fragment_ << "\" (Length: " << fragment_.length() << ")\n";
  }
  else
  {
    out << std::string(level * 4, ' ') << "Node (Total Length: " << this->getLength() << ")\n";
    if (left_)
    {
      left_->print_tree(out, level + 1);
    }
    if (right_)
    {
      right_->print_tree(out, level + 1);
    }
  }
}

}