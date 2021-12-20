def updateTable(step):
    for i in range(4):
        idx = (i + step)%4 + 4
        print(f"{i} {idx}\n")

updateTable(8)