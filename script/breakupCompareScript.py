import csv
import sys
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd


def compare_histogram_lc(data_cpp, data_python, title):
    fig = plt.figure(figsize=(6, 4), dpi=300)
    bins = np.linspace(0, 5.0, 100)
    plt.hist(data_cpp["Characteristic Length [m]"], bins, alpha=0.5, log=True, label="C++", color='blue')
    plt.hist(data_python["Characteristic Length [m]"], bins, alpha=0.5, log=True, label="Python", color='red')
    plt.legend(loc='upper right')
    plt.xlabel("$L_c$ in [m]")
    plt.ylabel("Number")
    plt.title("$L_c$ Distribution C++ vs. Python for " + title)
    plt.savefig("compare_histogram_lc_CPP_vs_Python", dpi=300)
    plt.close(fig)


def compare_histogram_am(data_cpp, data_python, title):
    fig = plt.figure(figsize=(6, 4), dpi=300)
    bins = np.linspace(0, 5.0, 100)
    plt.hist(data_cpp["A/M [m^2/kg]"], bins, alpha=0.5, log=True, label="C++", color='blue')
    plt.hist(data_python["A/M [m^2/kg]"], bins, alpha=0.5, log=True, label="Python", color='red')
    plt.legend(loc='upper right')
    plt.xlabel("$L_c$ in [m]")
    plt.ylabel("Number")
    plt.title("$A/M$ Distribution C++ vs. Python for " + title)
    plt.savefig("compare_histogram_am_CPP_vs_Python", dpi=300)
    plt.close(fig)


def main():
    file_name_cpp = sys.argv[1]
    file_name_python = sys.argv[2]
    plot_name = sys.argv[3]

    df_cpp = pd.read_csv(file_name_cpp)
    df_python = pd.read_csv(file_name_python)

    compare_histogram_lc(df_cpp, df_python, plot_name)
    compare_histogram_am(df_cpp, df_python, plot_name)


if __name__ == '__main__':
    main()
