#include <stdio.h>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <time.h>
#include <tuple> 

using namespace std;

pair<int, int> center (vector<vector<int> > *paths){
	int N = (*paths).size();			// The size of the vertex.
	vector<bool> seen(N, false);
	vector<int> depth(N, 0);			// The vector where we engister the depth of the vertex.
										// Initialize with 0.
	queue<int> BFS;
	int head;							// Cache in order to read the head of our BFS.
	int neighbor;						// A variable to make the code more readable.
	
	// We initialize BFS, seen and depth with the values of i (root).
	int i = 0;
	BFS.push(i);
	seen[i] = true;
	depth[i] = 0;
	
	while (!BFS.empty()) {
		/* We take the first element from BFS, then we check his neighbors((*paths)[head]). 
		Then, for every neighbors(neighbor), we check if it has been seen.
		If this is the case, then we update BFS, seen and depth with the values of neighbor. */
		head = BFS.front();
		BFS.pop();
		
		for (int k = 0; k < (*paths)[head].size(); k++){			// Check the neighbors of head.
			neighbor = (*paths)[head][k];
			if (!seen[neighbor]){									// Update
				seen[neighbor] = true;
				BFS.push(neighbor);
				depth[neighbor] = depth[head] + 1;
			}
		}
	}
	
	// Farest point from our root(i).
	int indice_temp = -1, depth_temp = -1;
	for (int j = 0; j < N; j++){
		if (depth[j] > depth_temp){
			depth_temp = depth[j];
			indice_temp = j;
		}
	}
	
	// We do the same thing for indice_temp.
	i = indice_temp;
	printf("%d", i);
	
	seen = vector<bool> (N, false);
	depth = vector<int> (N, 0);			// The vector where we engister the depth of the vertex.
										// Initialize with 0.
	vector<int> father (N, -1);
	
	// We initialize BFS, seen and depth with the values of i (root).
	BFS.push(i);
	seen[i] = true;
	depth[i] = 0;
	
	while (!BFS.empty()) {
		/* We take the first element from BFS, then we check his neighbors((*paths)[head]). 
		Then, for every neighbors(neighbor), we check if it has been seen.
		If this is the case, then we update BFS, seen and depth with the values of neighbor. */
		head = BFS.front();
		BFS.pop();
		
		for (int k = 0; k < (*paths)[head].size(); k++){			// Check the neighbors of head.
			neighbor = (*paths)[head][k];
			if (!seen[neighbor]){									// Update
				seen[neighbor] = true;
				BFS.push(neighbor);
				depth[neighbor] = depth[head] + 1;
				father[neighbor] = head;
			}
		}
	}

	printf("%d", i);
	// Farest point from our root(i).
	indice_temp = -1;
	depth_temp = -1;
	for (int j = 0; j < N; j++){
		if (depth[j] > depth_temp){
			depth_temp = depth[j];
			indice_temp = j;
		}
	}
	
	// Find the diameter
	vector<int> diameter;
	diameter.push_back(indice_temp);
	
	while (father[indice_temp] != i ){
		indice_temp = father[indice_temp];
		diameter.push_back(indice_temp);
	}
	//printf("%d", diameter.size());
	

	
	return pair<int, int> (diameter[0], diameter[diameter.size() - 1]);
	//return pair<int, int> (diameter[diameter.size() / 4], diameter[diameter.size() * 3 / 4]);
}
// the most weighted
pair<int, int> center_weight(vector<int> *weights){
	int heaviest_indice = -1;
	int heaviest_weight = 0;
	int weakest_indice = -1;
	int weakest_weight = INT_MAX;
	
	int N = (*weights).size();
	for (int i = 0; i < N; i++){
		if ((*weights)[i] > heaviest_weight){
			heaviest_indice = i;
			heaviest_weight = (*weights)[i];
		}
		if ((*weights)[i] < weakest_weight){
			weakest_indice = i;
			weakest_weight = (*weights)[i];
		}
	}
	return pair<int, int> (heaviest_indice, weakest_indice);
}

