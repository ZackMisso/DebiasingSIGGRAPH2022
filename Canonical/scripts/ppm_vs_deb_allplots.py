# import setup
import sys
import matplotlib.pyplot as plt
# import setup

import matplotlib as mpl
# import matplotlib.pyplot as plt
import matplotlib.font_manager
from matplotlib import rc
from pylab import rcParams
import numpy as np
import math


from matplotlib.pyplot import figure

# figure(figsize=(30, 16), dpi=200)


def setup():
    plt.style.use("ggplot")
    textColor = '#444444'
    fontName = 'Linux Libertine'  # only section headings change to libertine
    # mpl.rc('font', family=fontName)
    mpl.rc('font', size=20)
    mpl.rc('font', weight='light')
    # mpl.rc('text', color=textColor)
    mpl.rc('text', hinting='native')
    siz = 16.0  # 7.97
    # siz = 7.97
    pgf_with_latex = {
        "pgf.texsystem": "pdflatex",
        "text.usetex": True,
        "font.family": "serif",
        # "font.serif": 'Linux Libertine O',
        "font.sans-serif": [],
        "font.monospace": [],
        "axes.titlesize":  siz,
        "axes.labelsize":  siz,
        "font.size":  siz,
        "legend.fontsize":  siz,
        "xtick.labelsize":  siz,
        "ytick.labelsize":  siz,
        # "figure.figsize": figsize(0.9),     # default fig size of 0.9 textwidth
        "pgf.preamble": r"\usepackage{libertine} \usepackage[libertine]{newtxmath} \usepackage[T1]{fontenc}"
    }
    mpl.rcParams.update(pgf_with_latex)


setup()

# matplotlib.rcParams.update(matplotlib.rcParamsDefault)
# plt.rcParams['font.family'] = "serif"
# plt.rcParams['text.latex.preamble'] = r'\usepackage{libertine} \usepackage[libertine]{newtxmath} \usepackage[T1]{fontenc}'
# plt.rcParams['text.usetex'] = True
# plt.rcParams['font.size'] = 25
# plt.rcParams["figure.dpi"] = 80

# This is meant to be ran from within the same folder
# TODO: remove this as a script eventually

# tests = ["0_20", "0_25", "0_30", "0_35", "0_40", "0_45", "0_50", "0_55",
#          "0_60", "0_65", "0_70", "0_75", "0_80", "0_85", "0_90", "0_95"]

# for test in tests:
graph_title = ""
xaxis_title = ""
yaxis_title = ""

is_x_log = False
is_y_log = False

iters = 1
alph = 1.0 / (float(iters)/2.0)
if (alph > 1.0):
    alph = 1.0
path_to_results = "../results/photon_deb_vs_ppm/full_test/0_67_res/" + \
    str(iters)

graph_title = ""
xaxis_title = "cost (photons emitted)"
yaxis_title = "MSE"

# names = ["old debiased uniform", "ppm",
#          "Close debiased uniform", "debiased cone", "debiased quad"]

names = ["PPM", "Unbiased PM", "Unbiased PPM"]

avg_ppm = []
avg_deb_sqr = []
avg_deb_alp = []
avg_deb_inf = []
avg_ppm_err = []
avg_deb_sqr_err = []
avg_deb_alp_err = []
avg_deb_inf_err = []
avg_cost = []

start_k = 100000000

data_avg = open(
    "../results/photon_deb_vs_ppm/full_test/avg_equal_cost_ppm.txt").readlines()
siz = int(len(data_avg)/3)
for k in range(0, siz):
    val = float(data_avg[3*k])
    if val > pow(2.0, 31.2):
        avg_cost.append(float(data_avg[3*k]))
        avg_ppm.append(float(data_avg[3*k+1]))
        avg_ppm_err.append(float(data_avg[3*k+2]))
        start_k = min(k, start_k)

data_avg_2 = open(
    "../results/photon_deb_vs_ppm/full_test/avg_equal_cost_deb_cone_alpha_final.txt").readlines()
siz = int(len(data_avg_2)/3)
for k in range(start_k, siz):
    avg_deb_sqr.append(float(data_avg_2[3*k+1]))
    avg_deb_sqr_err.append(float(data_avg_2[3*k+2]))

data_avg_3 = open(
    "../results/photon_deb_vs_ppm/full_test/avg_equal_cost_debprog_cone_alpha_final.txt").readlines()
siz = int(len(data_avg_3)/3)
for k in range(start_k, siz):
    avg_deb_alp.append(float(data_avg_3[3*k+1]))
    avg_deb_alp_err.append(float(data_avg_3[3*k+2]))


avg_ppm_err_base = avg_ppm_err[0]
for i in range(0, len(avg_ppm_err)):
    # print("old: " + str(avg_ppm_err[i]))
    avg_ppm_err[i] = avg_ppm_err[i] / avg_ppm_err_base
    # print("new: " + str(avg_ppm_err[i]))

avg_deb_sqr_err_base = avg_deb_sqr_err[0]
for i in range(0, len(avg_deb_sqr_err)):
    avg_deb_sqr_err[i] = avg_deb_sqr_err[i] / avg_deb_sqr_err_base

