import java.sql.SQLException;


public abstract class AnyConnector {

	public abstract void init(String db_url, String username, String password, String table_name);

	public abstract Object get(int key);
	
	public abstract void set(int key, byte[] value);
	
	public abstract void load(int key, byte[] value);

}
