import java.util.concurrent.ConcurrentSkipListMap;


public class MyConcurrentSkipListMap
{
	public static ConcurrentSkipListMap<String, String> sl;

	public static void create_sl() {
		sl = new ConcurrentSkipListMap();
	}

	public static void put_sl(String[] args) {
		String key = args[0];
		String val = args[1];

		System.out.println(key);
		sl.put(key, val);

	}
	public static String get_sl(String key) {
		System.out.println(sl.get(key));
		return sl.get(key);
	}

	public static void main(String[] args)
	{
		System.out.println("Hello World");
		//create_sl();
		//put_sl("eunji", "seoul");
		//put_sl("mike", "pusan");

		//String city = get_sl("eunji");

		//System.out.println(city);

	}
}
