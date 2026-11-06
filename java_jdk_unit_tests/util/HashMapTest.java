package jdk8.util;
import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;
public class HashMapTest {
 @Test public void putGetRemoveAndNullSupport(){
	 HashMap<String,Integer>m=new HashMap<String,Integer>();
	 assertTrue(m.isEmpty()); assertNull(m.put("a",1));
	 assertEquals(Integer.valueOf(1),m.put("a",2)); m.put(null,3); m.put("nullValue",null); assertEquals(Integer.valueOf(2),m.get("a"));
	 assertEquals(Integer.valueOf(3),m.get(null)); assertTrue(m.containsKey("nullValue")); assertTrue(m.containsValue(null)); 
	 assertEquals(Integer.valueOf(2),m.remove("a")); assertFalse(m.containsKey("a"));}
 
 @Test public void viewsAreBackedByMap(){HashMap<String,Integer>m=new HashMap<String,Integer>();
 m.put("a",1);m.put("b",2); assertEquals(new HashSet<String>(Arrays.asList("a","b")),m.keySet()); 
 assertTrue(m.values().containsAll(Arrays.asList(1,2))); 
 for(Map.Entry<String,Integer>e:m.entrySet())if(e.getKey().equals("a"))e.setValue(9);
 assertEquals(Integer.valueOf(9),m.get("a")); m.keySet().remove("b");
 assertFalse(m.containsKey("b"));}
 
 @Test public void copyCloneEqualityAndClear(){HashMap<String,Integer>a=new HashMap<String,Integer>();
 a.put("x",1);HashMap<String,Integer>b=new HashMap<String,Integer>(a);assertEquals(a,b);
 assertEquals(a.hashCode(),b.hashCode());
 @SuppressWarnings("unchecked") HashMap<String,Integer>c=(HashMap<String,Integer>)a.clone();
 assertEquals(a,c);assertNotSame(a,c);c.clear();assertTrue(c.isEmpty());
 }
 
 @Test public void java8MapOperations(){HashMap<String,Integer>m=new HashMap<String,Integer>(); 
 assertEquals(Integer.valueOf(1),m.putIfAbsent("a",1)); assertEquals(Integer.valueOf(1),m.getOrDefault("a",9));
 assertEquals(Integer.valueOf(9),m.getOrDefault("x",9)); assertTrue(m.replace("a",1,2));
 assertEquals(Integer.valueOf(2),m.replace("a",3)); assertEquals(Integer.valueOf(4),m.computeIfAbsent("b",k->4)); 
 assertEquals(Integer.valueOf(5),m.computeIfPresent("b",(k,v)->v+1)); assertEquals(Integer.valueOf(1),m.compute("c",(k,v)->1));
 assertEquals(Integer.valueOf(7),m.merge("c",6,(x,y)->x+y)); m.replaceAll((k,v)->v*2); assertEquals(Integer.valueOf(14),m.get("c")); 
 final int[] sum={0};m.forEach((k,v)->sum[0]+=v);assertEquals(24,sum[0]);assertTrue(m.remove("a",6));}
}
