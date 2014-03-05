public class Algorithms {
	public static void linearAlgTime(long N) {
		for(int i = 0; i < N; i ++) {
			int x = 1;
		}
	}

	public static void quadraticAlgTime(long N) {
		for(int i = 0; i < N; i ++) {
			for(int j = 0; j < N; j ++) {
				int x = 1;
			}
		}
	}

	public static void logarithmicAlgTime(long N) {
		for(int i = N; i > 1; i /= 2) {
			int x = 1;
		}
	}

	public static void NlogNAlgTime(long N) {
		for(int i = 0; i < N; i ++) {
			for(int j = N; j > 1; j /= 2) {
				int x = 1;
			}
		}
	}
}
