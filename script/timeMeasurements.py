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

# GCC 9.3 Results

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
# Contains the Time Measurements for SoA parallel approach in micro-seconds with mass conservation
gcc_time_measurements_SoA_par_mass_max = np.array([10068, 610, 1306, 14750, 33308, 519697, 1893020, 16935800])
gcc_time_measurements_SoA_par_mass_avg = np.array([1123.9, 475.8, 980.1, 9065.3, 22052.2, 432043, 1603010, 10521500])
gcc_time_measurements_SoA_par_mass_min = np.array([35, 311, 721, 4258, 11575, 285780, 1164450, 7773010])
# Contains the Fragment Numbers of the above measurements for mass conservation
gcc_fragments_SoA_par_mass_max = np.array([74, 2137, 5685, 87819, 269742, 4122479, 13098840, 59814205])
gcc_fragments_SoA_par_mass_avg = np.array([61, 1715, 4962, 73302, 222871, 3746307, 11872340, 55176421])
gcc_fragments_SoA_par_mass_min = np.array([43, 1273, 4179, 53274, 161802, 3230728, 10422884, 51466684])

# Clang 10.0 Results

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
# Contains the Time Measurements for SoA parallel approach in micro-seconds with mass conservation
clang_time_measurements_SoA_par_mass_max = np.array([2600, 842, 1792, 23560, 51751, 889739, 2770860, 19887600])
clang_time_measurements_SoA_par_mass_avg = np.array([443, 703.7, 1360.2, 13008.3, 35205.5, 690059, 1989040, 12004400])
clang_time_measurements_SoA_par_mass_min = np.array([75, 607, 743, 6724, 18143, 339621, 1243910, 5226740])
# Contains the Fragment Numbers of the above measurements for mass conservation
clang_fragments_SoA_par_mass_max = np.array([69, 1888, 5878, 91100, 262320, 4338042, 13218109, 66267545])
clang_fragments_SoA_par_mass_avg = np.array([61, 1678, 5115, 72708, 227708, 3850426, 11295145, 57038138])
clang_fragments_SoA_par_mass_min = np.array([44, 1563, 4061, 60693, 177825, 2976784, 8866479, 41525083])

# Utilized cores (0 - 16)
utilized_cores = np.array([
    0.057391663100000004,
    0.29719989589999996,
    0.1558400977,
    0.09634660310000001,
    0.0012367221999999998,
    0.0029524258999999997,
    0.0021224513,
    0.0009660218999999999,
    0.0101923808,
    0.0008027215,
    0.000795917,
    0.0010135777,
    0.0102957217,
    0.0000069341,
    0.000012538900000000001,
    0.11984812660000001,
    0.1301281496
])


def time_measurement_gcc():
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
        plt.text(x=x - x / 10 * 4, y=10e7, s=str(lc), fontdict={"size": 7})

    plt.xscale("log")
    plt.yscale("log")
    plt.grid(True)
    plt.xlim(10, 10e7)
    plt.ylim(1, 10e8)

    plt.xlabel("Fragment Count")
    plt.ylabel("Time in [$\mu$s]")
    plt.title("Time Measurements GCC 9.3 - 10 runs each (min, avg, max)")

    plt.legend(loc="lower right")

    plt.savefig("timeMeasurements_milestones_gcc", dpi=300)
    plt.close(fig)


def time_measurement_clang():
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
                     clang_time_measurements_SoA_par_min, clang_time_measurements_SoA_par_max, color='magenta',
                     alpha=0.3)

    plt.vlines(x_values, 0, 10e7, colors="black", linestyles="dashed", label="$L_c$ in [m]")
    for lc, x in fragment_count_fixed.items():
        plt.text(x=x - x / 10 * 4, y=10e7, s=str(lc), fontdict={"size": 7})

    plt.xscale("log")
    plt.yscale("log")
    plt.grid(True)
    plt.xlim(10, 10e7)
    plt.ylim(1, 10e8)

    plt.xlabel("Fragment Count")
    plt.ylabel("Time in [$\mu$s]")
    plt.title("Time Measurements Clang 10.0 - 10 runs each (min, avg, max)")

    plt.legend(loc="lower right")

    plt.savefig("timeMeasurements_milestones_clang", dpi=300)
    plt.close(fig)


