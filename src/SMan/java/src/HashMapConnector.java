import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.HashMap;


public class HashMapConnector extends AnyConnector{

	HashMap<Integer, byte[]> map = new HashMap<Integer, byte[]>();
	
	public void init(String db_url, String username, String password, String table_name){
		
	}

	public Object get(int key){
	
		
		//try {
		//	Thread.sleep(1);
		//} catch (InterruptedException e) {
		//	e.printStackTrace();
		//}
		
		return map.get(key);
		
	}
	
	public void set(int key, byte[] value) {
		
		
		//try {
		//	Thread.sleep(1);
		//} catch (InterruptedException e) {
		//	e.printStackTrace();
		//}
		
		map.put(key, value);
		
	}
	
	public void load(int key, byte[] value) {
		
		map.put(key, value.clone());
		
	}
}