base = avg_deb_alp_err[0]
for i in range(0, len(avg_deb_alp_err)):
    avg_deb_alp_err[i] = avg_deb_alp_err[i] / avg_deb_sqr_err_base


# print(start_k)
# # data_avg_4 = open(
# #     "../results/photon_deb_vs_ppm/full_test/avg_equal_cost_deb_cone_alpha_80.txt").readlines()
# # siz = int(len(data_avg_4)/3)
# # for k in range(0, siz):
# #     avg_deb_inf.append(float(data_avg_4[3*k+1]))
# #     avg_deb_inf_err.append(float(data_avg_4[3*k+2]))

# # data_avg_4 = open(
# #     "../results/photon_deb_vs_ppm/full_test/avg_equal_cost_deb_quad_square.txt").readlines()
# # siz = int(len(data_avg_4)/3)
# # for k in range(0, siz):
# #     avg_deb_quad.append(float(data_avg_4[2*k+1]))

# # plt.title(graph_title)
# plt.xlabel(xaxis_title)
# plt.ylabel(yaxis_title)

# # print(len(avg_ppm))
# # print(len(avg_deb_uni))
# # print(len(avg_deb_cone))
# # print(len(avg_deb_quad))

# print(len(avg_deb_alp))

# plt.grid()

# plt.plot(avg_cost, avg_ppm, "-", label=names[0])
# plt.plot(avg_cost, avg_deb_sqr, "-", label=names[1])
# plt.plot(avg_cost, avg_deb_alp, "-", label=names[2])
# # plt.plot(avg_cost, avg_deb_inf, "-", label=names[3])

# plt.grid()
# plt.xscale("log", basex=2)
# plt.yscale("log", basey=2)

# # plt.xlim(pow(2.0, 31.2), avg_cost[len(avg_cost)-1])

# plt.legend()
# plt.savefig(path_to_results + ".png")
# plt.savefig(path_to_results + ".pgf")
# plt.savefig(path_to_results + ".pdf")
# plt.clf()

# graph_title = "Debiased Vs. PPM with base radius = 0.125"
xaxis_title = "cost (photons emitted)"
yaxis_title = "Mean absolute error"

path_to_results = "../results/photon_deb_vs_ppm/full_test/0_67_res/deb_vs_ppm_all_error_" + \
    str(iters)

# plt.title(graph_title)

# print(len(avg_ppm))
# print(len(avg_deb_uni))
# print(len(avg_deb_cone))
# print(len(avg_deb_quad))

# plt.xlim(pow(2.0, 31.2), avg_cost[len(avg_cost)-1])

# plt.grid()

fig, ax = plt.subplots()
# fig.set_size_inches(8, 4, forward=True)
fig.set_size_inches(8, 4, forward=True)

# ax.xlabel(xaxis_title)
# ax.ylabel(yaxis_title)

plt.xscale("log", basex=2)
plt.yscale("log", basey=2)
plt.xlim(pow(2.0, 31.2), avg_cost[len(avg_cost)-1])
plt.grid()

lns1 = ax.plot(avg_cost, avg_ppm_err, "-", label=names[0])
# ax.plot(avg_cost, avg_deb_sqr_err, "g-", label=names[1])
# ax.plot(avg_cost, avg_deb_alp_err, "b-", label=names[2])

ax.legend(loc='lower left')

print(avg_deb_sqr_err[0])

ax2 = ax.twinx()
plt.xscale("log", basex=2)
plt.yscale("log", basey=2)
lns2 = ax2.plot(avg_cost, avg_deb_sqr_err, "--",
                label=names[1], color='#348ABD')
lns3 = ax2.plot(avg_cost, avg_deb_alp_err, "-",
                label=names[2], color='#348ABD')
# plt.plot(avg_cost, avg_deb_inf_err, "-", label=names[3])
# plt.xlim(pow(2.0, 31.2), avg_cost[len(avg_cost)-1])

print(plt.rcParams['axes.prop_cycle'].by_key()['color'])

# plt.grid()
# plt.xscale("log", basex=2)
# plt.yscale("log", basey=2)

# labels = [item.get_text() for item in ax2.get_yticklabels()]

labels = ["2.5e-3", "1.2e-3",
          "6.2e-4"]
labels = [r"$2.5\textsc{e}$-$3$", r"$1.2\textsc{e}$-$3$",
          r"$6.2\textsc{e}$-$4$"]
ax.set_yticks([1.0, 0.5, 0.25])
# ax.set_yticklabels(labels, rotation=90, va="center", color="r")
ax.set_yticklabels(labels, color='#E24A33')


# ax.tick_params(axis='y', labelrotation=90, va="center")
# for tick in ax.get_yticklabels():
#     tick.set_rotation(90)

labels = ["1.2e+0", "6.1e-1",
          "3.0e-1"]
labels = [r"$1.2\textsc{e}$+$0$", r"$6.1\textsc{e}$-$1$",
          r"$3.0\textsc{e}$-$1$"]
