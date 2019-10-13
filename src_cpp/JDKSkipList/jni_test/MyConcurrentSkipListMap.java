import java.util.concurrent.ConcurrentSkipListMap;


public class MyConcurrentSkipListMap
{
	static int count = 0;

	public static ConcurrentSkipListMap<String, String> sl;

	public static void create_sl() {
		sl = new ConcurrentSkipListMap();
		count = 0;
	}

	public static void Put(String[] args) {
		String key = args[0];
		String val = args[1];

		long startTime = 0;
		long endTime = 0;
		long diffTime = 0;

		startTime = System.nanoTime();
	//	startTime = System.currentTimeMillis();
		sl.put(key, val);
	//	endTime = System.currentTimeMillis();
		endTime = System.nanoTime();
		diffTime = endTime - startTime;
		System.out.println(diffTime);
	}

	public static String Get(String key) {
		//System.out.println(sl.get(key));
		return sl.get(key);
	}

	public static void main(String[] args)
	{
	//	System.out.println("Hello World");
		long ops = 100;
		if(args.length > 0)
			ops = Long.parseLong(args[0]);
	//		System.out.println(args[0]);

		create_sl();

		long startTime = 0;
		long endTime = 0;
		double diffTime = 0;

		String[] kv = new String[2];
		
		startTime = System.nanoTime();
		for(long i = 0; i < ops; i++) {
			kv[0] = String.valueOf(i);
			kv[1] = String.valueOf(i+100);
			Put(kv);			
		}
/*
		endTime = System.nanoTime();
		diffTime = (endTime - startTime)/(double)1000000000;
		System.out.printf("Lat = %.6f ", diffTime);
		//System.out.printf("Lat = %.6f ", (double)diffTime/1000000000);
		//System.out.println("Lat = " + diffTime + " OPS = " + ops + " IOPS = " + diffTime/ops );
		System.out.printf(" OPS = %d IOPS = %.4f\n", ops, ops/diffTime );
		//System.out.println(" OPS = " + ops + " IOPS = " + ops/diffTime );
*/
		//create_sl();
		//put_sl("eunji", "seoul");
		//put_sl("mike", "pusan");

		//String city = get_sl("eunji");

		//System.out.println(city);

	}
}
