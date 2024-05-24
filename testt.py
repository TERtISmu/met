import networkx as nx
import matplotlib.pyplot as plt


# G = nx.complete_graph(5)
# nx.write_graphml_lxml(G, "fourpath.graphml")

G = nx.read_graphml("edgelistfile")

A = nx.adjacency_matrix(G)
print(A.todense())

values = [1, 2, 0, 3, 1]
nx.draw(G, node_color=values, pos=nx.shell_layout(G))
# nx.draw(G, with_labels=True, font_weight='bold')
plt.show()
