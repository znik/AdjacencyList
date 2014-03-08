#include <cstdio>
#include <vector>
#include <map>

#include <cassert>
#include <string>
#include <algorithm>

#include <fstream>
#include <sstream>

#include <time.h>

#ifndef WIN32
#include <likwid.h>
#endif

#include "definitions.h"


unsigned dataset = 10000;
unsigned dataload = 1;

void load_data(adjlist2 &al2) {
	std::ifstream fstream;
	fstream.open("twitter_combined.txt");

	if (!fstream.is_open()) {
		assert(false && "Input file not found...");
		return;
	}
	fstream.seekg (0, fstream.end);
	std::streamoff end = fstream.tellg();
	fstream.seekg (0, fstream.beg);

	std::string line;
	unsigned nul;	
	int percent = 0;

	std::vector<unsigned> v;
	unsigned counter = 0;
	while (std::getline(fstream, line) && counter++ < dataset) {
		if (line.empty()) continue;

		std::stringstream ss(line);
		edge_t e;
		ss >> nul;
		e.src = nul;
		ss >> nul;
		e.dst = nul;
		e.val = rand() % 40;

		vertex_t v1 = {e.src, rand() % 40};

		if (v.end() == std::find(v.begin(), v.end(), e.src)) {
			v.push_back(v1.id);
		}
		vertex_t v2 = {e.dst, rand() % 40};

		if (v.end() == std::find(v.begin(), v.end(), e.dst)) {
			v.push_back(v1.id);
		}
		al2.push(v1, e, v2);

		//
		std::streamoff length = fstream.tellg();
		if (percent < int(10000 * ((double)length) / end)) {
			percent = int(10000 * ((double)length) / end);
			printf("Bucketing: %d %%\r", percent);
		}
	}
	fstream.close();
}

int main(int argc, char *argv[]) {
#ifndef WIN32
	likwid_markerInit();
#endif
	if (argc >= 2) {
		dataset = atoi(argv[1]);
	}
	if (argc >= 3) {
		dataload = atoi(argv[2]);
	}
	printf("!DataSet size:%d\n", dataset);
	printf("!Iterations:%d\n", dataload);
	adjlist2 al2;
	load_data(al2);

	al2.create_lists();

	unsigned vcount = al2.vertices.size();
	unsigned in_ed = 0;
	unsigned out_ed = 0;

	likwid_markerStartRegion("Execution");
	
	for (unsigned repeat = 0; repeat < dataload; ++repeat)
	for (unsigned i = 0; i < vcount; ++i) {

		std::vector<int> &in_edges_ids = al2.in_edges[i];
		std::vector<int> &out_edges_ids = al2.out_edges[i];
		in_ed += in_edges_ids.size();
		out_ed += out_edges_ids.size();

		int sum = 0;

		for (unsigned j = 0; j < in_edges_ids.size(); ++j) {
			sum += al2.edges[in_edges_ids[j]].val;
		}
		int count = out_edges_ids.size();
		for (unsigned j = 0; j < out_edges_ids.size(); ++j) {
			al2.edges[out_edges_ids[j]].val = sum / count;
		}

	}

	likwid_markerStopRegion("Execution");
	
	printf("vcount=%d\n", int(vcount));
	printf("|in_edges|=%u\n|out_edges|=%u\n", in_ed, out_ed);
#ifndef WIN32
	likwid_markerClose();
#endif
	return 1;
}
