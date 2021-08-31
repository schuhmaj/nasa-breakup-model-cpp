import sys

import numpy
import numpy as np
import scipy as sp
import matplotlib as mpl
import matplotlib.pyplot as plt
import pandas as pd


def histogram_lc(data, title, cumulative):
    lc = data["Characteristic Length [m]"]
    weights = np.ones_like(lc) / float(len(lc))
    plt.hist(lc, density=False, bins=1000, color='b', weights=weights, cumulative=cumulative)
    plt.xscale("log")
    plt.xlabel('$L_c [m]$')
    plt.ylabel('Relative Probability')
    if cumulative:
        plt.title(title + ' $L_c$ Cumulative Distribution')
    else:
        plt.title(title + ' $L_c$ Distribution')
    plt.show()


def histogram_am(data, title, cumulative):
    am = data["A/M [m^2/kg]"]
    weights = np.ones_like(am) / float(len(am))
    plt.hist(am, bins=np.logspace(np.log10(0.001),np.log10(10.0), 50), color='b', weights=weights, cumulative=cumulative)
    plt.xscale("log")
    plt.xlim(10e-4, 10)
    plt.xlabel('$\log(A/M) [m^2/kg]$')
    plt.ylabel('Relative Probability')
    if cumulative:
        plt.title(title + ' $A/M$ Cumulative Distribution')
    else:
        plt.title(title + ' $A/M$ Distribution')
    plt.show()


def scatter_lc_am(data, title):
    data.plot(x="Characteristic Length [m]", y="A/M [m^2/kg]", kind="scatter",
              legend=True, loglog=True, alpha=0.2, color='b')
    plt.title(title)
    plt.xlabel('$L_c [m]$')
    plt.ylabel('$A/M [m^2/kg]$')
    plt.xlim(10e-3, 10)
    plt.ylim(10e-4, 10)
    plt.show()


def main():
    file_name = sys.argv[1]
    plot_name = "Iridium Kosmos Collision"

    df = pd.read_csv(file_name)

    histogram_lc(df, plot_name, False)
    histogram_lc(df, plot_name, True)
    histogram_am(df, plot_name, False)
    histogram_am(df, plot_name, True)
    scatter_lc_am(df, plot_name)


if __name__ == '__main__':
    main()
