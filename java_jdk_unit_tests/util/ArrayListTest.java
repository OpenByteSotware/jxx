package jdk8.util;
import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;
public class ArrayListTest {
 @Test public void constructionAndBasicOperations(){
	 ArrayList<String> x=new ArrayList<String>(); 
	 assertTrue(x.isEmpty()); 
	 assertTrue(x.add("a"));
	 x.add(0,"z");
 assertEquals(Arrays.asList("z","a"),x);
 assertEquals(2,x.size()); 
 assertEquals("a",x.get(1)); 
 assertEquals("a",x.set(1,"b")); 
 assertEquals("b",x.get(1));
 }
 
 @Test public void collectionOperations(){
	 ArrayList<String>x=new ArrayList<String>(Arrays.asList("a","b","c","b"));
	 assertTrue(x.contains("b")); 
 assertEquals(1,x.indexOf("b"));
 assertEquals(3,x.lastIndexOf("b")); assertTrue(x.remove("b")); assertEquals("c",x.remove(1));
 assertTrue(x.containsAll(Arrays.asList("a","b")));
 x.addAll(Arrays.asList("d","e")); x.addAll(1,Arrays.asList("x","y")); 
 assertEquals(Arrays.asList("a","x","y","b","d","e"),x);}
 
 @Test public void bulkAndArrayOperations(){
	 ArrayList<String>x=new ArrayList<String>(Arrays.asList("a","b","c"));
	 assertTrue(x.removeAll(Arrays.asList("b","z")));
 assertTrue(x.retainAll(Arrays.asList("a","z"))); 
 Object[] a=x.toArray();
 assertArrayEquals(new Object[]{"a"},a);
 
 String[] s=x.toArray(new String[0]);
 assertArrayEquals(new String[]{"a"},s);
 x.clear(); assertTrue(x.isEmpty());
 }
 
 @Test public void iterationSublistAndClone(){ArrayList<Integer>x=new ArrayList<Integer>(Arrays.asList(1,2,3)); 
 int sum=0; 
 for(Integer n:x)sum+=n; 
 assertEquals(6,sum); ListIterator<Integer>it=x.listIterator(1); 
 assertTrue(it.hasNext());
 assertEquals(Integer.valueOf(2),it.next()); 
 assertEquals(Arrays.asList(2,3),x.subList(1,3)); 
 @SuppressWarnings("unchecked") ArrayList<Integer>copy=(ArrayList<Integer>)x.clone(); 
 assertEquals(x,copy); 
 assertNotSame(x,copy);
 }
 
 @Test public void java8Operations(){
	 ArrayList<Integer>x=new ArrayList<Integer>(Arrays.asList(3,1,2));
	 x.removeIf(n->n==1); 
 x.replaceAll(n->n*2); x.sort(null); assertEquals(Arrays.asList(4,6),x);
 final int[] sum={0}; x.forEach(n->sum[0]+=n); 
 assertEquals(10,sum[0]);
 assertNotNull(x.spliterator());
 }
 
 @Test(expected=IndexOutOfBoundsException.class)
 public void getRejectsInvalidIndex(){new ArrayList<String>().get(0);}
}
