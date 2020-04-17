import java.io.File;
import java.util.LinkedList;
import java.util.Vector;

public class LinkDeletion {

	
	// Which test file we are using
    public static int fileNum = 10;
	
	
	// The tree in which we are looking for the two centers

	public static Tree tree;
	
	public static int[] weights;
		
	// Subtrees
	public static Tree T1;
	public static Tree T2;

	// adj_lists : stores the edges of "tree" in the form of adjacency lists

	public static Vector<Vector<Integer>> adj_lists; // adj_lists.get(i) is a vector containing all the neighbors of vertex i
	
	//variables useful for Goldman's algorithm:
	public static boolean stop = false; 
	public static int still_in_tree;
	public static int W1; // sum of the weights of the vertices in subtree T1
	public static int W2; //sum of the weights of the vertices in subtree T2
	
	
	//the c1,c2 with the lowest score until now
	public static int c1 = 0;
	public static int c2 = 0;
	public static long J = 0;
	
	public static void main(String[] args) throws Exception {
		
		
		// Step 1: Reading the file
		
		
		File file = new File("tests/centers."+fileNum+".in");
		
		tree = new Tree(file); // tree has 3 attributes : "N", "w" (array storing the weights of the vertices) and "edges" (array storing all edges once)
	
		weights = tree.w.clone();
		
		// ***Step 2: Calculate the adjacency lists*** (complexity O(n))

		adj_lists = tree.adj_lists();
		
		//***Step 3: *** 
		
		
		LinkDeletion.DFS_allCuts();
				
		c1+=1;
		c2+=1;
		
		System.out.println("c1:");
		System.out.println(c1);
		System.out.println("c2:");
		System.out.println(c2);
		System.out.println("J:");
		System.out.println(J);
	}
	
	public static void DFS_allCuts() { // computes c1, c2 with Goldman algorithm for every partition T1, T2
		
		LinkedList<Integer> path = new LinkedList<Integer>();
		path.add(0);
		boolean[] marked = new boolean[tree.N];
		marked[0] = true;
				
		aa:
		while (path.size()>0) {
			

			int y = path.getLast();
						
			for (int i = 0; i < adj_lists.get(y).size(); i++)  {
	        	
	            // b is neighbor of node y
	            int b = adj_lists.get(y).get(i); 
	  
	            
	            if (!marked[b]) {
	            	
	            	marked[b] = true;
	            	path.add(b);
	            	
	            	LinkDeletion.cut(y, b);
	            	
	            	
	            	continue aa;
	            }
	        
			}
			
			path.removeLast();
			
		}
		
	}
	
	public static void cut(int v1, int v2) {
		Object o1 = new Integer(v1);
		Object o2 = new Integer(v2);
		adj_lists.get(v1).remove(o2); 
		

		W1 = LinkDeletion.w_BFS(v1); // we compute the weight of F1 (ie the sum of the weights of its nodes)
		
		
		int newc1 = goldman(v1,W1);
		
		
		adj_lists.get(v2).remove(o1); 
		W2 = LinkDeletion.w_BFS(v2);
		int newc2 = goldman(v2,W2);
		
		adj_lists.get(v1).remove(o2); 
		adj_lists.get(v2).remove(o1); 

		long newJ = LinkDeletion.J(newc1)+ LinkDeletion.J(newc2); // see the definition of J in the report (different from the score S)
		adj_lists.get(v1).add(v2); 
		adj_lists.get(v2).add(v1); 

		if (J==0 || newJ < J) {
			J = newJ;
			c1 = newc1;
			c2 = newc2;
		}
	}
	