def time_measurement_relative_gcc():
    fig = plt.figure(figsize=(6, 4), dpi=300)

    x_values = lc_data_point
    y_norm = gcc_time_measurements_SoA_par_avg

    plt.plot(x_values, gcc_time_measurements_AoS_avg / y_norm, color="orange", label="Aos")
    plt.fill_between(x_values,
                     gcc_time_measurements_AoS_min / y_norm, gcc_time_measurements_AoS_max / y_norm, color="orange",
                     alpha=0.3)

    plt.plot(x_values, gcc_time_measurements_SoA_avg / y_norm, color="blue", label="SoA")
    plt.fill_between(x_values,
                     gcc_time_measurements_SoA_min / y_norm, gcc_time_measurements_SoA_max / y_norm, color="blue",
                     alpha=0.3)

    plt.plot(x_values, y_norm / y_norm, color="magenta", label="SoA parallel")
    plt.fill_between(x_values,
                     gcc_time_measurements_SoA_par_min / y_norm, gcc_time_measurements_SoA_par_max / y_norm,
                     color='magenta',
                     alpha=0.3)
    plt.vlines(x_values, 0, 11, colors="black", linestyles="dashed", label="$L_c$ in [m]")
    for lc, x in fragment_count_fixed.items():
        plt.text(x=lc, y=11, s=str(lc), fontdict={"size": 7})

    plt.grid(True)
    plt.xscale("log")
    plt.xlim(1.0, 0.0001)
    plt.ylim(0.0, 12.0)

    plt.xlabel("$L_c$ in [m]")
    plt.ylabel("Relative Time")
    plt.title("Relative Time GCC 9.3 - 10 runs each (min, avg, max)")

    plt.legend(loc="upper left")

    plt.savefig("timeMeasurements_relative_milestones_gcc", dpi=300)
    plt.close(fig)


def time_measurement_relative_clang():
    fig = plt.figure(figsize=(6, 4), dpi=300)

    x_values = lc_data_point
    y_norm = clang_time_measurements_SoA_par_avg

    plt.plot(x_values, clang_time_measurements_AoS_avg / y_norm, color="orange", label="Aos")
    plt.fill_between(x_values,
                     clang_time_measurements_AoS_min / y_norm, clang_time_measurements_AoS_max / y_norm, color="orange",
                     alpha=0.3)

    plt.plot(x_values, clang_time_measurements_SoA_avg / y_norm, color="blue", label="SoA")
    plt.fill_between(x_values,
                     clang_time_measurements_SoA_min / y_norm, clang_time_measurements_SoA_max / y_norm, color="blue",
                     alpha=0.3)

    plt.plot(x_values, y_norm / y_norm, color="magenta", label="SoA parallel")
    plt.fill_between(x_values,
                     clang_time_measurements_SoA_par_min / y_norm, clang_time_measurements_SoA_par_max / y_norm,
                     color='magenta',
                     alpha=0.3)
    plt.vlines(x_values, 0, 11, colors="black", linestyles="dashed", label="$L_c$ in [m]")
    for lc, x in fragment_count_fixed.items():
        plt.text(x=lc, y=11, s=str(lc), fontdict={"size": 7})

    plt.grid(True)
    plt.xscale("log")
    plt.xlim(1.0, 0.0001)
    plt.ylim(0.0, 12.0)

    plt.xlabel("$L_c$ in [m]")
    plt.ylabel("Relative Time")
    plt.title("Relative Time Clang 10.0 - 10 runs each (min, avg, max)")

    plt.legend(loc="upper left")

    plt.savefig("timeMeasurements_relative_milestones_clang", dpi=300)
    plt.close(fig)


def time_measurement_mass_gcc():
    fig = plt.figure(figsize=(6, 4), dpi=300)

    x_min = gcc_fragments_SoA_par_mass_min
    x_values = gcc_fragments_SoA_par_mass_avg
    x_max = gcc_fragments_SoA_par_mass_max

    plt.plot(x_values, gcc_time_measurements_SoA_par_mass_avg, color="red", label="Enabled")
    plt.fill_between(x_values,
                     gcc_time_measurements_SoA_par_mass_min, gcc_time_measurements_SoA_par_mass_max, color="red",
                     alpha=0.3)

    plt.plot(x_values, gcc_time_measurements_SoA_par_avg, color="blue", label="Disabled")
    plt.fill_between(x_values,
                     gcc_time_measurements_SoA_par_min, gcc_time_measurements_SoA_par_max, color="blue", alpha=0.3)

    plt.vlines(fragment_count_fixed.values(), 0, 10e7, colors="black", linestyles="dashed", label="$L_c$ in [m]")
    for lc, x in fragment_count_fixed.items():
        plt.text(x=x - x / 10 * 4, y=10e7, s=str(lc), fontdict={"size": 7})

    plt.hlines(gcc_time_measurements_SoA_par_mass_avg, x_min, x_max, colors="black", linewidths=5.0,
               label="Fragment number variation")

    plt.xscale("log")
    plt.yscale("log")
    plt.grid(True)
    plt.xlim(10, 10e7)
    plt.ylim(1, 10e8)

    plt.xlabel("Fragment Count")
    plt.ylabel("Time in [$\mu$s]")
    plt.title("Time Measurements GCC 9.3 - Mass Conservation")

    plt.legend(loc="lower right")

    plt.savefig("timeMeasurements_mass_con_gcc", dpi=300)
    plt.close(fig)


