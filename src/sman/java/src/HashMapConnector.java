import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.HashMap;


public class HashMapConnector {

	static HashMap<Integer, byte[]> map = new HashMap<Integer, byte[]>();
	
	public static void init(String db_url, String username, String password, String table_name)
			throws SQLException{
		
	}

	public static Object get(int key){
	
		return map.get(key);
		
	}
	
	public static void set(int key, byte[] value) {
	
		map.put(key, value);
		
	}
	
	public static void load(int key, byte[] value) {
		
		map.put(key, value.clone());
		
	}
}
