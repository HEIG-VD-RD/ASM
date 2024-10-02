/**
 * @file   binary_tree.h
 * @author Rick Wertenbroek
 * @date   19.11.20
 *
 * @brief  Header for the binary tree exercise for ASM lab
 */

#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

typedef const char data_t;

/// @brief Node for the binary search tree
struct Node;
typedef struct Node {
    uint32_t    value;
    data_t      *data;
    struct Node *left;
    struct Node *right;
} Node;

#if __QEMU_BARE__
    // Student assembly code
    extern void traverse_tree_asm(Node *root);
#else
    // Empty functions to emulate empty assembly code on host
    void traverse_tree_asm(Node *root) {}
#endif

#endif /* __BINARY_TREE_H__ */