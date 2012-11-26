import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;


public class PostgreSQLConnector extends AnyConnector{

	
	Connection db;
	
	String table_name;
	
	PreparedStatement st_load;
	PreparedStatement st_get;
	PreparedStatement st_set;
	
	public void init(String db_url, String username, String password, String table_name){
		
		try{
			System.out.println(db_url);
			System.out.println(username);
			System.out.println(password);
			System.out.println(table_name);
			
			this.table_name = table_name;
			
			db = DriverManager.getConnection
					("jdbc:postgresql://localhost:5432/postgres", 
							"postgres", "bB19871121");
			
			Statement st = this.db.createStatement();
			
			System.out.println("DROP TABLE IF EXISTS " + table_name + " CASCADE;");
			boolean rs = st.execute("DROP TABLE IF EXISTS " + table_name + " CASCADE;");
	
			System.out.println("CREATE TABLE " + table_name + " (key integer, value text);");
			rs = st.execute("CREATE TABLE " + table_name + " (key integer, value text);");
			
			System.out.println("CREATE INDEX " + table_name + "_key ON " + table_name + "(key);");
			rs = st.execute("CREATE INDEX " + table_name + "_key ON " + table_name + "(key);");	
		
			st_load = db.prepareStatement("INSERT INTO " + table_name + " VALUES (?,?);");
			st_get = db.prepareStatement("SELECT value FROM " + table_name + " WHERE key = ?;");
			st_set = db.prepareStatement("UPDATE " + table_name + " SET value = ? WHERE key = ?;");
		}catch(Exception e){
			
		}
	}
	
	public Object get(int key){

		System.out.println(" get key=" + key);
		
		try{
				System.out.println("1");
			st_get.setInt(1, key);
				System.out.println("2");
			ResultSet rs = st_get.executeQuery();
				System.out.println("3");
			while (rs.next()) {
				System.out.println("4");
				byte[] vv =  rs.getBytes("value");
				
				System.out.print(" ^^^^^" + vv.length + "^^^^^  ");
				for(byte b : vv){
					System.out.print(b + " ");
				}
				System.out.println("\n");
				
			    return rs.getBytes(1);
			}
			System.out.println(" empty key=" + key);
		}catch(SQLException e){
			System.out.println(e.getMessage());
		}
	
		return null;
		
	}
	
	public void set(int key, byte[] value){

		//System.out.println(" =====" + value);
		
		try{
			st_set.setBytes(1, value);
			st_set.setInt(2, key);
			st_set.execute();
		}catch(Exception e){
			
		}
		
	}
	
	public void load(int key, byte[] value) {
		
		try{
			byte[] bbb = new byte[5];
			bbb[0] = 0;
			bbb[1] = 1;
			bbb[2] = 2;
			bbb[3] = 3;
			bbb[4] = 4;
			
			value = bbb;
			
			st_load.setBytes(2, value);
			st_load.setInt(1, key);
			
			System.out.print(" $$$$$" + value.length + "$$$$$  ");
			for(byte b : value){
				System.out.print(b + " ");
			}
			System.out.println("\n");
			
			st_load.execute();
			
			Statement st = db.createStatement();
			ResultSet r = st.executeQuery("SELECT value FROM tmp_kv_2 WHERE key = " + key);
			while (r.next()) {
				byte[] vv =  r.getBytes(1);
				
				System.out.print(" ^^^^^" + vv.length + "^^^^^  ");
				for(byte b : vv){
					System.out.print(b + " ");
				}
				System.out.println("\n");
			}
		}catch(Exception e){
			
		}
		
	}
	
	/**
	 * @param args
	 * @throws SQLException 
	 */
	/*
	public static void main(String[] args) throws SQLException {
		
		if(args.length != 4){
			System.out.println("ERROR: <db_url> <username> <password> <table_name>");
			System.exit(1);
		}
		
		String db_url = args[0];
		String username = args[1];
		String password = args[2];
		String table_name = args[3];
		
		PostgreSQLConnector.table_name = table_name;
		
		PostgreSQLConnector.init(db_url, username, password, table_name);

		//PostgreSQLConnector.load(1, "1");
		//PostgreSQLConnector.load(2, "2");
		//PostgreSQLConnector.load(3, "3");
		//PostgreSQLConnector.load(4, "4");
		//PostgreSQLConnector.load(5, "5");
		
		System.out.println(PostgreSQLConnector.get(1));
		System.out.println(PostgreSQLConnector.get(2));
		System.out.println(PostgreSQLConnector.get(3));
		System.out.println(PostgreSQLConnector.get(4));
		System.out.println(PostgreSQLConnector.get(5));
		
		//PostgreSQLConnector.set(1, "5");
		//PostgreSQLConnector.set(2, "4");
		//PostgreSQLConnector.set(3, "3");
		//PostgreSQLConnector.set(4, "2");
		//PostgreSQLConnector.set(5, "1");
		
		System.out.println(PostgreSQLConnector.get(1));
		System.out.println(PostgreSQLConnector.get(2));
		System.out.println(PostgreSQLConnector.get(3));
		System.out.println(PostgreSQLConnector.get(4));
		System.out.println(PostgreSQLConnector.get(5));		
		
	}
	*/

}
