#!python

import os, shutil
import json
import glob
import math
import cycler
import itertools as it
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt

# Set INTERACTIVE to True and select image_configs below to view the plots interactively.
# Otherwise, plots will be saved as PDFs to IMAGES_DIR.
INTERACTIVE = False

NAN_IS_TIMEOUT = True
MAX_SECONDS = 1000
MAX_MILLIS = MAX_SECONDS * 1000
IMAGES_DIR = "benchmark/plots"
FIGSIZE_LARGE = (24, 12)
FIGSIZE_PAGE = (8, 8)
N_COLORS = 10
COLOR_MAP = "tab10"
markers = ["o", "s", "D", "^", "v"]

if not INTERACTIVE:
    mpl.rcParams["font.size"] = 12
    mpl.rcParams["font.family"] = "serif"
    mpl.rcParams["font.serif"] = ["Computer Modern"]
    mpl.rcParams["text.usetex"] = True

color_map = plt.get_cmap(COLOR_MAP)
mpl.rcParams["axes.prop_cycle"] = cycler.cycler(
    color=[color_map(i / N_COLORS) for i in range(N_COLORS)],
    marker=[markers[i % len(markers)] for i in range(N_COLORS)],
)


def texttt(s):
    return s if INTERACTIVE else "\\texttt{" + s + "}"


def load_data(paths):
    dfs = []
    for path in paths:
        with open(path) as file:
            df = pd.DataFrame(json.load(file)["benchmarks"])
        df[["problem", "solver"]] = df.name.str.split("@", expand=True)
        df.problem = texttt(df.problem.str.removeprefix("problems/"))
        df.solver = texttt(df.solver)
        dfs.append(df)
    merged_df = pd.concat(dfs)
    columns = merged_df["solver"].unique()  # to keep the solver order
    merged_df = merged_df.pivot(index="problem", columns="solver", values="cpu_time")
    merged_df = merged_df[columns]
    if NAN_IS_TIMEOUT:
        merged_df = merged_df.fillna(MAX_MILLIS)
    return merged_df.clip(upper=MAX_MILLIS)


def filter_data(df, problem_regex, solver_regex):
    if problem_regex:
        df = df.filter(regex=problem_regex, axis="index")
    if solver_regex:
        df = df.filter(regex=solver_regex, axis="columns")
    return df


def show_data(df, sort_by="", cumulative=False, figsize=FIGSIZE_LARGE, file_name=None):
    if sort_by:
        df = df.sort_values(sort_by)

    if cumulative:
        df = df.cumsum()

    # dmin, dmax = df.min().min() / 2, df.max().max() * 2
    dmin, dmax = 0.005, MAX_MILLIS * 2

    plots_fig, plots_ax = plt.subplots(figsize=figsize)
    plot = df.plot(ax=plots_ax, grid=True, logy=True)

    plt.xticks(range(len(df)), df.index, rotation="vertical")
    plt.yticks([10**i for i in range(-10, 10)])
    plt.yticks([j * 10**i for i in range(-10, 10) for j in range(10)], minor=True)
    plt.xlim(-1, len(df))
    plt.ylim(dmin, dmax)
    plt.ylabel("time (ms)")
    plt.tight_layout()

    if file_name:
        plt.savefig(file_name)
        return

    # interactive checkboxes
    lines = plot.lines
    _, check_ax = plt.subplots(figsize=(5, 1 + 0.3 * len(lines)))
    labels = [str(line.get_label()) for line in lines]
    visibility = [True for _ in lines]

    global check  # checkboxes lose interactivity if this gets garbage-collected
    check = mpl.widgets.CheckButtons(check_ax, labels, visibility)

    def toggle_line(label):
        index = labels.index(label)
        lines[index].set_visible(not lines[index].get_visible())
        plots_fig.canvas.draw_idle()

    check.on_clicked(toggle_line)
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

    fig, axs = plt.subplots(h, w, figsize=(18, 9))
    for row in axs:
        for ax in row:
            ax.set_visible(False)
    for i, (s1, s2) in enumerate(solver_pairs):
        ax = axs[i // w, i % w]
        ax.set_visible(True)
        compare_two_solvers(ax, df, s1, s2)

    fig.tight_layout()


paths = sorted(glob.glob("benchmark/*.json"))
data = load_data(paths)

reprs = "simple_default|dlx|sat_cadical_no-breaker_amk-card|ilp_gurobi_adjusted_eq-ign"
image_configs = [
    ("", "simple|dlx", "01-simple-dlx", FIGSIZE_LARGE),
    ("", "sat_cadical_no-breaker", "02-sat-cadical-no-breaker", FIGSIZE_LARGE),
    ("", "sat_cadical_breakid", "03-sat-cadical-breakid", FIGSIZE_LARGE),
    ("", "sat_cms_no-breaker", "04-sat-cms-no-breaker", FIGSIZE_LARGE),
    ("", "sat_cms_breakid", "05-sat-cms-breakid", FIGSIZE_LARGE),
    ("", "sat_.*_amo-bimander", "06-sat-amo-bimander", FIGSIZE_LARGE),
    ("", "sat_.*_amk-card", "07-sat-amk-card", FIGSIZE_LARGE),
    ("", "ilp_cbc_default", "08-ilp-cbc-default", FIGSIZE_LARGE),
    ("", "ilp_cbc_adjusted", "09-ilp-cbc-adjusted", FIGSIZE_LARGE),
    ("", "ilp_gurobi_default", "10-ilp-gurobi-default", FIGSIZE_LARGE),
    ("", "ilp_gurobi_adjusted", "11-ilp-gurobi-adjusted", FIGSIZE_LARGE),
    ("", "ilp_.*_eq-ign", "12-ilp-eq-ign", FIGSIZE_LARGE),
    ("", reprs, "13-representants", FIGSIZE_LARGE),
    ("many-unique/[0-9]", reprs, "case-study-many-unique", FIGSIZE_PAGE),
    ("2/corners|3I/L-square|4I/|4L/", reprs, "case-study-coloring", FIGSIZE_PAGE),
]

if not INTERACTIVE:
    shutil.rmtree(IMAGES_DIR, True)
    os.makedirs(IMAGES_DIR)

for problem_regex, solver_regex, file_name, figsize in image_configs:
    df = filter_data(data, problem_regex=problem_regex, solver_regex=solver_regex)
    show_data(
        df,
        sort_by="",
        cumulative=False,
        figsize=figsize,
        file_name=None if INTERACTIVE else f"{IMAGES_DIR}/{file_name}.pdf",
    )
    if INTERACTIVE:
        plt.show()
