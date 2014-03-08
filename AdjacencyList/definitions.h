#pragma once

struct vertex_t {
	int id;
	int val;
};

struct edge_t {
	int src;
	int dst;
	int val;
};

bool operator==(const vertex_t &v1, const vertex_t &v2) {
	return v1.id == v2.id;
}

struct adjlist2 {
	std::map<int, std::vector<int> > in_edges;
	std::map<int, std::vector<int> > out_edges;

	std::vector<edge_t> edges;
	std::vector<vertex_t> vertices;

	void push(vertex_t &src, edge_t &e, vertex_t &dst) { // O(|V|)
		if (vertices.end() == std::find(vertices.begin(), vertices.end(), src))
			vertices.push_back(src);

		if (vertices.end() == std::find(vertices.begin(), vertices.end(), dst))
			vertices.push_back(dst);

		edges.push_back(e);
	}

	void create_lists() { // O(|E||V|)
		printf("SUMMARY\n|E|=%d\n|V|=%d\n", edges.size(), vertices.size());

		for (unsigned i = 0; i < edges.size(); ++i) {
			int src_idx = 0, dst_idx = 0;
			for (unsigned j = 0; j < vertices.size(); ++j) {
				if (vertices[j].id == edges[i].src) {
					src_idx = j;
					break;
				}
			}
			for (unsigned j = 0; j < vertices.size(); ++j) {
				if (vertices[j].id == edges[i].dst) {
					dst_idx = j;
					break;
				}
			}
			in_edges[dst_idx].push_back(i);
			out_edges[src_idx].push_back(i);
		}
		printf("|in_edges|=%d\n|out_edges|=%d\n", in_edges.size(), out_edges.size());
	}
};