def time_measurement_mass_relative_gcc():
    fig = plt.figure(figsize=(6, 4), dpi=300)

    x_values = lc_data_point

    y_norm = gcc_time_measurements_SoA_par_avg

    plt.plot(x_values, gcc_time_measurements_SoA_par_mass_avg / y_norm, color="red", label="Enabled")
    plt.fill_between(x_values,
                     gcc_time_measurements_SoA_par_mass_min / y_norm, gcc_time_measurements_SoA_par_mass_max / y_norm,
                     color="red",
                     alpha=0.3)

    plt.plot(x_values, y_norm / y_norm, color="blue", label="Disabled")
    plt.fill_between(x_values,
                     gcc_time_measurements_SoA_par_min / y_norm, gcc_time_measurements_SoA_par_max / y_norm,
                     color="blue", alpha=0.3)
    plt.vlines(x_values, 0, 11, colors="black", linestyles="dashed", label="$L_c$ in [m]")
    for lc, x in fragment_count_fixed.items():
        plt.text(x=lc, y=11, s=str(lc), fontdict={"size": 7})

    plt.grid(True)
    plt.xscale("log")
    plt.xlim(1.0, 0.0001)
    plt.ylim(0.0, 12.0)

    plt.xlabel("$L_c$ in [m]")
    plt.ylabel("Relative Time")

    plt.title("Relative Time GCC 9.3 - Mass Conservation")

    plt.legend(loc="upper right")

    plt.savefig("timeMeasurements_mass_con_relative_gcc", dpi=300)
    plt.close(fig)


def time_measurement_mass_relative_clang():
    fig = plt.figure(figsize=(6, 4), dpi=300)

    x_values = lc_data_point

    y_norm = clang_time_measurements_SoA_par_avg

    plt.plot(x_values, clang_time_measurements_SoA_par_mass_avg / y_norm, color="red", label="Enabled")
    plt.fill_between(x_values,
                     clang_time_measurements_SoA_par_mass_min / y_norm,
                     clang_time_measurements_SoA_par_mass_max / y_norm,
                     color="red",
                     alpha=0.3)

    plt.plot(x_values, y_norm / y_norm, color="blue", label="Disabled")
    plt.fill_between(x_values,
                     clang_time_measurements_SoA_par_min / y_norm, clang_time_measurements_SoA_par_max / y_norm,
                     color="blue", alpha=0.3)
    plt.vlines(x_values, 0, 11, colors="black", linestyles="dashed", label="$L_c$ in [m]")
    for lc, x in fragment_count_fixed.items():
        plt.text(x=lc, y=11, s=str(lc), fontdict={"size": 7})

    plt.grid(True)
    plt.xscale("log")
    plt.xlim(1.0, 0.0001)
    plt.ylim(0.0, 12.0)

    plt.xlabel("$L_c$ in [m]")
    plt.ylabel("Relative Time")

    plt.title("Relative Time Clang 10.0 - Mass Conservation")

    plt.legend(loc="upper right")

    plt.savefig("timeMeasurements_mass_con_relative_clang", dpi=300)
    plt.close(fig)


def time_measurement_mass_clang():
    fig = plt.figure(figsize=(6, 4), dpi=300)

    x_min = clang_fragments_SoA_par_mass_min
    x_values = clang_fragments_SoA_par_mass_avg
    x_max = clang_fragments_SoA_par_mass_max

    plt.plot(x_values, clang_time_measurements_SoA_par_mass_avg, color="red", label="Enabled")
    plt.fill_between(x_values,
                     clang_time_measurements_SoA_par_mass_min, clang_time_measurements_SoA_par_mass_max, color="red",
                     alpha=0.3)

    plt.plot(x_values, clang_time_measurements_SoA_par_avg, color="blue", label="Disabled")
    plt.fill_between(x_values,
                     clang_time_measurements_SoA_par_min, clang_time_measurements_SoA_par_max, color="blue", alpha=0.3)

    plt.vlines(fragment_count_fixed.values(), 0, 10e7, colors="black", linestyles="dashed", label="$L_c$ in [m]")
    for lc, x in fragment_count_fixed.items():
        plt.text(x=x - x / 10 * 4, y=10e7, s=str(lc), fontdict={"size": 7})

    plt.hlines(clang_time_measurements_SoA_par_mass_avg, x_min, x_max, colors="black", linewidths=5.0,
               label="Fragment number variation")

    plt.xscale("log")
    plt.yscale("log")
    plt.grid(True)
    plt.xlim(10, 10e7)
    plt.ylim(1, 10e8)

    plt.xlabel("Fragment Count")
    plt.ylabel("Time in [$\mu$s]")
    plt.title("Time Measurements Clang 10.0 - Mass Conservation")

    plt.legend(loc="lower right")

    plt.savefig("timeMeasurements_mass_con_clang", dpi=300)
    plt.close(fig)


