#pragma once
#ifndef __JXX_BYTEARRAY__
#define __JXX_BYTEARRAY__

#include <iostream>
#include <vector>
#include <cstddef> // std::byte, std::to_integer
#include <stdexcept>

// ---------- Optional: demangle for GCC/Clang ----------
#if defined(__GNUG__) || defined(__clang__)
#include <cxxabi.h>
#include <cstdlib>
#endif


using namespace std;

namespace jxx::lang {

    class ByteArray {
        struct Node {
            bool isLeaf = false;
            vector<Node> children;
            vector<int8_t> data;

            Node() = default;
            Node(size_t size, bool leaf) : isLeaf(leaf) {
                if (leaf) data.resize(size, int8_t{ 0 });
                else children.resize(size);
            }
        };

        Node root;

    public:
        // Create only first dimension (like new uint8_t[dim][])
        explicit ByteArray(size_t firstDim) {
            root = Node(firstDim, false);
        }

        // Create fully rectangular array
        ByteArray(const std::vector<size_t>& dims) {
            if (dims.empty()) throw std::invalid_argument("No dimensions");
            root = buildRect(dims, 0);
        }

        // Proxy for subarrays
        class ArrayProxy {
            Node* node_;
        public:
            ArrayProxy(Node* node) : node_(node) {}

            ArrayProxy operator[](size_t idx) {
                if (node_->isLeaf) throw runtime_error("Too many indices");
                return ArrayProxy(&node_->children.at(idx));
            }

            // Allocate subarray
            void allocate(size_t size, bool leaf = true) {
                if (leaf) {
                    node_->isLeaf = true;
                    node_->data.assign(size, int8_t{ 0 });
                    node_->children.clear();
                }
                else {
                    node_->isLeaf = false;
                    node_->children.assign(size, Node());
                    node_->data.clear();
                }
            }

            bool isAllocated() const {
                return node_->isLeaf ? !node_->data.empty() : !node_->children.empty();
            }

            std::size_t size() const {
                return node_->isLeaf ? node_->data.size() : node_->children.size();
            }

            // Special case: if this is a leaf, return LeafProxy
            class LeafProxy {
                Node* node_;
            public:
                LeafProxy(Node* node) : node_(node) {}
                int8_t& operator[](size_t idx) {
                    if (!node_->isLeaf) throw runtime_error("Not a leaf node");
                    return node_->data.at(idx);
                }

                std::size_t size() const {
                    if (!node_->isLeaf) throw std::runtime_error("Not a leaf node");
                    return node_->data.size();
                }
            };

            LeafProxy leaf() { return LeafProxy(node_); }
           
        };

        ArrayProxy operator[](std::size_t idx) {
            return ArrayProxy(&root.children.at(idx));
        }

    private:
        static Node buildRect(const vector<std::size_t>& dims, size_t level) {
            Node node(dims[level], level == dims.size() - 1);
            if (!node.isLeaf) {
                for (auto& child : node.children) {
                    child = buildRect(dims, level + 1);
                }
            }
            return node;
        }
    };

}
#endif

