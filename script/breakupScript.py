import sys

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd


def histogram_lc(data, title):
    fig = plt.figure(figsize=(6, 4), dpi=300)
    lc = data["Characteristic Length [m]"]
    # weights = np.ones_like(lc) / float(len(lc))
    plt.hist(lc, density=False, bins=np.logspace(np.log10(0.05), np.log10(5.0), 50), color='b')
    plt.xlabel('$L_c [m]$')
    plt.ylabel('Number')
    plt.xscale("log")
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


def histogram_am_log(data, title, color):
    fig = plt.figure(figsize=(6, 4), dpi=300)
    am = np.log10(data["A/M [m^2/kg]"])
    # weights = np.ones_like(am) / float(len(am))
    plt.hist(am, bins=np.linspace(-3, 2, 25), color=color)
    plt.xlabel('$\log(A/M) [m^2/kg]$')
    plt.ylabel('Number')
    plt.title(title + " $A/M$ Distribution")
    plt.savefig("histogram_am_log_" + title, dpi=300)
    plt.close(fig)


def scatter_lc_am(data, title, color):
    fig = plt.figure(figsize=(6, 4), dpi=300)
    data.plot(x="Characteristic Length [m]", y="A/M [m^2/kg]", kind="scatter",
              legend=True, loglog=True, alpha=0.2, color=color, marker='.')
    plt.title(title)
    plt.xlabel('$L_c [m]$')
    plt.ylabel('$A/M [m^2/kg]$')
    plt.xlim(0.01, 10)
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


def scatter_dv_am_log(data, title, color):
    fig = plt.figure(figsize=(6, 4), dpi=300)
    velocity = np.array([np.fromstring(ele[1:-1], sep=' ') for ele in data["Ejection Velocity [m/s]"].to_numpy()])
    scalar = np.linalg.norm(velocity, axis=1)

    plt.scatter(x=scalar, y=data["A/M [m^2/kg]"], alpha=0.2, color=color, marker='.')
    plt.title(title)
    plt.xlabel('$\Delta V [m/s]$')
    plt.ylabel('$A/M [m^2/kg]$')
    plt.yscale("log")
    plt.xscale("log")
    plt.ylim(0.001, 100)
    plt.savefig("scatter_dv_am_log_" + title, dpi=300)
    plt.close(fig)


def histogram_lc_cum(data, fen, title):
    number = 3000
    fig = plt.figure(figsize=(6, 4), dpi=300)
    data1 = data[data["Name"] == "Kosmos 2251-Collision-Fragment"]["Characteristic Length [m]"]
    data2 = data[data["Name"] == "Iridium 33-Collision-Fragment"]["Characteristic Length [m]"]

    x = np.logspace(-2.0, 1.0, num=number)
    y = np.zeros(number)
    for i, point in enumerate(x):
        for mass in data1:
            if mass >= point:
                y[i] += 1
    plt.plot(x, y, c='m', label="Cosmos 2251")

    x = np.logspace(-2.0, 1.0, num=number)
    y = np.zeros(number)
    for i, point in enumerate(x):
        for mass in data2:
            if mass >= point:
                y[i] += 1
    plt.plot(x, y, c='k', label="Iridium 33")

    data3 = fen["Characteristic Length [m]"]
    x = np.logspace(-2.0, 1.0, num=number)
    y = np.zeros(number)
    for i, point in enumerate(x):
        for mass in data3:
            if mass >= point:
                y[i] += 1
    plt.plot(x, y, c='r', label="Fengyun 1C")

    plt.legend(loc='upper right')

    plt.xlabel('$L_c [m]$')
    plt.ylabel("Cumulative number")

    plt.xlim(0.01, 10)

    plt.xscale("log")
    plt.yscale("log")
    plt.grid(True)

    plt.title("Size Distributions of Modelled Breakups")
    plt.savefig("histogram_lc_cum_" + title, dpi=300)
    plt.close(fig)


def main():
    file_name = sys.argv[1]
    plot_name = sys.argv[2]
    file_name2 = sys.argv[3]
    file_name3 = sys.argv[4]

    df = pd.read_csv(file_name)
    fenggyun = pd.read_csv(file_name2)
    nimbus = pd.read_csv(file_name3)
    # df = df[df["Name"] == "Iridium 33-Collision-Fragment"]
    # df = df[df["Name"] == "Kosmos 2251-Collision-Fragment"]
    # df = df[df["Characteristic Length [m]"] != 0.0]
    # df = df[df["A/M [m^2/kg]"] > 0.0]

    # histogram_am_log(fenggyun, "Fenggyun-1C", 'r')

    # histogram_lc_cum(df, fenggyun, plot_name)
    # histogram_am(df, plot_name)
    # histogram_am_log(df, plot_name)

    # scatter_lc_am(df, plot_name, 'b')
    # scatter_lc_am(df[df["Name"] == "Iridium 33-Collision-Fragment"], "Iridium-33 Fragments", 'k')
    # scatter_lc_am(df[df["Name"] == "Kosmos 2251-Collision-Fragment"], "Cosmos-2251 Fragments", 'm')
    # scatter_lc_am(fenggyun, "Fengyun-1C Fragments", 'r')


    scatter_dv_am_log(nimbus, "Nimbus 6 R/B Explosion", 'b')
    histogram_am_log(nimbus, "Nimbus 6 R/B Explosion", 'b')
    scatter_lc_am(nimbus, "Nimbus 6 R/B Explosion", 'b')
    # scatter_dv_am_log(df, plot_name, 'b')


if __name__ == '__main__':
    main()
