class DiGraphAM {
	private int[][] adjTable;
	
	public DiGraphAM(int N) {
		adjTable = new int[N][N];
	}
	
	public void addEdge(int from, int to) {
		adjTable[from][to] = 1;
	}
	
	public void deleteEdge(int from, int to) {
		adjTable[from][to] = 0;
	}
	
	public int edgeCount() {
		int result = 0;
		for(int i = 0; i < adjTable.length; i ++)
			for(int j = 0; j < adjTable[0].length; j ++)
				result += adjTable[i][j];
		
		return result;
	}
	
	public int vertexCount() {
		return adjTable.length;
	}
	
	public void print() {
		for(int i = 0; i < adjTable.length; i ++) {
			System.out.print(i + " is connected to: ");
			
			boolean isFirst = true;
			for(int j = 0; j < adjTable[0].length; j ++) {
				if(adjTable[i][j] == 1) {
					if(!isFirst) System.out.print(", ");
					System.out.print(j);
					isFirst = false;
				}
			}
			System.out.println();
		}
	}
	
	private int[] indegrees() {
		int[] indegrees = new int[adjTable.length];
		
		for(int j = 0; j < adjTable[0].length; j ++) {
			for(int i = 0; i < adjTable.length; i ++) {
				indegrees[j] += adjTable[i][j];
			}
		}
		
		return indegrees;
	}
	
	public int[] topSort() throws Exception {
		int[] resultArray = new int[adjTable.length];
		int[] indegrees = indegrees();
		
		LQueue<Integer> waitingRoom = new LQueue<Integer>();
		
		int counter = 0;
		for (int i = 0; i < indegrees.length; i++) {
			if(indegrees[i] == 0) waitingRoom.enqueue(i);
		}
		
		while(!waitingRoom.isEmpty()) {
			int vertex = waitingRoom.dequeue();
			
			resultArray[counter++] = vertex;
			for(int j = 0; j < adjTable[0].length; j ++) {
				if(adjTable[vertex][j] == 1) {
					if(--indegrees[j] == 0)
						waitingRoom.enqueue(j);
				}
			}
		}
		if(counter != adjTable.length)
			throw new Exception();
		
		return resultArray;
	}
}