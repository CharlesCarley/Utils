/*
-------------------------------------------------------------------------------

    Copyright (c) Charles Carley.

    Contributor(s): none yet.

-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "Macro.h"
#include "Utils/skBinarySearchTree.h"
#include "Utils/skString.h"
#include "catch/catch.hpp"

#define TEST_CASE_NAME skBinarySearchTree
typedef skBinarySearchTree<int> SearchTree;

//                       50
//                     /     \
//                    /       \
//                   /         \
//                  30          70
//                 /  \      /     \
//                20  40     60     80
//                    /       \     /  \
//                   32        65  75  85
//                     \
//                      34
//                        \
//                         36
//
//   Case 1: Leaf nodes      {36, 20, 65, 75, 85}
//   Case 2: One Branch(L|R) {32, 60, 40}
//   Case 3: Two Branches    {30,70,50}
int list[] = {50, 30, 70, 20, 40, 60, 80, 32, 34, 36, 65, 75, 85};
int c1[]   = {36, 20, 65, 75, 85};
int c2[]   = {32, 60, 40};
int c3[]   = {30, 70, 50};

void populate(SearchTree &tree, int initial[], int len)
{
    for (int i = 0; i < len; ++i)
        tree.insert(initial[i]);

    if (DEBUG)
    {
        SearchTree::Iterator it = tree.iterator_ascending();
        while (it.hasMoreElements())
            printf("%i ", it.getNext());
        printf("\n");
    }
}

void remove(SearchTree &tree, int removelist[], int len, int removeListLen)
{
    SearchTree::Iterator it;
    int                  i = 0;

    for (i = 0; i < removeListLen; ++i)
    {
        int v = removelist[i];
        if (tree.findNonRecursive(v))
        {
            if (DEBUG)
                printf("Removing %i: = ", v);

            tree.erase(v);

            if (DEBUG)
            {
                it = tree.iterator_ascending();
                while (it.hasMoreElements())
                    printf("%i ", it.getNext());
                printf("\n");
            }
        }
    }

    if (DEBUG)
        printf("\n");
}

TEST_CASE("BinarySearchTree_case1_then_1x1")
{
    // should produce the following output

    // 20 30 32 34 36 40 50 60 65 70 75 80 85
    // Removing 36: = 20 30 32 34 40 50 60 65 70 75 80 85
    // Removing 20: = 30 32 34 40 50 60 65 70 75 80 85
    // Removing 65: = 30 32 34 40 50 60 70 75 80 85
    // Removing 75: = 30 32 34 40 50 60 70 80 85
    // Removing 85: = 30 32 34 40 50 60 70 80
    //
    // Removing 50: = 80 70 60 40 34 32 30
    // Removing 30: = 80 70 60 40 34 32
    // Removing 70: = 80 60 40 34 32
    // Removing 40: = 80 60 34 32
    // Removing 60: = 80 34 32
    // Removing 80: = 34 32
    // Removing 32: = 34
    // Removing 34: =
    SearchTree           tree;
    SearchTree::Iterator it;
    int                  i = 0;

    int len    = sizeof(list) / sizeof(int);
    int c1_len = sizeof(c1) / sizeof(int);

    populate(tree, list, len);
    remove(tree, c1, len, c1_len);
    int result[] = {30, 32, 34, 40, 50, 60, 70, 80};
    int rl = sizeof(result) / sizeof(int), r = 0;
    it = tree.iterator_ascending();

    while (it.hasMoreElements())
    {
        int v = it.getNext();
        if (r < len)
            EXPECT_EQ(result[r], v);
        ++r;
    }

    remove(tree, list, len, len);

    EXPECT_EQ(true, tree.left() == 0);
    EXPECT_EQ(true, tree.right() == 0);
}

TEST_CASE("BinarySearchTree_case2_then_1x1")
{
    // should produce the following output
    // 20 30 32 34 36 40 50 60 65 70 75 80 85
    // Removing 32: = 20 30 34 36 40 50 60 65 70 75 80 85
    // Removing 60: = 20 30 34 36 40 50 65 70 75 80 85
    // Removing 40: = 20 30 34 36 50 65 70 75 80 85
    //
    // Removing 50: = 85 80 75 70 65 36 34 30 20
    // Removing 30: = 85 80 75 70 65 36 34 20
    // Removing 70: = 85 80 75 65 36 34 20
    // Removing 20: = 85 80 75 65 36 34
    // Removing 80: = 85 75 65 36 34
    // Removing 34: = 85 75 65 36
    // Removing 36: = 85 75 65
    // Removing 65: = 85 75
    // Removing 75: = 85
    // Removing 85: =

    SearchTree           tree;
    SearchTree::Iterator it;
    int                  i = 0;

    int len    = sizeof(list) / sizeof(int);
    int c2_len = sizeof(c2) / sizeof(int);

    populate(tree, list, len);
    remove(tree, c2, len, c2_len);
    int result[] = {20, 30, 34, 36, 50, 65, 70, 75, 80, 85};
    int rl = sizeof(result) / sizeof(int), r = 0;
    it = tree.iterator_ascending();

    while (it.hasMoreElements())
    {
        int v = it.getNext();
        if (r < len)
            EXPECT_EQ(result[r], v);
        ++r;
    }

    remove(tree, list, len, len);
    EXPECT_EQ(true, tree.left() == 0);
    EXPECT_EQ(true, tree.right() == 0);
}

TEST_CASE("BinarySearchTree_case3_then_1x1")
{
    // should produce the following output
    // 20 30 32 34 36 40 50 60 65 70 75 80 85
    // Removing 30: = 20 32 34 36 40 50 60 65 70 75 80 85
    // Removing 70: = 20 32 34 36 40 50 60 65 75 80 85
    // Removing 50: = 20 32 34 36 40 60 65 75 80 85
    //
    // Removing 20: = 85 80 75 65 60 40 36 34 32
    // Removing 40: = 85 80 75 65 60 36 34 32
    // Removing 60: = 85 80 75 65 36 34 32
    // Removing 80: = 85 75 65 36 34 32
    // Removing 32: = 85 75 65 36 34
    // Removing 34: = 85 75 65 36
    // Removing 36: = 85 75 65
    // Removing 65: = 85 75
    // Removing 75: = 85
    // Removing 85: =

    SearchTree           tree;
    SearchTree::Iterator it;
    int                  i = 0;

    int len    = sizeof(list) / sizeof(int);
    int c3_len = sizeof(c3) / sizeof(int);

    populate(tree, list, len);
    remove(tree, c3, len, c3_len);

    int result[] = {20, 32, 34, 36, 40, 60, 65, 75, 80, 85};
    int rl = sizeof(result) / sizeof(int), r = 0;
    it = tree.iterator_ascending();

    while (it.hasMoreElements())
    {
        int v = it.getNext();
        if (r < len)
            EXPECT_EQ(result[r], v);
        ++r;
    }

    remove(tree, list, len, len);

    EXPECT_EQ(true, tree.left() == 0);
    EXPECT_EQ(true, tree.right() == 0);
}

//                       50
//                     /     \
//                    /       \
//                   /         \
//                  30          70
//                 /  \       /    \
//                20  40     60     80
//                    /       \     /  \
//                   32        65  75  85
//                     \
//                      34
//                        \
//                         36
//

TEST_CASE("BinarySearchTree_minimum_maximum")
{
    SearchTree tree;
    int        i = 0;

    int len = sizeof(list) / sizeof(int);
    populate(tree, list, len);

    SearchTree::NodePointerType node = tree.root(), L, R;
    EXPECT_EQ(true, node != 0);
    EXPECT_EQ(50, node->data());

    node = node->left();
    EXPECT_EQ(true, node != 0);
    EXPECT_EQ(30, node->data());

    L = node->left();
    EXPECT_EQ(true, L != 0);
    EXPECT_EQ(20, L->data());

    R = node->right();
    EXPECT_EQ(true, R != 0);
    EXPECT_EQ(40, R->data());

    EXPECT_EQ(true, R->right() == 0);

    L = R->left();
    EXPECT_EQ(true, L != 0);
    EXPECT_EQ(32, L->data());

    EXPECT_EQ(true, L->left() == 0);

    R = tree.maximum(L->right());
    EXPECT_EQ(true, R != 0);
    EXPECT_EQ(36, R->data());

    node = tree.root();
    R    = node->right();
    EXPECT_EQ(true, R != 0);
    EXPECT_EQ(70, R->data());

    L = tree.maximum(R->left());
    EXPECT_EQ(true, L != 0);
    EXPECT_EQ(65, L->data());

    R = tree.maximum(R->right());
    EXPECT_EQ(85, R->data());
}
