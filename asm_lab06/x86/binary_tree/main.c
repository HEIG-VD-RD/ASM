/**
 * @file   binary_tree.c
 * @author Rick Wertenbroek
 * @date   19.11.20
 *
 * @brief  Binary search tree exercise for ASM lab
 */

#ifndef __QEMU_BARE__
#define __QEMU_BARE__ 0
#endif

#if __QEMU_BARE__
//#   warning "Compiling for QEMU Bare Metal (U-Boot)"
#   include <common.h>
#   include <exports.h>
#else
#   warning "Compiling for host"
#   include <stdint.h>
#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>
#endif

#include "binary_tree.h"

#define M_NELEMS(x) (sizeof(x) / sizeof((x)[0]))
#define MAX_NODES 1024

/// @brief Dynamically allocates a node
Node *allocate_node_with_data(uint32_t value, data_t *data) {
    uint32_t size = sizeof(struct Node);
    Node *node  = malloc(size);
    if (node == NULL) {
        printf("malloc() failed !\n");
	return NULL;
    } else {
      //printf("Allocated 0x%08x\n", (uint32_t)node);
    }
    node->value = value;
    node->data  = data;
    node->left  = NULL;
    node->right = NULL;

    return node;
}

/// @brief Dynamically allocates a node
Node *allocate_node(uint32_t value) {
    return allocate_node_with_data(value, NULL);
}

/// @brief recursive insertion because simpler to write, compiler should optimize because this is tail rec.
void insert(Node *node, Node *root) {
    if (node->value <= root->value) {
        if (root->left) {
            insert(node, root->left);
        } else {
            root->left = node;
        }
    } else {
        if (root->right) {
            insert(node, root->right);
        } else {
            root->right = node;
        }
    }
}

/// @brief recursive get_all_nodes because simpler to write.
/// @note  not the most efficient way to do this and will grow the stack by the depth of the tree, anyways, this is fine for the example.
uint32_t get_all_nodes(Node *root, Node *array[], uint32_t num);
uint32_t get_all_nodes(Node *root, Node *array[], uint32_t num) {
    if (root == NULL) {
        return num;
    }
    if (array == NULL) {
        return num;
    }
    if (num == MAX_NODES) {
        printf("Cannot go deeper !\n");
        return num;
    }
    array[num++] = root;

    if (root->left) {
        num = get_all_nodes(root->left, array, num);
    }
    if (root->right) {
        num = get_all_nodes(root->right, array, num);
    }
    return num;
}

/// @brief unallocates all nodes of a tree, root included
void free_tree(Node *root) {
    static struct Node *array[MAX_NODES] = {};
    uint32_t num = get_all_nodes(root, array, 0);
    for (uint32_t _ = 0; _ < num; ++_) {
      //printf("Freed 0x%08x\n", (uint32_t)array[_]);
        free(array[_]);
    }
}

/// @brief The main entrypoint of the application
int main(int argc, char *argv[]) {
    int err = 0;

    // Lab example tree :
    Node *root = allocate_node_with_data(8, "over");
    insert(allocate_node_with_data(3,  "quick"), root);
    insert(allocate_node_with_data(10, "the"),   root);
    insert(allocate_node_with_data(1,  "A"),     root);
    insert(allocate_node_with_data(6,  "fox"),   root);
    insert(allocate_node_with_data(13, "lazy"),  root);
    insert(allocate_node_with_data(7,  "jumps"), root);
    insert(allocate_node_with_data(4,  "brown"), root);
    insert(allocate_node_with_data(14, "dog"),   root);
    printf("First example output (same as in lab assignment) : \n");
    traverse_tree_asm(root);

    printf("\n\n---------------\n\n");

    // Same sentence but in another order (different tree)
    Node *tree2 = allocate_node_with_data(6, "fox");
    insert(allocate_node_with_data(1,  "A"),     tree2);
    insert(allocate_node_with_data(8,  "over"),  tree2);
    insert(allocate_node_with_data(7,  "jumps"), tree2);
    insert(allocate_node_with_data(3,  "quick"), tree2);
    insert(allocate_node_with_data(10, "the"),   tree2);
    insert(allocate_node_with_data(14, "dog"),   tree2);
    insert(allocate_node_with_data(13, "lazy"),  tree2);
    insert(allocate_node_with_data(4,  "brown"), tree2);
    printf("Second example output, should be the same as first : \n");
    traverse_tree_asm(tree2);

    printf("\n\n---------------\n\n");

    // Degenerate tree (= list) with only right inserts
    printf("Third example expected output order :\nThe owls are not what they seem !\n");
    Node *tree3 = allocate_node_with_data(2, "The");
    insert(allocate_node_with_data(3,  "owls"),     tree3);
    insert(allocate_node_with_data(5,  "are"),  tree3);
    insert(allocate_node_with_data(7,  "not"), tree3);
    insert(allocate_node_with_data(8,  "what"), tree3);
    insert(allocate_node_with_data(10, "they"),   tree3);
    insert(allocate_node_with_data(15, "seem"),   tree3);
    insert(allocate_node_with_data(16, "!"),  tree3);
    printf("Third example output : \n");
    traverse_tree_asm(tree3);

    printf("\n\n---------------\n\n");

    // Degenerate tree (= list) with only left inserts after first node
    printf("Fourth example expected output order :\nWe shall say \"Ni\" to you... if you do not appease us.\n");
    Node *tree4 = allocate_node_with_data(19, "us.");
    insert(allocate_node_with_data(17,  "appease"),     tree4);
    insert(allocate_node_with_data(16,  "not"),  tree4);
    insert(allocate_node_with_data(15,  "do"), tree4);
    insert(allocate_node_with_data(11,  "you"), tree4);
    insert(allocate_node_with_data(10, "if"),   tree4);
    insert(allocate_node_with_data(8, "you..."),   tree4);
    insert(allocate_node_with_data(6, "to"),  tree4);
    insert(allocate_node_with_data(5,  "\"Ni\""),     tree4);
    insert(allocate_node_with_data(4,  "say"),  tree4);
    insert(allocate_node_with_data(3,  "shall"), tree4);
    insert(allocate_node_with_data(2,  "We"), tree4);
    printf("Fourth example output : \n");
    traverse_tree_asm(tree4);

    printf("\n\n---------------\n\n");

    // There is a bug with free ...
    //free_tree(root);
    root = NULL; // To indicate it is freed;
    //free_tree(tree2);
    tree2 = NULL;
    //free_tree(sentence_tree);
    tree3 = NULL;
    //free_tree(sentence_tree2);
    tree4 = NULL;

#if __QEMU_BARE__
	printf("Hit any key to exit ... ");
	while (!tstc());

	/* consume input */
	(void) getc();

	printf("\n\n");
#endif

	return err;
}
