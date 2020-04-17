import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.Math;
import java.util.*;

public class Tree {
	int N; // number of vertices
	int[][] edges; // each element is an edge [i,j] with i<j
	int[] w; // each element w[i] is the weight of vertex i
	
	public Tree(int N0, int[][] edges0, int[] w0) { // explicit constructor (we don't use it in our algorithm "TwoCenters")
		this.N = N0;
		this.edges = edges0;
		this.w = w0;
	}
	
	public Tree(File file) throws Exception { // constructor using the .in test files
		FileInputStream fis = null;
		BufferedInputStream bis = null;
		DataInputStream dis = null;

		fis = new FileInputStream(file);

		// Here BufferedInputStream is added for fast reading.
		bis = new BufferedInputStream(fis);
		dis = new DataInputStream(bis);

		// **Storing the number of vertices**

		int N = Integer.parseInt(dis.readLine());
		// N is the first line of the file and denotes the number of vertices

		// **Storing the edges**

		int[][] edges = new int[N - 1][2]; // each line of the matrix represents an edge
		// we remark that in the entry, the edges are sorted: first the edges starting
		// with the smallest number, and always from a smaller number to a bigger number
		// ex: 1-2, 1-3, 2-3

		for (int i = 0; i < N - 1; i++) {
			String[] nb = dis.readLine().split(" ");
			edges[i][0] = Integer.parseInt(nb[0]);
			edges[i][1] = Integer.parseInt(nb[1]);
		}

		// **Storing the weights**

		int[] w = new int[N];

		// dis.available() returns 0 if the file does not have more lines.
		for (int i = 0; i < N; i++) {

			// this statement reads the line from the file and print it to
			// the console.
			w[i] = Integer.parseInt(dis.readLine());
		}

		fis.close();
		bis.close();
		dis.close();
		this.N = N;
		this.edges = edges;
		this.w = w;
		
	}
	
	
	// START OF METHODS FOR TWO CENTERS USING FLOYD WARSHALL
	
	public int[][] adjacency_matrix(){ // calculates the adjacency matrix of the tree
		// we decide to set to 1 the coefficients where (i,j) is an edge, to 0 the diagonal coefficients and to 100000 (representing +infinity) the coefficients i!=j where there is no edge (i,j)
		int[][] adj_matrix = new int[this.N][this.N];
		
		for (int i = 0; i < this.N ; i++) {
			for (int j = 0 ; j < this.N; j++) {
				if (i!=j)
					adj_matrix[i][j] = 100000;
			}
		}
		
		for (int i =0; i < this.N -1; i ++) {
			int v1 = this.edges[i][0];
			int v2 = this.edges[i][1];
			adj_matrix[v1-1][v2-1] = 1;
			adj_matrix[v2-1][v1-1] = 1;
		}
		
		return adj_matrix;
	}
	
	
	public int[][] distance_matrix_FM(){ // calculates the distances between all vertices with Floyd-Warshall algorithm (time complexity : O(n^3))
		// remark: Floyd-Marshall works for every graph. Here we have a tree, so we surely can find an algorithm that is more time efficient
		int[][] dist_matrix = this.adjacency_matrix();
		for (int k =0; k< this.N; k ++) {
			for (int i =0; i< this.N; i ++) {
				for (int j =0; j< this.N; j ++) {
					dist_matrix[i][j] = Math.min(dist_matrix[i][j],dist_matrix[i][k] + dist_matrix[k][j]);
				}
			}
		}
		
		return dist_matrix;
	}
	
	// END OF METHODS FOR TWO CENTERS FLOYD MARSHALL
	
	// START OF METHODS TO IMPROVE THE COMPLEXITY OF STEP 1
	
	public Vector<Vector<Integer>> adj_lists(){ // instead of doing an adjacency matrix like method "adjacency matrix", this method stores edges in the form of adjacency lists
		Vector<Vector<Integer>> adj_lists = new Vector<Vector<Integer>>();
		for (int i = 0; i < this.N; i++) {
			adj_lists.add(new Vector<Integer>());
		}
		
		for (int i=0; i< this.N - 1;i++) {
			int x = this.edges[i][0];
			int y = this.edges[i][1];
			adj_lists.get(x-1).add(y-1);
			adj_lists.get(y-1).add(x-1);
		}
		return adj_lists;
	}
	
	public int[][] distance_matrix_BFS(Vector<Vector<Integer>> adj_lists){ // Time complexity: n* O(n) = O(n^2)
		int[][] dist_matrix_BFS = new int[this.N][this.N];
		
		
		// create a queue  
	    Queue<Integer> que = new LinkedList<Integer>();  
		
		for (int x =0 ; x < this.N; x++) { // for each vertex, we use BFS to calculate the distance 

	    	// enqueue element x  
		    que.add(x);  
		    
		  
		    // do until queue is empty  
		    while (que.size() > 0)  
		    {  
		        // get the first element of queue  
		        int y = que.peek();  
		  
		        // dequeue element  
		        que.remove();  
		        
		        // traverse neighbors of node y 
		        for (int i = 0; i < adj_lists.get(y).size(); i++)  {
		        	
		            // b is neighbor of node y
		            int b = adj_lists.get(y).get(i); 
		  
		            // if the distance between x and b is not already calculated and written in dist_matrix
		            if (b !=x && dist_matrix_BFS[x][b] ==0) 
		            {  
		  
		                // enqueue b in queue  
		                que.add(b);  
		  
		                // distance from x to b is distance of x to y + 1  
		                dist_matrix_BFS[x][b] = dist_matrix_BFS[x][y] + 1;  
		                
		            }  
		            
		        }  
		    } 
		    
		    
		}
		
		return dist_matrix_BFS;
	}
	
	
	

}
