#include <stdio.h>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <time.h>

using namespace std;

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

// Calculate the score with vertex i and j while we have already every distance between vertex. (+DP for the score)
long long score_with_distances(int N, vector<vector<int> > *distance_between_vertex, vector<int> *weights, int i, int j){
	long long s = 0;
	for (int k = 0; k < N; k++){
		s += (*weights)[k] * min((*distance_between_vertex)[i][k], (*distance_between_vertex)[j][k]);
	}
	return s;
}

int main(){
	int N;				// The maximum of N is 50000. We can use int for N.
	int temp1, temp2;	// Cache for reading the numbers.
	long long best_score;	// Cache for saving the best_score.
	long long score_temp;	// Cache for saving the score local.
	int best_index_1, best_index_2;		// The best score is achieved with these two index.
	time_t seconds;		// Runtime.
	
	FILE *file;
	// Read file from centers.1.in to centers.10.in
	for (int i = 4; i < 5; i++){
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
		for (int a = 0; a < N; a++){
			distance_between_vertex[a] = all_distance(&paths, a);
		}
	
	// method Floyd-Warshall for every distance between vertex. (slower)
		//vector<vector<int> > distance_between_vertex = all_distance_DP(&paths);
		printf("Finished!\n");

		// Violant : Try every possible couple.
		best_score = LONG_MAX;
		printf("Calculating...");
		for (int a = 0; a < N; a++){
			if (a % 500 == 0) printf("\n%d", a);
			for (int b = a + 1; b < N; b++){
				score_temp = score_with_distances(N, &distance_between_vertex, &weights, a, b);
				//score_temp = score(N, &paths, &weights, a, b);
				if (score_temp < best_score){
					best_index_1 = a;
					best_index_2 = b;
					best_score = score_temp;
				}
			}
		}
		// best_score = score_with_distances_greedy(N, &score_DP, &distance_between_vertex, &paths, &weights, 0 ,1);
		printf("Finished!\n");
		
		printf("The best score found with vertex1 :%d\t vertex2 :%d\t score :%ld\n", best_index_1, best_index_2, best_score);
		// Print to file.
		filename = string("output/centers.") + to_string(i) + string(".out");
		printf("Outputing file: %s\n", filename.c_str());
		file = fopen(filename.c_str(), "w");
		fprintf(file, "%ld", best_score);
		fclose (file);
		
		seconds += time(NULL);
		printf ("%ld seconds used\n\n", seconds);
		// End
	}
	return 0;
}
