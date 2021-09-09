import numpy as np
import scipy as sp
import matplotlib as mpl
import matplotlib.pyplot as plt

# Contains the L_c measure points in [m]
lc_data_point = np.array([1.0, 0.1, 0.05, 0.01, 0.005, 0.001, 0.0005, 0.0002])
# Contains the fixed number of fragments for some L_c value in [m]
fragment_count_fixed = {
    1.0: 23,
    0.1: 1208,
    0.05: 3954,
    0.01: 61997,
    0.005: 202830,
    0.001: 3179589,
    0.0005: 10402370,
    0.0002: 49843638
}
# Contains the Time Measurements for AoS approach in micro-seconds
gcc_time_measurements_AoS_max = np.array([19, 520, 1595, 23352, 70220, 1141540, 3693200, 32444700])
gcc_time_measurements_AoS_avg = np.array([10.4, 474, 1466.5, 20910.3, 67915.7, 1049560, 3590180, 19880100])
gcc_time_measurements_AoS_min = np.array([8, 460, 1393, 19814, 66858, 940707, 3536950, 17495400])
# Contains the Time Measurements for SoA approach in micro-seconds
gcc_time_measurements_SoA_max = np.array([3719, 536, 1691, 23251, 70520, 1076500, 3430080, 17343600])
gcc_time_measurements_SoA_avg = np.array([379.8, 500.7, 1595.9, 21762.1, 67148.3, 1047360, 3386900, 17107600])
gcc_time_measurements_SoA_min = np.array([7, 491, 1547, 21248, 65524, 1023900, 3342570, 16137500])
# Contains the Time Measurements for SoA parallel approach in micro-seconds
gcc_time_measurements_SoA_par_max = np.array([7824, 296, 464, 5853, 14321, 214575, 825130, 4833780])
gcc_time_measurements_SoA_par_avg = np.array([857.1, 138.7, 440.3, 4632.3, 11869.2, 173976, 737182, 4313500])
gcc_time_measurements_SoA_par_min = np.array([18, 112, 410, 3939, 10790, 158734, 637936, 3869120])

# Contains the Time Measurements for AoS approach in micro-seconds
clang_time_measurements_AoS_max = np.array([138, 1103, 3478, 47368, 144885, 2250630, 7349420, 36137100])
clang_time_measurements_AoS_avg = np.array([30.6, 1069.3, 3209.7, 42510.9, 139220, 2154490, 7298970, 35283600])
clang_time_measurements_AoS_min = np.array([18, 1052, 3129, 33487, 123697, 1965500, 7268280, 33502500])
# Contains the Time Measurements for SoA approach in micro-seconds
clang_time_measurements_SoA_max = np.array([129, 1153, 3418, 44742, 139831, 2196900, 7173140, 42380000])
clang_time_measurements_SoA_avg = np.array([31.3, 1093.2, 3290.4, 42979.8, 136874, 2144810, 7103430, 35222900])
clang_time_measurements_SoA_min = np.array([19, 1075, 3240, 42021, 126172, 2007190, 6910200, 33135000])
# Contains the Time Measurements for SoA parallel approach in micro-seconds
clang_time_measurements_SoA_par_max = np.array([2540, 320, 621, 8293, 19804, 306578, 1215060, 6938640])
clang_time_measurements_SoA_par_avg = np.array([313.2, 189.9, 589.9, 7049.5, 17115.9, 263603, 1109550, 5965700])
clang_time_measurements_SoA_par_min = np.array([15, 165, 560, 5924, 14643, 255949, 966486, 5054580])


def main():
    print("Staring to plot...")
    print("Staring to plot Milestones Time Measurements for GCC 9.3")
    fig = plt.figure(figsize=(6, 4), dpi=300)

    x_values = fragment_count_fixed.values()

    plt.plot(x_values, gcc_time_measurements_AoS_avg, color="orange", label="Aos")
    plt.fill_between(x_values,
                     gcc_time_measurements_AoS_min, gcc_time_measurements_AoS_max, color="orange", alpha=0.3)

    plt.plot(x_values, gcc_time_measurements_SoA_avg, color="blue", label="SoA")
    plt.fill_between(x_values,
                     gcc_time_measurements_SoA_min, gcc_time_measurements_SoA_max, color="blue", alpha=0.3)

    plt.plot(x_values, gcc_time_measurements_SoA_par_avg, color="magenta", label="SoA parallel")
    plt.fill_between(x_values,
                     gcc_time_measurements_SoA_par_min, gcc_time_measurements_SoA_par_max, color='magenta', alpha=0.3)

    plt.vlines(x_values, 0, 10e7, colors="black", linestyles="dashed", label="$L_c$ in [m]")
    for lc, x in fragment_count_fixed.items():
        plt.text(x=x-x/10*4, y=10e7, s=str(lc), fontdict={"size": 7})

    plt.xscale("log")
    plt.yscale("log")
    plt.grid(True)

    plt.xlabel("Fragment Count")
    plt.ylabel("Time in [$\mu$s]")
    plt.title("Time Measurements GCC 9.3 - 10 runs each (min, avg, max)")

    plt.legend(loc="lower right")

    plt.savefig("timeMeasurements_milestones_gcc", dpi=300)
    plt.close(fig)
    print("Finished to plot Milestones Time Measurements")

    print("Staring to plot Milestones Time Measurements for Clang 10.0")
    fig = plt.figure(figsize=(6, 4), dpi=300)

    x_values = fragment_count_fixed.values()

    plt.plot(x_values, clang_time_measurements_AoS_avg, color="orange", label="Aos")
    plt.fill_between(x_values,
                     clang_time_measurements_AoS_min, clang_time_measurements_AoS_max, color="orange", alpha=0.3)

    plt.plot(x_values, clang_time_measurements_SoA_avg, color="blue", label="SoA")
    plt.fill_between(x_values,
                     clang_time_measurements_SoA_min, clang_time_measurements_SoA_max, color="blue", alpha=0.3)

    plt.plot(x_values, clang_time_measurements_SoA_par_avg, color="magenta", label="SoA parallel")
    plt.fill_between(x_values,
                     clang_time_measurements_SoA_par_min, clang_time_measurements_SoA_par_max, color='magenta', alpha=0.3)

    plt.vlines(x_values, 0, 10e7, colors="black", linestyles="dashed", label="$L_c$ in [m]")
    for lc, x in fragment_count_fixed.items():
        plt.text(x=x-x/10*4, y=10e7, s=str(lc), fontdict={"size": 7})

    plt.xscale("log")
    plt.yscale("log")
    plt.grid(True)

    plt.xlabel("Fragment Count")
    plt.ylabel("Time in [$\mu$s]")
    plt.title("Time Measurements Clang 10.0 - 10 runs each (min, avg, max)")

    plt.legend(loc="lower right")

    plt.savefig("timeMeasurements_milestones_clang", dpi=300)
    plt.close(fig)
    print("Finished to plot Milestones Time Measurements")


if __name__ == '__main__':
    main()
