import numpy as np
import scipy as sp
import matplotlib as mpl
import matplotlib.pyplot as plt

# Contains the L_c measure points in [m]
lc_data_point = np.array([1.0, 0.1, 0.05, 0.01, 0.005, 0.001, 0.0005, 0.0002])
# Contains the fixed number of fragments for some L_c value in [m]
fragment_count_fixed = {
    1.0:    23,
    0.1:    1208,
    0.05:   3954,
    0.01:   61997,
    0.005:  202830,
    0.001:  3179589,
    0.0005: 10402370,
    0.0002: 49843638
}
# Contains the Time Measurements for AoS approach
time_measurements_AoS = {
    1.0: np.array([]),
    0.1: np.array([]),
    0.05: np.array([]),
    0.01: np.array([]),
    0.005: np.array([]),
    0.001: np.array([]),
    0.0005: np.array([]),
    0.0002: np.array([])
}
# Contains the Time Measurements for SoA approach
time_measurements_SoA = {
    1.0: np.array([]),
    0.1: np.array([]),
    0.05: np.array([]),
    0.01: np.array([]),
    0.005: np.array([]),
    0.001: np.array([]),
    0.0005: np.array([]),
    0.0002: np.array([])
}
# Contains the Time Measurements for SoA parallel approach
time_measurements_SoA_par = {
    1.0: np.array([4.0, 6.0, 5.227]),
    0.1: np.array([6.215, 7.171]),
    0.05: np.array([7.284, 7.411]),
    0.01: np.array([13.816, 12.256, 14.463]),
    0.005: np.array([26.19, 22.99, 24.91]),
    0.001: np.array([235.0, 262.0, 247.0, 241.0, 235.0, 239.0]),
    0.0005: np.array([736.0, 720.0, 1036.0, 720]),
    0.0002: np.array([3949.0, 3935.0, 3957.0])
}


def average_time(time_measurements):
    return [np.average(time_measurements[key]) for key in lc_data_point]


def main():
    print("Staring to plot...")
    print("Staring to plot Milestones Time Measurements")
    fig = plt.figure(figsize=(6, 4), dpi=300)

    # aos = average_time(time_measurements_AoS)
    # soa = average_time(time_measurements_SoA)
    soa_par = average_time(time_measurements_SoA_par)

    # plt.plot(aos, fragment_count_fixed.items(), color="b")
    # plt.plot(soa, fragment_count_fixed.items(), color="m")
    plt.plot(fragment_count_fixed.values(), soa_par, color="r")


    plt.xscale("log")
    plt.yscale("log")
    plt.grid(True)

    plt.xlabel("Fragment Count")
    plt.ylabel("Time in [ms]")

    plt.savefig("timeMeasurements_milestones", dpi=300)
    plt.close(fig)
    print("Finished to plot Milestones Time Measurements")


if __name__ == '__main__':
    main()
