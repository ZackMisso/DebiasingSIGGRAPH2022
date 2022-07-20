# import matplotlib as mpl
# import matplotlib.pyplot as plt
# import matplotlib.font_manager
# from matplotlib import rc
# from pylab import rcParams


# def setup():
#     plt.style.use("ggplot")
#     textColor = '#444444'
#     fontName = 'Linux Biolinium'
#     #mpl.rc('font', family=fontName)
#     mpl.rc('font', size=10)
#     mpl.rc('font', weight='light')
#     #mpl.rc('text', color=textColor)
#     mpl.rc('text', hinting='native')
#     pgf_with_latex = {
#         "pgf.texsystem": "pdflatex",
#         "text.usetex": True,
#         "font.family": "serif",
#         "font.serif": [],
#         "font.sans-serif": [],
#         "font.monospace": [],
#         "axes.titlesize":  7.97,
#         "axes.labelsize":  7.97,
#         "font.size":  7.97,
#         "legend.fontsize":  7.97,
#         "xtick.labelsize":  7.97,
#         "ytick.labelsize":  7.97,
#         # "figure.figsize": figsize(0.9),     # default fig size of 0.9 textwidth
#         "pgf.preamble": r"\usepackage[utf8x]{inputenc} \usepackage[T1]{fontenc}"
#     }
#     mpl.rcParams.update(pgf_with_latex)