ax2.set_yticks([1.0, 0.5, 0.25])
# ax2.set_yticklabels(labels, rotation=-90, va="center", color="b")
ax2.set_yticklabels(labels, color='#348ABD')

# for tick in ax2.get_yticklabels():
#     tick.set_rotation(90)

# labels[1] = 'Testing'
print("labels: " + labels[1])

ax.set_xlabel(xaxis_title)
ax.set_ylabel(yaxis_title)

# ax.set_xticklabels(labels)

# lns = lns1+lns2+lns3
# labs = [l.get_label() for l in lns]
# ax.legend(lns, labs)

ax2.legend()

# plt.tight_layout()
plt.tight_layout(rect=[-0.028, -0.058, 1.028, 1.058])

plt.savefig(path_to_results + ".png")
plt.savefig(path_to_results + ".pgf")
plt.savefig(path_to_results + ".pdf")
plt.clf()


# for k in range(0, iters):
#     print("ITER: " + str(k))
#     paths_to_data = []
#     paths_to_data.append(
#         "../results/photon_deb_vs_ppm/r_0_125_many/equal_cost_deb_"+str(k)+".txt")
#     paths_to_data.append(
#         "../results/photon_deb_vs_ppm/r_0_125_many/equal_cost_ppm_"+str(k)+".txt")

#     is_x_log = True
#     is_y_log = True
#     conv_lines = False

#     def parse_all_data(paths):
#         xs = []
#         ys = []

#         for i in range(0, len(paths)):
#             data = open(paths[i]).readlines()
#             tmp_y = []
#             tmp_x = []
#             for j in range(len(data)):
#                 if (j % 4 == 1):  # cost
#                     tmp_x.append(float(data[j]))
#                 elif j % 4 == 3:  # mse
#                     tmp_y.append(float(data[j]))
#             ys.append(tmp_y)
#             xs.append(tmp_x)

#         return xs, ys

#     xs, ys = parse_all_data(paths_to_data)

#     # print(xs)
#     # print(len(ys))
#     # print(ys[0])

#     # slopes_x = []
#     # slopes = []

#     # for i in range(1, len(xs)):
#     #     slopes.append((math.log2(ys[0][i]) - math.log2(ys[0][i-1])) /
#     #                   (math.log2(xs[i]) - math.log2(xs[i-1])))
#     #     slopes_x.append(xs[i])

#     final_xs = []
#     final_ys = []

#     start_iter = 0
#     # start_iter = 2048
#     # print(len(xs))

#     for i in range(0, len(xs)):
#         if (i >= start_iter):
#             final_xs.append(xs[i])
#             final_ys.append(ys[0][i])

#     if conv_lines:
#         start_y = ys[0][start_iter]
#         start_x = xs[start_iter]

#         # half_slope = [start_y / math.pow(start_x, -0.5)]
#         # one_slope = [start_y / math.pow(start_x, -1.0)]
#         # two_slope = [start_y / math.pow(start_x, -2.0/3.0)]
#         # onehalf_slope = [start_y / math.pow(start_x, -1.5)]
#         half_slope = [start_y]
#         one_slope = [start_y]
#         two_slope = [start_y]
#         onehalf_slope = [start_y]

#         for i in range(1, len(final_xs)):
#             half_slope.append(
#                 half_slope[0] / math.pow(start_x, -0.5) * math.pow(xs[i+start_iter], -0.5))
#             one_slope.append(
#                 one_slope[0] / math.pow(start_x, -1.0) * math.pow(xs[i+start_iter], -1.0))
#             two_slope.append(
#                 two_slope[0] / math.pow(start_x, -2.0/3.0) * math.pow(xs[i+start_iter], -2.0/3.0))
#             onehalf_slope.append(
#                 onehalf_slope[0] / math.pow(start_x, -1.5) * math.pow(xs[i+start_iter], -1.5))

#         plt.plot(final_xs, half_slope, "--k", alpha=0.2)
#         plt.plot(final_xs, one_slope, "--k", alpha=0.2)
#         plt.plot(final_xs, onehalf_slope, "--k", alpha=0.2)
#         plt.plot(final_xs, two_slope, "--r", alpha=0.2)

#     plt.grid()

#     # for i in range(1, len(xs)):
#     #     one_slope.append(one_slope[i-1] * xs[i] / xs[i-1])

#     if (is_x_log):
#         plt.xscale("log", basex=2)
#     if (is_y_log):
#         plt.yscale("log", basey=2)

#     if (k == iters-1):
#         plt.plot(xs[0], ys[0], "r-", alpha=alph, label=names[0])
#         plt.plot(xs[1], ys[1], "b-", alpha=alph, label=names[1])
#     else:
#         plt.plot(xs[0], ys[0], "r-", alpha=alph)
#         plt.plot(xs[1], ys[1], "b-", alpha=alph)

# for i in range(len(ys)):
#     plt.plot(final_xs, final_ys, "-")


# plt.plot(avg_cost, avg_ppm, "b-", label=names[1])
# plt.plot(avg_cost, avg_deb, "r-", label=names[0])

# plt.legend(loc=3)
# plt.savefig(path_to_results)
# plt.clf()
