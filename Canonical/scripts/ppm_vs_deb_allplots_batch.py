# # import setup
# import sys
# import matplotlib.pyplot as plt
# #import setup

# import matplotlib as mpl
# #import matplotlib.pyplot as plt
# import matplotlib.font_manager
# from matplotlib import rc
# from pylab import rcParams
# import numpy as np
# import math

# for ind in range(30, 90):
#     if ind >= 36 and (ind < 48 or ind > 50) and (ind < 72 or ind > 74):
#         # for test in tests:
#         graph_title = ""
#         xaxis_title = ""
#         yaxis_title = ""

#         is_x_log = False
#         is_y_log = False

#         iters = 512
#         alph = 1.0 / (float(iters)/2.0)
#         if (alph > 1.0):
#             alph = 1.0
#         path_to_results = "../results/photon_deb_vs_ppm/r_0_1250_low_many/deb_vs_ppm_all_" + \
#             str(iters)+".png"

#         graph_title = "Debiased Vs. PPM with base radius = 0.125"
#         xaxis_title = "cost (photons emitted)"
#         yaxis_title = "MSE"

#         names = ["debiased", "ppm"]

#         avg_ppm = []
#         avg_deb = []
#         avg_cost = []

#         data_avg = open(
#             "../results/photon_deb_vs_ppm/r_0_1250_low_many/avg_equal_cost_ppm.txt").readlines()
#         siz = int(len(data_avg)/2)
#         for k in range(0, siz):
#             avg_cost.append(float(data_avg[2*k]))
#             avg_ppm.append(float(data_avg[2*k+1]))

#         data_avg_2 = open(
#             "../results/photon_deb_vs_ppm/r_0_1250_low_many/avg_equal_cost_deb.txt").readlines()
#         siz = int(len(data_avg_2)/2)
#         for k in range(0, siz):
#             avg_deb.append(float(data_avg_2[2*k+1]))

#         plt.title(graph_title)
#         plt.xlabel(xaxis_title)
#         plt.ylabel(yaxis_title)

#         for k in range(0, iters):
#             print("ITER: " + str(k))
#             paths_to_data = []
#             paths_to_data.append(
#                 "../results/photon_deb_vs_ppm/r_0_1250_low_many/equal_cost_deb_"+str(k)+".txt")
#             paths_to_data.append(
#                 "../results/photon_deb_vs_ppm/r_0_1250_low_many/equal_cost_ppm_"+str(k)+".txt")

#             is_x_log = True
#             is_y_log = True
#             conv_lines = False

#             def parse_all_data(paths):
#                 xs = []
#                 ys = []

#                 for i in range(0, len(paths)):
#                     data = open(paths[i]).readlines()
#                     tmp_y = []
#                     tmp_x = []
#                     for j in range(len(data)):
#                         if (j % 4 == 1):  # cost
#                             tmp_x.append(float(data[j]))
#                         elif j % 4 == 3:  # mse
#                             tmp_y.append(float(data[j]))
#                     ys.append(tmp_y)
#                     xs.append(tmp_x)

#                 return xs, ys

#             xs, ys = parse_all_data(paths_to_data)

#             # print(xs)
#             # print(len(ys))
#             # print(ys[0])

#             # slopes_x = []
#             # slopes = []

#             # for i in range(1, len(xs)):
#             #     slopes.append((math.log2(ys[0][i]) - math.log2(ys[0][i-1])) /
#             #                   (math.log2(xs[i]) - math.log2(xs[i-1])))
#             #     slopes_x.append(xs[i])

#             final_xs = []
#             final_ys = []

#             start_iter = 0
#             # start_iter = 2048
#             # print(len(xs))

#             for i in range(0, len(xs)):
#                 if (i >= start_iter):
#                     final_xs.append(xs[i])
#                     final_ys.append(ys[0][i])

#             if conv_lines:
#                 start_y = ys[0][start_iter]
#                 start_x = xs[start_iter]

#                 # half_slope = [start_y / math.pow(start_x, -0.5)]
#                 # one_slope = [start_y / math.pow(start_x, -1.0)]
#                 # two_slope = [start_y / math.pow(start_x, -2.0/3.0)]
#                 # onehalf_slope = [start_y / math.pow(start_x, -1.5)]
#                 half_slope = [start_y]
#                 one_slope = [start_y]
#                 two_slope = [start_y]
#                 onehalf_slope = [start_y]

#                 for i in range(1, len(final_xs)):
#                     half_slope.append(
#                         half_slope[0] / math.pow(start_x, -0.5) * math.pow(xs[i+start_iter], -0.5))
#                     one_slope.append(
#                         one_slope[0] / math.pow(start_x, -1.0) * math.pow(xs[i+start_iter], -1.0))
#                     two_slope.append(
#                         two_slope[0] / math.pow(start_x, -2.0/3.0) * math.pow(xs[i+start_iter], -2.0/3.0))
#                     onehalf_slope.append(
#                         onehalf_slope[0] / math.pow(start_x, -1.5) * math.pow(xs[i+start_iter], -1.5))

#                 plt.plot(final_xs, half_slope, "--k", alpha=0.2)
#                 plt.plot(final_xs, one_slope, "--k", alpha=0.2)
#                 plt.plot(final_xs, onehalf_slope, "--k", alpha=0.2)
#                 plt.plot(final_xs, two_slope, "--r", alpha=0.2)

#             plt.grid()

#             # for i in range(1, len(xs)):
#             #     one_slope.append(one_slope[i-1] * xs[i] / xs[i-1])

#             if (is_x_log):
#                 plt.xscale("log", basex=2)
#             if (is_y_log):
#                 plt.yscale("log", basey=2)

#             if (k == iters-1):
#                 plt.plot(xs[0], ys[0], "r-", alpha=alph, label=names[0])
#                 plt.plot(xs[1], ys[1], "b-", alpha=alph, label=names[1])
#             else:
#                 plt.plot(xs[0], ys[0], "r-", alpha=alph)
#                 plt.plot(xs[1], ys[1], "b-", alpha=alph)

#             # for i in range(len(ys)):
#             #     plt.plot(final_xs, final_ys, "-")

#         plt.plot(avg_cost, avg_ppm, "b-", label=names[1])
#         plt.plot(avg_cost, avg_deb, "r-", label=names[0])

#         plt.legend(loc=3)
#         plt.savefig(path_to_results)
#         plt.clf()
