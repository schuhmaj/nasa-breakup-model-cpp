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
    plt.xlabel("$A/M$ in [m^2/kg]")
    plt.ylabel("Number")
    plt.title("$A/M$ Distribution C++ vs. Python for " + title)
    plt.savefig("compare_histogram_am_CPP_vs_Python", dpi=300)
    plt.close(fig)


def compare_histogram_dv(data_cpp, data_python, title):
    fig = plt.figure(figsize=(6, 4), dpi=300)
    bins = np.linspace(0, 10000, 100)
    velocity_cpp = np.array([np.fromstring(ele[1:-1], sep=' ') for ele in data_cpp["Ejection Velocity [m/s]"].to_numpy()])
    velocity_python = np.array([np.fromstring(ele[1:-1], sep=' ') for ele in data_python["Ejection Velocity [m/s]"].to_numpy()])

    scalar_cpp = np.linalg.norm(velocity_cpp, axis=1)
    scalar_python = np.linalg.norm(velocity_python, axis=1)

    plt.hist(scalar_cpp, bins, alpha=0.5, log=True, label="C++", color='blue')
    plt.hist(scalar_python, bins, alpha=0.5, log=True, label="Python", color='red')
    plt.legend(loc='upper right')
    plt.xlabel("$\Delta V$ in [m/s]")
    plt.ylabel("Number")
    plt.title("$\Delta V$ Distribution C++ vs. Python for " + title)
    plt.savefig("compare_histogram_dv_CPP_vs_Python", dpi=300)
    plt.close(fig)


def compare_histogram_lc_log(data_cpp, data_python, title):
    fig = plt.figure(figsize=(6, 4), dpi=300)
    log_cpp = np.log10(data_cpp["Characteristic Length [m]"])
    log_python = np.log10(data_python["Characteristic Length [m]"])
    plt.hist(log_cpp, bins=50, alpha=0.5, log=True, label="C++", color='blue')
    plt.hist(log_python, bins=50, alpha=0.5, log=True, label="Python", color='red')
    plt.legend(loc='upper right')
    plt.xlabel("$\log_{10}(L_c)$")
    plt.ylabel("Number")
    plt.title("$\log_{10}(L_c)$ Distribution C++ vs. Python for " + title)
    plt.savefig("compare_histogram_lc_CPP_vs_Python_log", dpi=300)
    plt.close(fig)


def compare_histogram_am_log(data_cpp, data_python, title):
    fig = plt.figure(figsize=(6, 4), dpi=300)
    log_cpp = np.log10(data_cpp["A/M [m^2/kg]"])
    log_python = np.log10(data_python["A/M [m^2/kg]"])
    plt.hist(log_cpp, bins=50, alpha=0.5, log=True, label="C++", color='blue')
    plt.hist(log_python, bins=50, alpha=0.5, log=True, label="Python", color='red')
    plt.legend(loc='upper right')
    plt.xlabel("$\log_{10}(A/M)$")
    plt.ylabel("Number")
    plt.title("$\log_{10}(A/M)$ Distribution C++ vs. Python for " + title)
    plt.savefig("compare_histogram_am_CPP_vs_Python_log", dpi=300)
    plt.close(fig)


def compare_scatter_lc_am(data_cpp, data_python, title):
    fig = plt.figure(figsize=(6, 4), dpi=300)
    plt.scatter(x=data_cpp["Characteristic Length [m]"], y=data_cpp["A/M [m^2/kg]"], alpha=0.2, color='b', marker='.')
    plt.scatter(x=data_python["Characteristic Length [m]"], y=data_python["A/M [m^2/kg]"], alpha=0.2, color='r', marker='.')
    plt.xscale("log")
    plt.yscale("log")
    plt.title(title)
    plt.xlabel('$L_c [m]$')
    plt.ylabel('$A/M [m^2/kg]$')
    plt.xlim(0.001, 10)
    plt.ylim(0.001, 100)
    plt.savefig("compare_scatter_lc_am_" + title, dpi=300)
    plt.close(fig)


def compare_scatter_dv_am_log(data_cpp, data_python, title):
    fig = plt.figure(figsize=(6, 4), dpi=300)

    velocity_cpp = np.array([np.fromstring(ele[1:-1], sep=' ') for ele in data_cpp["Ejection Velocity [m/s]"].to_numpy()])
    velocity_python = np.array([np.fromstring(ele[1:-1], sep=' ') for ele in data_python["Ejection Velocity [m/s]"].to_numpy()])

    scalar_cpp = np.linalg.norm(velocity_cpp, axis=1)
    scalar_python = np.linalg.norm(velocity_python, axis=1)

    plt.scatter(x=scalar_cpp, y=data_cpp["A/M [m^2/kg]"], alpha=0.2, color='b', marker='.')
    plt.scatter(x=scalar_python, y=data_python["A/M [m^2/kg]"], alpha=0.2, color='r', marker='.')
    plt.title(title)
    plt.xlabel('$\Delta V [m/s]$')
    plt.ylabel('$A/M [m^2/kg]$')
    plt.yscale("log")
    plt.xscale("log")
    plt.ylim(0.001, 100)
    plt.savefig("compare_scatter_dv_am_log_" + title, dpi=300)
    plt.close(fig)


def main():
    file_name_cpp = sys.argv[1]
    file_name_python = sys.argv[2]
    plot_name = sys.argv[3]

    df_cpp = pd.read_csv(file_name_cpp)
    df_python = pd.read_csv(file_name_python)

    compare_histogram_lc(df_cpp, df_python, plot_name)
    compare_histogram_lc_log(df_cpp, df_python, plot_name)
    compare_histogram_am(df_cpp, df_python, plot_name)
    compare_histogram_am_log(df_cpp, df_python, plot_name)
    compare_histogram_dv(df_cpp, df_python, plot_name)
    compare_scatter_lc_am(df_cpp, df_python, plot_name)
    compare_scatter_dv_am_log(df_cpp, df_python, plot_name)


if __name__ == '__main__':
    main()
