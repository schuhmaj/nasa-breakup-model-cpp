import sys

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd


def histogram_lc(data, title, cumulative=False):
    fig = plt.figure(figsize=(6, 4), dpi=300)
    lc = data["Characteristic Length [m]"]
    # weights = np.ones_like(lc) / float(len(lc))
    plt.hist(lc, density=False, bins=np.linspace(0, 5, 50), color='b',
             cumulative=cumulative)
    plt.xlabel('$L_c [m]$')
    plt.ylabel('Number')
    if cumulative:
        plt.title(title + ' $L_c$ Cumulative Distribution')
    else:
        plt.title(title + ' $L_c$ Distribution')
    plt.savefig("histogram_lc_" + title, dpi=300)
    plt.close(fig)


def histogram_am(data, title, cumulative=False):
    fig = plt.figure(figsize=(6, 4), dpi=300)
    am = data["A/M [m^2/kg]"]
    # weights = np.ones_like(am) / float(len(am))
    plt.hist(am, bins=np.logspace(np.log10(0.001), np.log10(10.0), 25), color='b',
             cumulative=cumulative)
    plt.xscale("log")
    plt.xlabel('A/M [m^2/kg]$')
    plt.ylabel('Number')
    if cumulative:
        plt.title(title + ' $A/M$ Cumulative Distribution')
    else:
        plt.title(title + ' $A/M$ Distribution')
    plt.savefig("histogram_am_" + title, dpi=300)
    plt.close(fig)


def histogram_am_log(data, title):
    fig = plt.figure(figsize=(6, 4), dpi=300)
    am = np.log10(data["A/M [m^2/kg]"])
    # weights = np.ones_like(am) / float(len(am))
    plt.hist(am, bins=np.linspace(-3, 2, 25), color='b')
    plt.xlabel('$\log(A/M) [m^2/kg]$')
    plt.ylabel('Number')
    plt.title(title + " $A/M$ Distribution")
    plt.savefig("histogram_am_log_" + title, dpi=300)
    plt.close(fig)


def scatter_lc_am(data, title):
    fig = plt.figure(figsize=(6, 4), dpi=300)
    data.plot(x="Characteristic Length [m]", y="A/M [m^2/kg]", kind="scatter",
              legend=True, loglog=True, alpha=0.2, color='b')
    plt.title(title)
    plt.xlabel('$L_c [m]$')
    plt.ylabel('$A/M [m^2/kg]$')
    plt.xlim(0.001, 10)
    plt.ylim(0.001, 100)
    plt.savefig("scatter_lc_am_" + title, dpi=300)
    plt.close(fig)


def scatter_dv_am(data, title):
    fig = plt.figure(figsize=(6, 4), dpi=300)
    velocity = np.array([np.fromstring(ele[1:-1], sep=' ') for ele in data["Ejection Velocity [m/s]"].to_numpy()])
    scalar = np.linalg.norm(velocity, axis=1)

    plt.scatter(x=scalar, y=data["A/M [m^2/kg]"], alpha=0.2, color='b', marker='.')
    plt.title(title)
    plt.xlabel('$\Delta V [m/s]$')
    plt.ylabel('$A/M [m^2/kg]$')
    plt.yscale("log")
    plt.xlim(0, 10000)
    plt.ylim(0.001, 100)
    plt.savefig("scatter_dv_am_" + title, dpi=300)
    plt.close(fig)


def scatter_dv_am_log(data, title):
    fig = plt.figure(figsize=(6, 4), dpi=300)
    velocity = np.array([np.fromstring(ele[1:-1], sep=' ') for ele in data["Ejection Velocity [m/s]"].to_numpy()])
    scalar = np.linalg.norm(velocity, axis=1)

    plt.scatter(x=scalar, y=data["A/M [m^2/kg]"], alpha=0.2, color='b', marker='.')
    plt.title(title)
    plt.xlabel('$\Delta V [m/s]$')
    plt.ylabel('$A/M [m^2/kg]$')
    plt.yscale("log")
    plt.xscale("log")
    plt.ylim(0.001, 100)
    plt.savefig("scatter_dv_am_log_" + title, dpi=300)
    plt.close(fig)


def main():
    file_name = sys.argv[1]
    plot_name = sys.argv[2]

    df = pd.read_csv(file_name)
    # df = df[df["Name"] == "Iridium 33-Collision-Fragment"]
    # df = df[df["Name"] == "Kosmos 2251-Collision-Fragment"]
    # df = df[df["Characteristic Length [m]"] != 0.0]
    # df = df[df["A/M [m^2/kg]"] > 0.0]

    histogram_lc(df, plot_name)
    histogram_am(df, plot_name)
    histogram_am_log(df, plot_name)
    scatter_lc_am(df, plot_name)
    scatter_dv_am(df, plot_name)
    scatter_dv_am_log(df, plot_name)


if __name__ == '__main__':
    main()
