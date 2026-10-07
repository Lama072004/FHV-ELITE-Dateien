import matplotlib.pyplot as plt

Lagerbestand = (("Äpfel", 50, "green"), ("Bananen", 99, "yellow"), ("Birnen", 2, "gold"), ("Kirschen", 52, "red"), ("Orangen", 0, "orange"))

figure, axis = plt.subplots()

frucht_name = [item[0] for item in Lagerbestand]
frucht_menge = [item[1] for item in Lagerbestand]
frucht_farbe = [item[2] for item in Lagerbestand]

axis.set_title("Lagerbestand Obst")
axis.set_ylabel("Anzahl Steigen")
axis.legend( title="Fruchtfarben")

bars = axis.bar(frucht_name, frucht_menge, color=frucht_farbe)

for i in range(len(frucht_name)):
    plt.text(i, frucht_menge[1] + 1, frucht_menge[ i])

plt.show()