import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# https://www.python-graph-gallery.com/11-grouped-barplot

df = pd.read_csv("f1-engines-scores.csv", header=0)
df.drop(columns=["domain"], inplace=True)
data = df.values
# data = data.reshape((3, -1, 4))
data = data.reshape((-1, 4, 4))

print(data)

domains = ["WHS", "CT", "WHG"]
engines = ["Davinci", "Curie", "Babbage", "Ada"]
barWidth = 0.18

for i, domain in enumerate(domains):
    # 0: Windows, 1: Cook, 2: Wiki
    bars1 = data[i, :, 0]
    bars2 = data[i, :, 1]
    bars3 = data[i, :, 2]
    bars4 = data[i, :, 3]

    fig, ax = plt.subplots()

    r1 = np.arange(len(engines))
    r2 = [x + barWidth for x in r1]
    r3 = [x + barWidth for x in r2]
    r4 = [x + barWidth for x in r3]

    plt.bar(r1, bars1, color='#b4ffff', width=barWidth, edgecolor='white', label='1-shot')
    plt.bar(r2, bars2, color='#80deea', width=barWidth, edgecolor='white', label='2-shot')
    plt.bar(r3, bars3, color='#4bacb8', width=barWidth, edgecolor='white', label='3-shot')
    plt.bar(r4, bars4, color='#005662', width=barWidth, edgecolor='white', label='4-shot')

    # plt.ylim([0, 1])
    plt.xlabel('Engine', fontweight='bold')
    plt.title("{} F1 Scores".format(domain))
    plt.xticks([r + barWidth*3/2 for r in range(len(bars1))], engines)
    plt.legend()

    plt.show()