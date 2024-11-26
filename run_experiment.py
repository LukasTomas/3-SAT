import os
import tqdm
import time
import subprocess
import numpy as np
import pandas as pd

import matplotlib
import matplotlib.pyplot as plt
matplotlib.use('Agg')

PROGRAMS_DIR = os.path.join('.', 'algorithms')
GSAT_EXEC = os.path.join(PROGRAMS_DIR, 'gsat2')
PROBSAT_EXEC = os.path.join(PROGRAMS_DIR, 'probsat')

DATA_DIR = os.path.join('.', 'data')
OPTIONS = ['20-91', '36-157', '50-218', '75-320']

RESULTS_DIR = os.path.join('.', 'results')

NUMBER_OF_SEEDS = 400
MAX_CNF_FILES = -1      # -1 for all files


def run_command(program_exec, cnf_file, seed):
    if program_exec == GSAT_EXEC:
        output = subprocess.run([program_exec, '-i', '100000000', '-r', str(seed), os.path.join(data_path, cnf_file)], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        output = output.stderr
        result = output.splitlines()[0]
        # returns 4 integers separated by space
        result_ints = list(map(int, result.split()))
        if len(result_ints) != 4:
            raise Exception(f'Invalid result for {cnf_file}: {result}')
        
        iters_took, max_iters, satisfied_clauses, total_clauses = result_ints
        satisfied = satisfied_clauses == total_clauses
        return iters_took, satisfied
    else:
        output = subprocess.run([program_exec, os.path.join(data_path, cnf_file), str(seed)], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        output = output.stdout
        result = int(output.splitlines()[0])
        satisfied = result != -1;
        return result, satisfied


def run_experiment(program_exec, data_path, seeds):
    """
    Run the experiment for a given program and dataset
    """

    satisfied_iters = []

    cnf_files = os.listdir(data_path)
    if MAX_CNF_FILES > 0:
        cnf_files = cnf_files[:MAX_CNF_FILES]
    for cnf_file in tqdm.tqdm(cnf_files, desc=f'{data_path}'):

        iters = []
        for seed in seeds:
            iters_took, satisfied = run_command(program_exec, cnf_file, seed)
            # cnf_file not satisfied if satisfied_clauses != total_clauses
            if not satisfied:
                raise Exception(f'{cnf_file} not satisfied')
            iters.append(iters_took)

        avg_iters = sum(iters) / len(iters)
        # print(f'{cnf_file} average iterations: {avg_iters}')
        satisfied_iters.append(avg_iters)

    return satisfied_iters 


if __name__ == '__main__':
    if not os.path.exists(RESULTS_DIR):
        os.makedirs(RESULTS_DIR)

    probsat_exec = PROBSAT_EXEC
    if os.path.islink(PROBSAT_EXEC):
        probsat_exec = os.readlink(PROBSAT_EXEC)

    PROGRAMS = [GSAT_EXEC, probsat_exec]
    # PROGRAMS = [probsat_exec]
    for program in PROGRAMS:
        if not os.path.exists(program):
            raise Exception(f'{program} executable not found. Please compile the code first.')

    if not os.path.exists(DATA_DIR):
        raise Exception(f'Data {DATA_DIR} directory not found')
    for option_str in OPTIONS:
        data_path = os.path.join(DATA_DIR, option_str)
        if not os.path.exists(data_path):
            raise Exception(f'Data {data_path} directory not found')

    # set seed for seed generation
    root_seed = 0
    np.random.seed(root_seed)
    seeds = np.random.randint(0, 2**32, NUMBER_OF_SEEDS)

    for program_exec in PROGRAMS:
        df = pd.DataFrame(columns=OPTIONS)
        for dataset_dir in OPTIONS:
            data_path = os.path.join(DATA_DIR, dataset_dir)
            print(f'Running {program_exec} on {data_path}')
            iterations = run_experiment(program_exec, data_path, seeds)
            df[dataset_dir] = iterations
        
        program_name = os.path.basename(program_exec)
        output_path = os.path.join(RESULTS_DIR, f'{program_name}_results.csv')
        print(f'Saving results to {output_path}_results.csv')
        df.to_csv(output_path, index=False)            


    #     print(option_str)
    #     average_iters = sum(satisfied_iters) / len(satisfied_iters)
    #     print(f'Files satisfied: {files_satisfied}/{total_files} = {files_satisfied/total_files:.2f}')
    #     df = pd.DataFrame(satisfied_iters, columns=['iterations'])
    #     print(df.describe())


    #     plt.hist(satisfied_iters, bins=10, color='blue', edgecolor='black')
    #     plt.xlabel('Value Range')
    #     plt.ylabel('Frequency')
    #     plt.title('Histogram of Satisfied Iterations')
    #     plt.savefig('satisfied_iters_histogram.png')
    #     plt.close()

    #     ecdf = np.arange(1, len(satisfied_iters) + 1) / len(satisfied_iters)
    #     sorted_iters = np.sort(satisfied_iters)

    #     plt.step(sorted_iters, ecdf, where='post', color='blue')
    #     plt.xlabel('Iterations')
    #     plt.ylabel('ECDF')
    #     plt.title('Empirical Cumulative Distribution Function of Satisfied Iterations')
    #     plt.savefig('satisfied_iters_ecdf.png')
    #     plt.close()

    # avg_all_iters = sum(all_satisfied_iters) / len(all_satisfied_iters)
    # print('Total average iterations:', avg_all_iters)
    # df = pd.DataFrame(all_satisfied_iters, columns=['iterations'])
    # print(df.describe())