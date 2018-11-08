#!/usr/bin/env python
import argparse
import matplotlib.pyplot as plt
import json
import numpy as np

def plot_results(prefix, N):
    # Get the benchmark results for each hash functions.
    std_hash_results = np.zeros(N, dtype=float)
    clhash_results = np.zeros(N, dtype=float)
    boost_hash_results = np.zeros(N, dtype=float)
    for idx in range(1, N):
        data_file = prefix + str(idx) + ".json"
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
        else:
            print("How can we get here?")

            
    # Plot results
    x = np.arange(1, N + 1, 1)
    line1, line2, line3 = plt.plot(
        x, std_hash_results, "r", x, clhash_results, "b", x, boost_hash_results, "o")
    plt.xlabel("String length (bytes)")
    plt.ylabel("Run time (ns)")
    plt.ylim(0, 200)
    plt.xlim(1, N)
    plt.grid()
    plt.legend((line1, line2, line3), ('std::hash', 'clhash', 'boost::hash'))
    plt.show()

    
# Parse input arguments
parser = argparse.ArgumentParser(description='Required input arguments.')
parser.add_argument('prefix', metavar='prefix', type=str,
                    help='Prefix of JSON data files')
parser.add_argument('maximum_length', metavar='maximum_length', type=int,
                    help='The maximum length of tested strings')

args = parser.parse_args()
prefix = args.prefix
N = args.maximum_length
print("Prefix: ", prefix)
print("Maximum string length: ", N)
N = args.maximum_length

# Plot benchmark results
plot_results(prefix, N)