	public static int w_BFS(int v) { // computes the weight of a subtree starting from vertex v
		int weightSubTree = weights[v];
		
		
		// create a queue  
	    LinkedList<Integer> que = new LinkedList<Integer>(); 
	    // enqueue element start_pt 
	    que.add(v);  
	    
	    boolean[] seen = new boolean[LinkDeletion.tree.N];
	    
	    seen[v] = true;
	    
	    // do until queue is empty  
	    while (que.size() > 0)  
	    {  
	        // get the first element of queue and dequeue it 
	        int y = que.poll();  
	  
	        
	        // traverse neighbors of node y 
	        for (int i = 0; i < adj_lists.get(y).size(); i++)  {
	        	
	            // b is neighbor of node y
	            int b = adj_lists.get(y).get(i); 
	  
	            // if b has not been visited yet
	            if (!seen[b]) 
	            {  
	  
	                // enqueue b in queue  
	                que.add(b);  
	                seen[b] = true;
	                weightSubTree += weights[b];
	  	                
	            }  
	            
	        }  
	    } 
	    
	    return weightSubTree;
	}
	
	public static long J(int newc) {// uses BFS
		long j = 0;
		
	    // array to store level of each node  
		int level[] = new int[LinkDeletion.tree.N]; 
		
		// create a queue  
	    LinkedList<Integer> que = new LinkedList<Integer>(); 
	    // enqueue element start_pt 
	    que.add(newc);  
	    
	    boolean[] seen = new boolean[LinkDeletion.tree.N];
	    
	    seen[newc] = true;
	    
	    // do until queue is empty  
	    while (que.size() > 0)  
	    {  
	        // get the first element of queue and dequeue it 
	        int y = que.poll();  
	  
	        
	        // traverse neighbors of node y 
	        for (int i = 0; i < adj_lists.get(y).size(); i++)  {
	        	
	            // b is neighbor of node y
	            int b = adj_lists.get(y).get(i); 
	  
	            // if b has not been visited yet
	            if (!seen[b]) 
	            {  
	  
	                // enqueue b in queue  
	                que.add(b);  
	                seen[b] = true;
	                level[b] = level[y] + 1;
	                j += level[b] * LinkDeletion.tree.w[b];
	  	                
	            }  
	            
	        }  
	    } 
	    
	    return j;
	}
	
	public static int goldman(int v,int W) { // v : starting point of Goldman's algorithm
		still_in_tree = v;
		stop = false;
		while (!stop) {
			stop = step(W);
		}
		
		back_to_zero(); // we must re-initialize weights and the adj_lists with the original values (modified by method "step")

		return still_in_tree;
	}
	
	public static boolean step(int W) {
		if (adj_lists.get(still_in_tree).size() == 0) {
			return true;
		}
		int endVertex = LinkDeletion.endVertex();
		if (2*weights[endVertex] >= W) {
			still_in_tree = endVertex; //we stop : endVertex is a center
			return true;
		}
		else {
			int neighbor = adj_lists.get(endVertex).get(0); // the only neighbor of endVertex
			Object o = new Integer(endVertex);
			adj_lists.get(neighbor).remove(o); // we delete the link between neighbor and endVertex
			weights[neighbor] += weights[endVertex];
			still_in_tree = neighbor;
			return false;
		}
	}
	
	
	public static int endVertex() { //we start from "still_in_tree" and we follow any path until we find an end-vertex
		
		int last = still_in_tree;

		boolean[] marked = new boolean[tree.N];
		marked[still_in_tree] = true;
		
		int cnt = 0;
		
		aa:
		while (cnt < tree.N*10) {
			
			cnt+=1;
			
			for (int i = 0; i < adj_lists.get(last).size(); i++)  {
	        	
	            // b is neighbor of node y
	            int b = adj_lists.get(last).get(i); 
	  
	            
	            if (!marked[b]) {
	            	
	            	marked[b] = true;
	            	last = b;
	            	
	            	
	            	continue aa;
	            }
	        
			}
			
			break aa; // if "last" has no unmarked neighbor, then it is an end-vertex
		}
		
		return last;
	}
	
	public static void back_to_zero() {
		weights = tree.w.clone();
		adj_lists = tree.adj_lists();
	}
}
