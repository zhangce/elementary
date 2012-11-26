import java.util.Iterator;
import java.util.Map.Entry;

import org.apache.accumulo.core.Constants;
import org.apache.accumulo.core.client.AccumuloException;
import org.apache.accumulo.core.client.AccumuloSecurityException;
import org.apache.accumulo.core.client.BatchWriter;
import org.apache.accumulo.core.client.Connector;
import org.apache.accumulo.core.client.MultiTableBatchWriter;
import org.apache.accumulo.core.client.Scanner;
import org.apache.accumulo.core.client.TableNotFoundException;
import org.apache.accumulo.core.client.ZooKeeperInstance;
import org.apache.accumulo.core.data.Key;
import org.apache.accumulo.core.data.Mutation;
import org.apache.accumulo.core.data.Range;
import org.apache.accumulo.core.data.Value;
import org.apache.accumulo.core.security.ColumnVisibility;
import org.apache.hadoop.io.Text;


public class AccumuloConnector extends AnyConnector{
	
	Connector connector;
	Scanner scan;
	BatchWriter writer;
	MultiTableBatchWriter mtbw;
	
	String table;
	
	public void init(String db_url, String username, String password, String table_name){
		
		String[] arr_it = table_name.split("/");
		String sinstance = arr_it[0];
		table = arr_it[1];
		
		byte[] pass = password.getBytes();
		
		ZooKeeperInstance instance = new ZooKeeperInstance(sinstance, db_url);
		
	    try {
	    	
	    	long memBuf = 1L; // bytes to store before sending a batch
			long timeout = 1000L; // milliseconds to wait before sending
			int numThreads = 10;
	    	
	    	connector = instance.getConnector(username, pass);
	    	
	    	if (!connector.tableOperations().exists(table)){
	    		connector.tableOperations().create(table);
	    	}
	    	
	    	scan = connector.createScanner(table, Constants.NO_AUTHS);
	    	mtbw = connector.createMultiTableBatchWriter(200000l, 300, 4);
	    	writer = mtbw.getBatchWriter(table);
	    	
		} catch (Exception e) {
			e.printStackTrace();
		}
	    
	}
	
	public Object get(int key){
		
		Iterator<Entry<Key,Value>> iter;
		
		synchronized(this){
			
			Text t = new Text(""+key);
			scan.setRange(new Range(t, t));
			iter = scan.iterator();
		}
		    
	    while (iter.hasNext()) {
	    	
	      Entry<Key,Value> e = iter.next();
	      Text colf = e.getKey().getColumnFamily();
	      Text colq = e.getKey().getColumnQualifier();
	      return e.getValue().get();
	    }
		
	    return null;
	}
	
	public void set(int key, byte[] value) {
		
		try{
			Mutation m = new Mutation(new Text(""+ key));
			
			Text colf = new Text("" + key);
			m.put(colf, new Text("" + key), 
					new Value(value));
			
			writer.addMutation(m);
			mtbw.flush();
			
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public void load(int key, byte[] value) {
		
	    //System.out.println("~~~" + key + ", " + value);

		try{
			Mutation m = new Mutation(new Text(""+ key));
			
			Text colf = new Text("" + key);
			m.put(colf, new Text("" + key), 
					new Value(value));
			
			writer.addMutation(m);
			mtbw.flush();
			
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args) throws AccumuloException, AccumuloSecurityException, TableNotFoundException {
		
		if (args.length < 5 || args.length > 7) {
			System.out.println("bin/accumulo accumulo.examples.helloworld.ReadData <instance name> <zoo keepers> <username> <password> <tablename> [startkey [endkey]]");
		    System.exit(1);
		}
		
		AccumuloConnector ac = new AccumuloConnector();
		ac.init("localhost:2181" , "root", "bB19871121", "i2/testtable");
		
		//ac.set(10, "1".getBytes());
		//ac.set(10, "2".getBytes());
		
		ac.mtbw.close();
	    
	}

}
