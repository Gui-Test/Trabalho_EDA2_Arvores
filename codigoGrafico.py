
import pandas as pd
import matplotlib.pyplot as plt
from google.colab import files

arquivos = files.upload()

df_add = pd.read_csv("adicao.csv")
df_remove = pd.read_csv("remocao.csv")
colunas = ["AVL", "ARN", "ARB_1", "ARB_5", "ARB_10"]
nomes = [
    "Árvore AVL",
    "Árvore Rubro-Negra",
    "Árvore B - Ordem 1",
    "Árvore B - Ordem 5",
    "Árvore B - Ordem 10"
]


plt.figure(figsize=(12, 6))

for coluna, nome in zip(colunas, nomes):
    plt.plot(
        df_add["Quantidade_de_nos"],
        df_add[coluna],
        marker="o",
        label=nome
    )

plt.title("Esforço Computacional Médio - Adição", fontsize=14)

plt.xlabel("Quantidade de nós", fontsize=12)
plt.ylabel("Esforço computacional médio", fontsize=12)

plt.xticks(df_add["Quantidade_de_nos"])
plt.grid(True, linestyle="--", alpha=0.6)
plt.legend()

plt.tight_layout()

plt.show()


plt.figure(figsize=(12, 6))

for coluna, nome in zip(colunas, nomes):
    plt.plot(
        df_remove["Quantidade_de_nos"],
        df_remove[coluna],
        marker="o",
        label=nome
    )

plt.title("Esforço Computacional Médio - Remoção", fontsize=14)
plt.xlabel("Quantidade de nós",fontsize=12)
plt.ylabel("Esforço computacional médio", fontsize=12)

plt.xticks(df_remove["Quantidade_de_nos"])
plt.grid(True, linestyle="--",  alpha=0.6)
plt.legend()
plt.tight_layout()

plt.show()
