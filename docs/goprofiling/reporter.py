# /// script
# dependencies = [
#   "pandas",
#   "matplotlib",
#   "seaborn",
#   "tabulate",
# ]
# ///

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import sys
import os

# Function to clean and convert duration string from microseconds (µs) to milliseconds (ms)
def convert_to_ms(duration_str):
    # Strip all whitespace
    duration_str = duration_str.strip()

    # Clean up 'µs' unit
    if 'µs' in duration_str:
        duration_str = duration_str.replace('µs', '')

    try:
        # The result is in microseconds, convert to milliseconds (1 ms = 1000 µs)
        microseconds = float(duration_str)
        return microseconds / 1000.0
    except ValueError:
        return None

def analyze_timing_data(file_path):
    # 1. Load and clean data
    data = []
    try:
        with open(file_path, 'r') as f:
            for line in f:
                line = line.strip()
                if not line:
                    continue

                if ',' in line:
                    parts = line.split(',', 1)
                    type_val = parts[0].strip()
                    duration_str = parts[1].strip()
                    
                    duration_ms = convert_to_ms(duration_str)
                    if duration_ms is not None:
                        data.append({'Type': type_val, 'Duration (ms)': duration_ms})

    except FileNotFoundError:
        print(f"Error: File not found at {file_path}")
        return
    except Exception as e:
        print(f"An error occurred while reading the file: {e}")
        return

    if not data:
        print("Error: No valid data found in the file.")
        return

    df = pd.DataFrame(data)

    # 2. Statistical Summary Table
    print("\n" + "="*40)
    print("      Statistical Summary Table")
    print("="*40)
    summary = df.groupby('Type')['Duration (ms)'].describe()
    # Print the summary in a neat Markdown table format
    print(summary.to_markdown(numalign="left", stralign="left"))
    summary.to_csv("timing_summary.csv")
    print(f"\nStatistical summary saved to timing_summary.csv")


    # 3. Box Plot Chart
    plt.figure(figsize=(8, 6))
    sns.boxplot(x='Type', y='Duration (ms)', data=df, palette='viridis')
    plt.title('Execution Time Distribution by Type')
    plt.ylabel('Duration (ms)')
    plt.xlabel('Type')
    plt.grid(True, axis='y', linestyle='--', alpha=0.7)
    plt.savefig('boxplot_duration.png')
    plt.close()
    print("Box plot saved to boxplot_duration.png")


    # 4. Histograms for each type
    unique_types = df['Type'].unique()
    n_types = len(unique_types)
    fig, axes = plt.subplots(1, n_types, figsize=(6 * n_types, 6))

    if n_types == 1:
        axes = [axes]
    
    min_val = df['Duration (ms)'].min()
    max_val = df['Duration (ms)'].max()
    bins = 3 # Using fewer bins for a small dataset

    for i, type_name in enumerate(unique_types):
        data_subset = df[df['Type'] == type_name]['Duration (ms)']
        ax = axes[i]
        
        ax.hist(data_subset, bins=bins, edgecolor='black', alpha=0.7)
        
        mean_val = data_subset.mean()
        ax.axvline(mean_val, color='red', linestyle='dashed', linewidth=1)
        min_ylim, max_ylim = ax.get_ylim()
        
        ax.text(mean_val, max_ylim * 0.95, 'Mean: {:.3f}ms'.format(mean_val), color='red', ha='center', fontsize=10)

        ax.set_title(f'Histogram for Type: {type_name}')
        ax.set_xlabel('Duration (ms)')
        ax.set_ylabel('Frequency')
        ax.set_xlim(min_val * 0.95, max_val * 1.05)
        ax.grid(True, axis='y', linestyle='--', alpha=0.7)

    plt.tight_layout()
    plt.savefig('histograms_by_type.png')
    plt.close()
    print("Histograms saved to histograms_by_type.png")

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: python analyze_timing.py <input_file_path>")
        sys.exit(1)
    file_path = sys.argv[1]
    analyze_timing_data(file_path)