// Calculate the distance between i and every vertex with BFS
vector<int> all_distance(vector<vector<int> > *paths, int i){
	int N = (*paths).size();			// The size of the vertex.
	vector<bool> seen(N, false);
	vector<int> depth(N, 0);			// The vector where we engister the depth of the vertex.
										// Initialize with 0.
	queue<int> BFS;
	int head;							// Cache in order to read the head of our BFS.
	int neighbor;						// A variable to make the code more readable.
	
	// We initialize BFS, seen and depth with the values of i.
	BFS.push(i);
	seen[i] = true;
	depth[i] = 0;
	
	while (!BFS.empty()) {
		/* We take the first element from BFS, then we check his neighbors((*paths)[head]). 
		Then, for every neighbors(neighbor), we check if it has been seen.
		If this is the case, then we update BFS, seen and depth with the values of neighbor. */
		head = BFS.front();
		BFS.pop();
		
		for (int k = 0; k < (*paths)[head].size(); k++){			// Check the neighbors of head.
			neighbor = (*paths)[head][k];
			if (!seen[neighbor]){									// Update
				seen[neighbor] = true;
				BFS.push(neighbor);
				depth[neighbor] = depth[head] + 1;
			}
		}
	}
	return depth;
}

int one_center(vector<vector<int> > *paths, vector<int> *weights, int i, long long best_score){
	int N = (*paths).size();
	int best_indice = i;
	long long s = 0;
	for (int indice = 0; indice < (*paths)[i].size(); indice++){
		s = 0;
		int neighbor = (*paths)[i][indice];
		vector<int> distance = all_distance(paths, neighbor);
			for (int k = 0; k < N; k++){
				s += (*weights)[k] * distance[k];
			}
			if (s < best_score){
				best_score = s;
				best_indice = indice;
			}
	}
	if (best_indice == i) return i;
	return one_center(paths, weights, best_indice, best_score);
}

// Calculate the score with vertex i and j while we have already every distance between vertex. (+DP for the score)
long long score_with_distances(int N, map<pair<int, int>, long long> *score_DP, vector<vector<int> > *distance_between_vertex, vector<int> *weights, int a, int b){
	int i = min(a, b);
	int j = max(a, b);
	if ((*score_DP).find(pair<int, int>(i, j)) != (*score_DP).end()){
		return (*score_DP)[pair<int, int>(i, j)];
	}
	long long s = 0;
	for (int k = 0; k < N; k++){
		s += (*weights)[k] * min((*distance_between_vertex)[i][k], (*distance_between_vertex)[j][k]);
	}
	(*score_DP)[pair<int, int>(i, j)] = s;
	return s;
}

// Calculate the shortest distance with greedy.
tuple<int, int, long long> score_with_distances_greedy(int N, map<pair<int, int>, long long> *score_DP, 
	vector<vector<int> > *distance_between_vertex, vector<vector<int> > *paths, vector<int> *weights, 
	int x, int y){
	int i = min(x, y), j = max(x, y);
	if ((*distance_between_vertex)[i].empty()){
		(*distance_between_vertex)[i] = all_distance(paths, i);
	}
	if ((*distance_between_vertex)[j].empty()){
		(*distance_between_vertex)[j] = all_distance(paths, j);	
	}
	
	long long best_score = score_with_distances(N, score_DP, distance_between_vertex, weights, i, j);
	int best_indice_i = i;
	int best_indice_j = j;
	int indice_temp_1, indice_temp_2;
	long score_temp;


	for (int a = 0; a <= (*paths)[i].size(); a++){
		for (int b = 0; b <= (*paths)[j].size(); b++){
			indice_temp_1 = (a == (*paths)[i].size())? i: (*paths)[i][a];
			indice_temp_2 = (b == (*paths)[j].size())? j: (*paths)[j][b];
			
			if ((*distance_between_vertex)[indice_temp_1].empty()){
				(*distance_between_vertex)[indice_temp_1] = all_distance(paths, indice_temp_1);
			}
			if ((*distance_between_vertex)[indice_temp_2].empty()){
				(*distance_between_vertex)[indice_temp_2] = all_distance(paths, indice_temp_2);	
			}

			score_temp = score_with_distances(N, score_DP, distance_between_vertex, weights, indice_temp_1, indice_temp_2);
			
			if (score_temp < best_score){
				best_indice_i = indice_temp_1;
				best_indice_j = indice_temp_2;
				best_score = score_temp;
			}
		}
	}
	if ((best_indice_i == i) && (best_indice_j == j)) {
		tuple<int, int, long long> result;
		get<0>(result) = indice_temp_1;
		get<1>(result) = indice_temp_2;
		get<2>(result) = best_score;
		return result;
	}
	if ((best_indice_j == i) && (best_indice_i == j)) {
		tuple<int, int, long long> result;
		get<0>(result) = indice_temp_1;
		get<1>(result) = indice_temp_2;
		get<2>(result) = best_score;
		return result;
	}
	return score_with_distances_greedy(N, score_DP, distance_between_vertex, paths, weights, best_indice_i, best_indice_j);
}

