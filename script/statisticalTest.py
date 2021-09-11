import numpy as np
from scipy import stats
import pandas as pd


def lc_test(data_cpp, data_python):
    lc_cpp = data_cpp["Characteristic Length [m]"]
    lc_python = data_python["Characteristic Length [m]"]
    result = stats.kstest(lc_python, lc_cpp)
    print("L_c Result:\n", result)


def am_test(data_cpp, data_python):
    am_cpp = data_cpp["A/M [m^2/kg]"]
    am_python = data_python["A/M [m^2/kg]"]
    result = stats.kstest(am_cpp, am_python)
    print("A/M Result:\n", result)


def dv_test(data_cpp, data_python):
    velocity_cpp = np.array(
        [np.fromstring(ele[1:-1], sep=' ') for ele in data_cpp["Ejection Velocity [m/s]"].to_numpy()])
    velocity_python = np.array(
        [np.fromstring(ele[1:-1], sep=' ') for ele in data_python["Ejection Velocity [m/s]"].to_numpy()])
    scalar_cpp = np.linalg.norm(velocity_cpp, axis=1)
    scalar_python = np.linalg.norm(velocity_python, axis=1)

    result = stats.kstest(scalar_cpp, scalar_python)
    print("DV Result:\n", result)


def main():
    file_name_cpp = "../cpp-results/iridium_cosmos_result_onlySC11.csv"
    file_name_python = "../python-results/iridium_cosmos_python.csv"

    df_cpp = pd.read_csv(file_name_cpp)
    df_python = pd.read_csv(file_name_python)

    print("Doing Statistical Test...")
    lc_test(df_cpp, df_python)
    am_test(df_cpp, df_python)
    dv_test(df_cpp, df_python)
    print("Finished Statistical Test")


if __name__ == '__main__':
    main()