def mass_range_gcc():
    fig = plt.figure(figsize=(6, 4), dpi=300)

    x_values = lc_data_point
    plt.plot(x_values, gcc_fragments_SoA_par_mass_avg, color="red")
    plt.fill_between(x_values,
                     gcc_fragments_SoA_par_mass_min, gcc_fragments_SoA_par_mass_max, color="red",
                     alpha=0.3)

    plt.vlines(x_values, 0, 10e7, colors="black", linestyles="dashed", label="$L_c$ in [m]")

    plt.grid(True)

    plt.xlim(1.0, 0.0001)
    plt.ylim(0.1, 10e7)

    plt.xscale("log")
    plt.yscale("log")

    plt.xlabel("$L_c$ in [m]")
    plt.ylabel("Fragment Number")
    plt.title("Mass Conservation GCC 9.3 - Fragment Number Variation")

    plt.savefig("mass_con_range_gcc", dpi=300)
    plt.close(fig)


def mass_range_clang():
    fig = plt.figure(figsize=(6, 4), dpi=300)

    x_values = lc_data_point
    plt.plot(x_values, clang_fragments_SoA_par_mass_avg, color="red")
    plt.fill_between(x_values,
                     clang_fragments_SoA_par_mass_min, clang_fragments_SoA_par_mass_max, color="red",
                     alpha=0.3)

    plt.vlines(x_values, 0, 10e7, colors="black", linestyles="dashed", label="$L_c$ in [m]")

    plt.grid(True)

    plt.xlim(2.0, 0.0001)
    plt.ylim(0.1, 10e7)

    plt.xscale("log")
    plt.yscale("log")

    plt.xlabel("$L_c$ in [m]")
    plt.ylabel("Fragment Number")
    plt.title("Mass Conservation Clang 10.0 - Fragment Number Variation")

    plt.savefig("mass_con_range_clang", dpi=300)
    plt.close(fig)


def core_utilization_plot():
    fig = plt.figure(figsize=(8, 4), dpi=300)
    numbers = np.arange(0, 17)

    utilized_cores_ms = utilized_cores * 1000

    bars1 = plt.bar(numbers, utilized_cores_ms)

    for i, b in enumerate(bars1):
        if i < 12:
            b.set_color("red")
        elif i < 16:
            b.set_color("orange")
        else:
            b.set_color("green")

    plt.vlines(5.636, 0, 300, colors="black", linestyles="dashed", label="Average utilization")
    plt.grid(True)
    plt.xlim(-1, 17)

    plt.xlabel("Utilized Cores")
    plt.ylabel("Time in [ms]")
    plt.title("Core Utilization for $L_c = 0.0005$ as analyzed by VTune")

    plt.legend(loc="upper right")

    plt.savefig("vtune_speed_0005", dpi=300)
    plt.close(fig)


def lc_number():
    fig = plt.figure(figsize=(6, 4), dpi=300)

    mass = 556.0 + 900.0
    lc_values = np.linspace(1.0, 0.0001)
    fragment_numbers = 0.1 * (mass ** 0.75) * (lc_values ** -1.71)

    plt.plot(lc_values, fragment_numbers, label="Iridium-33 Cosmos-2251 Collision", color="red")

    lines = plt.vlines(lc_data_point, 0, 10e7, colors="black", linestyles="dashed", label="Time Measurements")

    plt.grid(True)
    plt.xlim(1.0, 0.0001)

    plt.xscale("log")
    plt.yscale("log")

    plt.xlabel("$L_{c, min}$ in [m]")
    plt.ylabel("Fragment Number")
    plt.title("Number of Fragments depending on $L_{c, min}$")
    plt.legend(loc="upper left")

    plt.savefig("lc_number", dpi=300)
    plt.close(fig)


def main():
    print("Staring to plot...")

    # ime Measurements
    time_measurement_gcc()
    time_measurement_clang()

    time_measurement_relative_gcc()
    time_measurement_relative_clang()

    # Mass conservation Time
    time_measurement_mass_gcc()
    time_measurement_mass_clang()

    time_measurement_mass_relative_gcc()
    time_measurement_mass_relative_clang()

    lc_number()

    print("Finished Plotting")


if __name__ == '__main__':
    main()
