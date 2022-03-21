#define o 1    // mems++
#define oo 1   // mems+= 2
#define ooo 1  // mems+= 3
#define O "%"
#define mod %

#define max_level 1000
#define max_cols 100000
#define max_nodes 50000000
#define bufsize (9 * max_cols + 3)

#define show_basics 1
#define show_choices 2
#define show_details 4
#define show_profile 128
#define show_full_state 256
#define show_tots 512
#define show_warnings 1024
#define show_max_deg 2048

#define len itm
#define aux spare

#define root 0

#define sanity_checking 0

#define panic(m)                                                    \
    {                                                               \
        fprintf(stderr, "" O "s!\n" O "d: " O ".99s\n", m, p, buf); \
        exit(-666);                                                 \
    }

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
using namespace std;

typedef unsigned int uint;
typedef unsigned long long ullng;

typedef struct node_struct {
    int up, down;
    int itm;
    int spare;
} node;

typedef struct itm_struct {
    int name;
    int prev, next;
} item;

int random_seed = 0;
int randomizing;
int vbose = show_basics + show_warnings;
int spacing;
int show_choices_max = 1000000;
int show_choices_gap = 1000000;

int show_levels_max = 1000000;
int maxl = 0;
char buf[bufsize];
ullng count;
ullng options;
ullng imems, mems;
ullng updates;
ullng bytes;
ullng nodes;
ullng thresh = 0;
ullng delta = 0;
ullng maxcount = 0xffffffffffffffff;
ullng timeout = 0x1fffffffffffffff;
FILE* shape_file;
char* shape_name;
int maxdeg;

node nd[max_nodes];
int last_node;
item cl[max_cols + 2];
int second = max_cols;
int last_itm;

int level;
int choice[max_level];
ullng profile[max_level];

void print_option(int p, vector<int>& out_option) {
    int k, q;
    if (p < last_itm || p >= last_node || nd[p].itm <= 0) {
        fprintf(stderr, "Illegal option " O "d!\n", p);
        return;
    }
    for (q = p;;) {
        out_option.push_back(cl[nd[q].itm].name - 1);
        q++;
        if (nd[q].itm <= 0) q = nd[q].up;
        if (q == p) break;
    }
    for (q = nd[nd[p].itm].down, k = 1; q != p; k++) {
        if (q == nd[p].itm) {
            return;
        } else
            q = nd[q].down;
    }
}

void sanity(void) {
    int k, p, q, pp, qq, t;
    for (q = root, p = cl[q].next;; q = p, p = cl[p].next) {
        if (cl[p].prev != q) fprintf(stderr, "Bad prev field at itm " O ".8s!\n", cl[p].name);
        if (p == root) break;

        for (qq = p, pp = nd[qq].down, k = 0;; qq = pp, pp = nd[pp].down, k++) {
            if (nd[pp].up != qq) fprintf(stderr, "Bad up field at node " O "d!\n", pp);
            if (pp == p) break;
            if (nd[pp].itm != p) fprintf(stderr, "Bad itm field at node " O "d!\n", pp);
        }
        if (nd[p].len != k) fprintf(stderr, "Bad len field in item " O ".8s!\n", cl[p].name);
    }
}

void cover(int c) {
    int cc, l, r, rr, nn, uu, dd, t;
    o, l = cl[c].prev, r = cl[c].next;
    oo, cl[l].next = r, cl[r].prev = l;
    updates++;
    for (o, rr = nd[c].down; rr >= last_itm; o, rr = nd[rr].down)
        for (nn = rr + 1; nn != rr;) {
            o, uu = nd[nn].up, dd = nd[nn].down;
            o, cc = nd[nn].itm;
            if (cc <= 0) {
                nn = uu;
                continue;
            }
            oo, nd[uu].down = dd, nd[dd].up = uu;
            updates++;
            o, t = nd[cc].len - 1;
            o, nd[cc].len = t;
            nn++;
        }
}

