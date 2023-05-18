#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_struct {
    int weight;
    char ch;
} node;

typedef struct tree_node_struct tree_node;

struct tree_node_struct {
    node val;
    tree_node *left;
    tree_node *right;
};

typedef struct heapq_struct {
    int cap;
    int size;
    tree_node *elems;
} heapq_t, *heapq;


heapq new_heapq(int max_cap) {
    heapq h;
    h = malloc(sizeof(heapq_t));
    h->elems = malloc((max_cap + 1) * sizeof(tree_node));
    h->cap = max_cap;
    h->size = 0;
    return h;
}

int is_empty_heap(heapq h) {
    return h->size == 0;
}

int is_full_heapq(heapq h) {
    return h->size == h->cap;
}

void free_heapq(heapq h) {
    free(h->elems);
    free(h);
}

int push_heapq(heapq h, tree_node x) {
    int i;
    if (is_full_heapq(h)) return -1;
    for (i = ++h->size; h->elems[i / 2].val.weight > x.val.weight; i /= 2)
        h->elems[i] = h->elems[i / 2];
    h->elems[i] = x;
    return 0;
}

tree_node pop_heapq(heapq h) {
    int i, child;
    tree_node min, last;
    if (is_empty_heap(h)) return h->elems[0];
    min = h->elems[1];
    last = h->elems[h->size--];
    for (i = 1; i * 2 <= h->size; i = child) {
        child = i * 2;
        if (child != h->size && h->elems[child + 1].val.weight < h->elems[child].val.weight)
            child++;
        if (last.val.weight > h->elems[child].val.weight)
            h->elems[i] = h->elems[child];
        else break;
    }
    h->elems[i] = last;
    return min;
}

tree_node *hfm(heapq h) {
    while (h->size > 1) {
        //每次合并堆中最小堆2个结点， 再将合并后堆结点加入堆中
        tree_node t1 = pop_heapq(h);
        tree_node t2 = pop_heapq(h);

        //为要合并堆2个结点分配堆上的内存
        tree_node *mt1 = malloc(sizeof(tree_node));
        tree_node *mt2 = malloc(sizeof(tree_node));
        *mt1 = t1;
        *mt2 = t2;

        //合并
        t1.val.weight += t2.val.weight;
        t1.val.ch = 0;
        t1.left = mt1;
        t1.right = mt2;
        push_heapq(h, t1);
    }
    tree_node *root = malloc(sizeof(tree_node));
    *root = pop_heapq(h);
    return root;
}

void encode_hfm_tree(tree_node *tree, int *weights, int off, int code) {
    if (tree == NULL) return;
    if (tree->val.ch != 0) {
        printf("%c weight = %d  encode = ", tree->val.ch, weights[tree->val.ch - 'a']);
        for (int i = off - 1; i >= 0; --i) {
            printf("%d", (code & (1 << i)) > 0);
        }
        printf("\n");
    }
    encode_hfm_tree(tree->left, weights, off + 1, code | (1 << off));
    encode_hfm_tree(tree->right, weights, off + 1, code);
}

int main() {
    //用于编码的堆
    heapq hfm_heapq = new_heapq(128);
    //随机字符串
    char *chs = "aaabbbbcccccccccccddppqqooolss";

    tree_node node;
    size_t len = strlen(chs);
    int le[26] = {0};

    //统计各个字母出现频率
    for (int i = 0; i < len; i++) {
        le[chs[i] - 'a']++;
    }
    for (int i = 0; i < 26; i++) {
        if (le[i] == 0) continue;
        node.val.ch = (char) ('a' + i);
        node.val.weight = le[i];
        //入堆
        push_heapq(hfm_heapq, node);
    }

    //构造哈夫曼树
    tree_node *hfm_tree = hfm(hfm_heapq);

    //输出编码结果
    encode_hfm_tree(hfm_tree, le, 0, 0);
    free_heapq(hfm_heapq);
    return 0;
}