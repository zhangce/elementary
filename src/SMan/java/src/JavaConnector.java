import java.util.ArrayList;


public class JavaConnector {

	public static ArrayList<AnyConnector> connectors
		= new ArrayList<AnyConnector>();
	
	
	public static int push_new(String name){
		if(name.equals("HashMapConnector")){
			connectors.add(new HashMapConnector());
		}
		if(name.equals("AccumuloConnector")){
			connectors.add(new AccumuloConnector());
		}
		return connectors.size() - 1;
	}
	
	public static void init(int cid, String db_url, String username, String password, String table_name){
		connectors.get(cid).init(db_url, username, password, table_name);
	}

	public static Object get(int cid, int key){
    
    //System.out.println("#### cid = " + cid);
    //System.out.println("#### key = " + key);
    //System.out.println("#### result = " + connectors.get(cid).get(key));
    
		return connectors.get(cid).get(key);
	}
	
	public static void set(int cid, int key, byte[] value){
		connectors.get(cid).set(key, value);
	}
	
	public static void load(int cid, int key, byte[] value){
		connectors.get(cid).load(key, value);
	}
	
	
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
	
	}

}
