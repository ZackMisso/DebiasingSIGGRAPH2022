import sys
import matplotlib.pyplot as plt

graph_title = ""
xaxis_title = ""
yaxis_title = ""

is_x_log = False
is_y_log = False

path_to_data = sys.argv[1]
path_to_results = sys.argv[2]

index = 3

# while index < len(sys.argv):
#     if (sys.argv[index].strip() == "--x_log"):
#         is_x_log = True
#     if (sys.argv[index].strip() == "--y_log"):
#         is_y_log = True
#     if (sys.argv[index].strip() == "--title"):
#         index = index + 1
#         graph_title = sys.argv[index].strip()
#     if (sys.argv[index].strip() == "--x_title"):
#         index = index + 1
#         xaxis_title = sys.argv[index].strip()
#     if (sys.argv[index].strip() == "--y_title"):
#         index = index + 1
#         yaxis_title = sys.argv[index].strip()
#     index = index + 1

data = open(path_to_data).readlines()

xs = []
ys = []

# for i in range(len(data)):
#     if i % 2 == 0:
#         xs.append(data[i])
#     else:
#         ys.append(data[i])

print(xs)
print(ys)

for i in range(len(data)):
    if i % 2 == 0:
        xs.append(float(data[i]))
    else:
        ys.append(float(data[i]))

# print(xs)
# print(len(ys))
# print(ys[0])

# if (is_x_log):
#     plt.xscale("log", basex=2)
# if (is_y_log):
#     plt.yscale("log", basey=2)

plt.plot(xs, ys, "-b")

plt.savefig(path_to_results)

plt.clf()
