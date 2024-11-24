import pandas as pd
import matplotlib.pyplot as plt

def plot_data(df, title):
    plt.figure(figsize=(10, 6))
    for column in df.columns[1:]:
        plt.plot(df['Size'], df[column], label=column)

    plt.xlabel('Array Size')
    plt.ylabel('Time (microseconds)')
    plt.title(title)
    plt.legend()
    plt.grid(True)

    plt.show()

random_df = pd.read_csv("../data/random_data.csv")
reverse_df = pd.read_csv("../data/reverse_data.csv")
almost_df = pd.read_csv("../data/almost_data.csv")

plot_data(random_df, "Random Data")

plot_data(reverse_df, "Reverse Sorted Data")

plot_data(almost_df, "Almost Sorted Data")