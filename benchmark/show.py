#!python

import json
import glob
import math
import cycler
import itertools as it
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt

FIGSIZE = (18, 9)
VMIN, VMAX = 10**-10, 10**10
COLORS = 20

cm = plt.get_cmap("tab20")
mpl.rcParams["axes.prop_cycle"] = cycler.cycler(
    color=[cm(i / COLORS) for i in range(COLORS)]
)


def load_data(paths):
    dfs = []
    for path in paths:
        with open(path) as file:
            df = pd.DataFrame(json.load(file)["benchmarks"])
        df[["problem", "solver"]] = df.name.str.split("@", expand=True)
        df.problem = df.problem.str.removeprefix("problems/")
        dfs.append(df)
    return pd.concat(dfs).pivot("problem", "solver", "cpu_time")


def filter_data(df, problem_regex, solver_regex):
    if problem_regex:
        df = df.filter(regex=problem_regex, axis="index")
    if solver_regex:
        df = df.filter(regex=solver_regex, axis="columns")
    return df


def show_data(df, sort_by):
    if sort_by:
        df = df.sort_values(sort_by)

    dmin, dmax = df.min().min() / 2, df.max().max() * 2

    df.plot(figsize=FIGSIZE, style="o-", grid=True, logy=True)

    plt.xticks(range(len(df)), df.index, rotation="vertical")
    plt.yticks([10**i for i in range(-10, 10)])
    plt.xlim(-1, len(df))
    plt.ylim(dmin, dmax)

    plt.tight_layout()


def compare_two_solvers(ax, df, solver1, solver2):
    df.plot.scatter(solver1, solver2, ax=ax, logx=True, logy=True)

    dmin, dmax = df.min().min() / 2, df.max().max() * 2

    ax.axline((dmin, dmin), (dmax, dmax), color="k", lw=0.2)
    ax.set_xlim(dmin, dmax)
    ax.set_ylim(dmin, dmax)
    ax.set_aspect(1)


def compare_all_solvers(df):
    solver_pairs = list(it.combinations(df.columns, 2))
    h = math.ceil((len(solver_pairs) / 2) ** 0.5)
    w = 2 * h

    fig, axs = plt.subplots(h, w, figsize=FIGSIZE)
    for row in axs:
        for ax in row:
            ax.set_visible(False)
    for i, (s1, s2) in enumerate(solver_pairs):
        ax = axs[i // w, i % w]
        ax.set_visible(True)
        compare_two_solvers(ax, df, s1, s2)

    fig.tight_layout()


paths = sorted(glob.glob("benchmark/*.json"))
df = filter_data(load_data(paths), "", "cadical")
show_data(df, "")
compare_all_solvers(df)

plt.show()