void uncover(int c) {
    int cc, l, r, rr, nn, uu, dd, t;
    for (o, rr = nd[c].down; rr >= last_itm; o, rr = nd[rr].down)
        for (nn = rr + 1; nn != rr;) {
            o, uu = nd[nn].up, dd = nd[nn].down;
            o, cc = nd[nn].itm;
            if (cc <= 0) {
                nn = uu;
                continue;
            }
            oo, nd[uu].down = nd[dd].up = nn;
            o, t = nd[cc].len + 1;
            o, nd[cc].len = t;
            nn++;
        }
    o, l = cl[c].prev, r = cl[c].next;
    oo, cl[l].next = cl[r].prev = c;
}

vector<vector<int>> dlx_solve(const vector<int>& p_items, const vector<int>& s_items,
                              const vector<vector<int>>& in_options) {
    vector<vector<int>> out_options;

    int cc, i, j, k, p, pp, q, r, t, cur_node, best_itm;
    spacing = 1;
    maxcount = 1;

    if (max_nodes <= 2 * max_cols) {
        fprintf(stderr, "Recompile me: max_nodes must exceed twice max_cols!\n");
        exit(-999);
    }
    last_itm = 1;
    if (!last_itm) panic("No items");

    for (int item : p_items) {
        cl[last_itm].name = item + 1;
        for (k = 1; o, cl[k].name != cl[last_itm].name; k++)
            ;
        if (k < last_itm) panic("Duplicate item name");

        if (last_itm > max_cols) panic("Too many items");
        oo, cl[last_itm - 1].next = last_itm, cl[last_itm].prev = last_itm - 1;

        o, nd[last_itm].up = nd[last_itm].down = last_itm;
        last_itm++;
    }
    if (second != max_cols) panic("Item name line contains | twice");
    second = last_itm;
    for (int item : s_items) {
        cl[last_itm].name = item + 1;
        for (k = 1; o, cl[k].name != cl[last_itm].name; k++)
            ;
        if (k < last_itm) panic("Duplicate item name");

        if (last_itm > max_cols) panic("Too many items");
        oo, cl[last_itm - 1].next = last_itm, cl[last_itm].prev = last_itm - 1;

        o, nd[last_itm].up = nd[last_itm].down = last_itm;
        last_itm++;
    }

    if (second == max_cols) second = last_itm;
    oo, cl[last_itm].prev = last_itm - 1, cl[last_itm - 1].next = last_itm;
    oo, cl[second].prev = last_itm, cl[last_itm].next = second;

    oo, cl[root].prev = second - 1, cl[second - 1].next = root;
    last_node = last_itm;

    for (const vector<int>& in_option : in_options) {
        i = last_node;
        pp = 0;
        for (int item : in_option) {
            cl[last_itm].name = item + 1;
            for (k = 0; o, cl[k].name != cl[last_itm].name; k++)
                ;
            if (k == last_itm) panic("Unknown item name");
            if (o, nd[k].aux >= i) panic("Duplicate item name in this option");
            last_node++;
            if (last_node == max_nodes) panic("Too many nodes");
            o, nd[last_node].itm = k;
            if (k < second) pp = 1;
            o, t = nd[k].len + 1;

            o, nd[k].len = t;
            nd[k].aux = last_node;
            o, r = nd[k].up;
            ooo, nd[r].down = nd[k].up = last_node, nd[last_node].up = r, nd[last_node].down = k;
        }
        if (!pp) {
            while (last_node > i) {
                o, k = nd[last_node].itm;
                oo, nd[k].len--, nd[k].aux = i - 1;
                o, q = nd[last_node].up, r = nd[last_node].down;
                oo, nd[q].down = r, nd[r].up = q;

                last_node--;
            }
        } else {
            o, nd[i].down = last_node;
            last_node++;
            if (last_node == max_nodes) panic("Too many nodes");
            options++;
            o, nd[last_node].up = i + 1;
            o, nd[last_node].itm = -options;
        }
    }

    imems = mems, mems = 0;

    level = 0;
forward:
    nodes++;
    if (sanity_checking) sanity();

    if (delta && (mems >= thresh)) {
        thresh += delta;
    }
    if (mems >= timeout) {
        fprintf(stderr, "TIMEOUT!\n");
        goto done;
    }

    t = max_nodes;
    for (o, k = cl[root].next; k != root; o, k = cl[k].next) {
        if (o, nd[k].len <= t) {
            if (nd[k].len < t)
                best_itm = k, t = nd[k].len, p = 1;
            else {
                p++;
            }
        }
    }
    if (t > maxdeg) maxdeg = t;
    if (shape_file) {
        fprintf(shape_file, "" O "d " O ".8s\n", t, cl[best_itm].name);
        fflush(shape_file);
    }

    cover(best_itm);
    oo, cur_node = choice[level] = nd[best_itm].down;

advance:
    if (cur_node == best_itm) goto backup;

    for (pp = cur_node + 1; pp != cur_node;) {
        o, cc = nd[pp].itm;
        if (cc <= 0)
            o, pp = nd[pp].up;
        else
            cover(cc), pp++;
    }

    if (o, cl[root].next == root) {
        nodes++;
        if (level + 1 > maxl) {
            if (level + 1 >= max_level) {
                fprintf(stderr, "Too many levels!\n");
                exit(-5);
            }
            maxl = level + 1;
        }
        if (shape_file) {
            fprintf(shape_file, "sol\n");
            fflush(shape_file);
        }
        {
            count++;
            if (spacing && (count mod spacing == 0)) {
                for (k = 0; k <= level; k++) {
                    vector<int> out_option;
                    print_option(choice[k], out_option);
                    out_options.push_back(out_option);
                };
            }
            if (count >= maxcount) goto done;
            goto recover;
        }
    }

    if (++level > maxl) {
        if (level >= max_level) {
            fprintf(stderr, "Too many levels!\n");
            exit(-4);
        }
        maxl = level;
    }
    goto forward;

backup:
    uncover(best_itm);
    if (level == 0) goto done;
    level--;
    oo, cur_node = choice[level], best_itm = nd[cur_node].itm;

recover:
    for (pp = cur_node - 1; pp != cur_node;) {
        o, cc = nd[pp].itm;
        if (cc <= 0)
            o, pp = nd[pp].down;
        else
            uncover(cc), pp--;
    }

    oo, cur_node = choice[level] = nd[cur_node].down;
    goto advance;

done:
    if (shape_file) fclose(shape_file);

    random_seed = 0;
    randomizing = 0;
    vbose = show_basics + show_warnings;
    spacing = 0;
    show_choices_max = 1000000;
    show_choices_gap = 1000000;

    show_levels_max = 1000000;
    maxl = 0;
    buf[0] = 0;
    count = 0;
    options = 0;
    imems = 0, mems = 0;
    updates = 0;
    bytes = 0;
    nodes = 0;
    thresh = 0;
    delta = 0;
    maxcount = 0xffffffffffffffff;
    timeout = 0x1fffffffffffffff;
    shape_file = 0;
    shape_name = 0;
    maxdeg = 0;

    for (int i = 0; i < last_node; i++) {
        nd[i].up = 0;
        nd[i].down = 0;
        nd[i].itm = 0;
        nd[i].spare = 0;
    }
    last_node = 0;
    for (int i = 0; i < last_itm + 2; i++) {
        cl[i].name = 0;
        cl[i].prev = 0;
        cl[i].next = 0;
    }
    second = max_cols;
    last_itm = 0;

    level = 0;
    for (int i = 0; i < max_level; i++) {
        choice[i] = 0;
        profile[i] = 0;
    }

    return out_options;
}