// Calculate the shortest distance with greedy.
tuple<int, int, long long> score_with_distances_greedy2(int N, map<pair<int, int>, long long> *score_DP, 
	vector<vector<int> > *distance_between_vertex, vector<vector<int> > *paths, vector<int> *weights, 
	int x, int y, bool first){
	int i = min(x, y), j = max(x, y);
	if ((*distance_between_vertex)[i].empty()){
		(*distance_between_vertex)[i] = all_distance(paths, i);
	}
	if ((*distance_between_vertex)[j].empty()){
		(*distance_between_vertex)[j] = all_distance(paths, j);	
	}
	
	long long best_score = score_with_distances(N, score_DP, distance_between_vertex, weights, i, j);
	int best_indice_i = i;
	int best_indice_j = j;
	int indice_temp_1, indice_temp_2;
	long score_temp;

	if (first){
		for (int a = 0; a <= (*paths)[i].size(); a++){
			indice_temp_1 = (a == (*paths)[i].size())? i: (*paths)[i][a];
			indice_temp_2 = j;
			
			if ((*distance_between_vertex)[indice_temp_1].empty()){
				(*distance_between_vertex)[indice_temp_1] = all_distance(paths, indice_temp_1);
			}
			if ((*distance_between_vertex)[indice_temp_2].empty()){
				(*distance_between_vertex)[indice_temp_2] = all_distance(paths, indice_temp_2);	
			}

			score_temp = score_with_distances(N, score_DP, distance_between_vertex, weights, indice_temp_1, indice_temp_2);
			
			if (score_temp < best_score){
				best_indice_i = indice_temp_1;
				best_indice_j = indice_temp_2;
				best_score = score_temp;
			}
		}
	}
	else{
		for (int b = 0; b <= (*paths)[j].size(); b++){
			indice_temp_1 = i;
			indice_temp_2 = (b == (*paths)[j].size())? j: (*paths)[j][b];
			
			if ((*distance_between_vertex)[indice_temp_1].empty()){
				(*distance_between_vertex)[indice_temp_1] = all_distance(paths, indice_temp_1);
			}
			if ((*distance_between_vertex)[indice_temp_2].empty()){
				(*distance_between_vertex)[indice_temp_2] = all_distance(paths, indice_temp_2);	
			}

			score_temp = score_with_distances(N, score_DP, distance_between_vertex, weights, indice_temp_1, indice_temp_2);
			
			if (score_temp < best_score){
				best_indice_i = indice_temp_1;
				best_indice_j = indice_temp_2;
				best_score = score_temp;
			}
		}	
	}
	if ((best_indice_i == i) && (best_indice_j == j)) {
		tuple<int, int, long long> result;
		get<0>(result) = indice_temp_1;
		get<1>(result) = indice_temp_2;
		get<2>(result) = best_score;
		return result;
	}
	if ((best_indice_j == i) && (best_indice_i == j)) {
		tuple<int, int, long long> result;
		get<0>(result) = indice_temp_1;
		get<1>(result) = indice_temp_2;
		get<2>(result) = best_score;
		return result;
	}
	return score_with_distances_greedy2(N, score_DP, distance_between_vertex, paths, weights, best_indice_i, best_indice_j, first);
}


