#!/usr/bin/env python
import argparse
import matplotlib.pyplot as plt
import json
import numpy as np

def plot_results(prefix, N, YLIM):
    # Get the benchmark results for each hash functions.
    std_hash_results = np.zeros(N, dtype=float)
    clhash_results = np.zeros(N, dtype=float)
    boost_hash_results = np.zeros(N, dtype=float)
    xxhash_results = np.zeros(N, dtype=float)
    farmhash_results = np.zeros(N, dtype=float)
    hybrid_hash_results = np.zeros(N, dtype=float)
    for idx in range(0, N):
        data_file = prefix + str(idx + 1) + ".json"
        # print("data_file: ", data_file)
        data = json.load(open(data_file))
        benchmark_results = data["benchmarks"]
        # print(benchmark_results)

        for info in benchmark_results:
            benchmark_name = info["name"]
            if (benchmark_name == "std_hash_string"):
                std_hash_results[idx] = info["real_time"]
            elif (benchmark_name == "clhash_string"):
                clhash_results[idx] = info["real_time"]
            elif (benchmark_name == "boost_hash_string"):
                boost_hash_results[idx] = info["real_time"]
            elif (benchmark_name == "hybrid_hash_string"):
                hybrid_hash_results[idx] = info["real_time"]
            elif (benchmark_name == "xxhash_string"):
                xxhash_results[idx] = info["real_time"]
            elif (benchmark_name == "farmhash_string"):
                farmhash_results[idx] = info["real_time"]
            else:
                print("How can we get here?")

    # Plot results
    x = np.arange(1, N + 1, 1)
    lines = plt.plot(
        x, std_hash_results, "r-+", x, clhash_results, "b-o", x, boost_hash_results, 'g-', x, hybrid_hash_results, 'y-^', x, xxhash_results, 'g-v', x, farmhash_results, 'm-*')
    plt.xlabel("String length (bytes)")
    plt.ylabel("Run time (ns)")
    plt.ylim(0, YLIM)
    plt.xlim(1, N)
    plt.grid()
    plt.legend(lines, ('std::hash', 'clhash', 'boost::hash', 'hybrid_hash', 'xxHash', 'farmhash'))
    plt.title("Hash functions performance analysis with random strings");
    plt.show()

# Parse input arguments
parser = argparse.ArgumentParser(description='Required input arguments.')
parser.add_argument('prefix', metavar='prefix', type=str,
                    help='Prefix of JSON data files')
parser.add_argument('maximum_length', metavar='maximum_length', type=int,
                    help='The maximum length of tested strings')
parser.add_argument('ylim', metavar='ylim', type=int,
                    help='The maximum length of tested strings')

args = parser.parse_args()
prefix = args.prefix
N = args.maximum_length
ylim = args.ylim
print("Prefix: ", prefix)
print("Maximum string length: ", N)
N = args.maximum_length

# Plot benchmark results
plot_results(prefix, N, ylim)
