//import java.util.concurrent.ConcurrentSkipListMap;
//import ConcurrentSkipListMap;


public class MyConcurrentSkipListMap
{
	static int count = 0;

	//public static ConcurrentSkipListMap sl;
	public static ConcurrentSkipListMap<String, String> sl;

	public static void create_sl() {
		sl = new ConcurrentSkipListMap<String, String>();
		count = 0;
	}
	public static void Put(String[] args) {
//		System.out.println("Put 1");
		String key = args[0];
		String val = args[1];

		String rv = sl.put(key, val);
//		System.out.println("Finished");
	}
	public static void Put(String key, String val) {
//		System.out.println("Put 2");
		String rv = sl.put(key, val);
//		System.out.println("Finished");
	}

	public static String Get(String key) {
		//System.out.println(sl.get(key));

		return sl.get(key);
	}
	public static void main(String[] args)
	{
        System.out.println("Hello World");
        long ops = 1000000;
        if (args.length > 0)
            ops = Long.parseLong(args[0]);
        //      System.out.println(args[0]);

        create_sl();

        long startTime = 0;
        long endTime = 0;
        double diffTime = 0;

        String[] kv = new String[2];
        startTime = System.nanoTime();
        for (long i = 0; i < ops; i++) {
            kv[0] = String.valueOf(i);
            kv[1] = String.valueOf(i + 100);
            Put(kv);
        }
        endTime = System.nanoTime();
        diffTime = (endTime - startTime)/(double)1000000000;
        System.out.printf("Lat = %.6f ", diffTime);
        System.out.printf(" OPS = %d IOPS = %.4f\n", ops, ops/diffTime );
    }

//		String city = Get("eunji");
//		System.out.println(city);
}