tuple<int, int, long long> score_with_distances_greedy1(int N, map<pair<int, int>, long long> *score_DP, 
	vector<vector<int> > *distance_between_vertex, vector<vector<int> > *paths, vector<int> *weights, 
	int x, int y){
		bool first = true;
		int best_indice_i = x;
		int best_indice_j = y;
		bool no_change = false;
		bool once = false;
		tuple<int, int, long long> tmp;
		
		while (!no_change){
			no_change = true;
			tmp = score_with_distances_greedy2(N, score_DP, distance_between_vertex, paths, weights, best_indice_i, best_indice_j, first);
			
			if ((best_indice_i != get<0>(tmp)) or (best_indice_j != get<1>(tmp))){
				best_indice_i = get<0>(tmp);
				best_indice_j = get<1>(tmp);
				if (once){
					once = false;
				}
			}
			else{
				if (once) no_change = true;
				once = true;
				first = !first;
			}
		}
		return tmp;
	}


int main(){
	int N;				// The maximum of N is 50000. We can use int for N.
	int temp1, temp2;	// Cache for reading the numbers.
	tuple<int, int, long long> best_score;	// Cache for saving the best_score.
	long long score_temp;	// Cache for saving the score local.
	int best_index_1, best_index_2;		// The best score is achieved with these two index.
	time_t seconds;		// Runtime.
	
	FILE *file;
	// Read file from centers.1.in to centers.10.in
	for (int i = 1; i < 11; i++){
		seconds = -time(NULL);
		// open file
		string filename = string("tests/centers.") + to_string(i) + string(".in");
		printf("Reading file: %s\n", filename.c_str());
		file = fopen(filename.c_str(), "r");
		
	// Reading input
		printf("Reading input...\n");
		// N
		fscanf(file, "%d", &N);
		// Variable initialization
		vector<vector<int> > paths (N);			// Initialize paths with N vectors of type int.
		vector<int> weights(N, 0);				// Initialize weights with N value 0.
		map<pair<int, int>, long long> score_DP;
	
		// Read paths. The vertex are reindexed from 0 to N - 1.
		for (int j = 0; j < N - 1 ; j++){
			fscanf(file, "%d %d", &temp1, &temp2);
			temp1 -= 1;
			temp2 -= 1;
			paths[temp1].push_back(temp2);
			paths[temp2].push_back(temp1);
		}
		
		// Read weights. The vertex are reindexed from 0 to N - 1.
		for (int j = 0; j < N; j++){
			fscanf(file, "%d", &temp1);
			weights[j] = temp1;
		}
		
		fclose(file);

		// Calculate
		printf("Calculating distance...");
	// method BFS for every distance between vertex.
		vector<vector<int> > distance_between_vertex (N);
		/*for (int a = 0; a < N; a++){
			distance_between_vertex[a] = all_distance(&paths, a);
		}*/
		printf("Finished!\n");
		
		printf("Calculating...");
		
		// We start with the best node from greedy algorithm.
		//int one = one_center(&paths, &weights, 0, LONG_MAX);
		//best_score = score_with_distances_greedy(N, &score_DP, &distance_between_vertex, &paths, &weights, one, one);
		
		// We start with the heaviest node.
		pair<int, int> two_center = center_weight(&weights);
		best_score = score_with_distances_greedy(N, &score_DP, &distance_between_vertex, &paths, &weights, two_center.first, two_center.second);
		
		// We start with the longest path and pick the two nodes from 1/4, 3/4 of this path.
		//pair<int, int> two_center = center(&paths);
		//best_score = score_with_distances_greedy(N, &score_DP, &distance_between_vertex, &paths, &weights, two_center.first, two_center.second);
		
		// Random.
		//best_score = score_with_distances_greedy(N, &score_DP, &distance_between_vertex, &paths, &weights, 14, 76);
		
		printf("Finished!\n");
		printf("The best score found with vertex1 :%d\t vertex2 :%d\t score :%ld\n", get<0>(best_score), get<1>(best_score), get<2>(best_score));
		
		// Print to file.
		filename = string("output/centers.") + to_string(i) + string(".out");
		printf("Outputing file: %s\n", filename.c_str());
		file = fopen(filename.c_str(), "w");
		fprintf(file, "%ld", get<2>(best_score));
		fclose (file);
		
		seconds += time(NULL);
		printf ("%ld seconds used\n\n", seconds);
		// End
	}
	//system("PAUSE");
	return 0;
}
