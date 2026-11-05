package jdk8.util;
import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;
public class HashSetTest {
 @Test public void setRejectsDuplicatesAndSupportsNull(){HashSet<String>s=new HashSet<String>();assertTrue(s.add("a"));assertFalse(s.add("a"));assertTrue(s.add(null));assertEquals(2,s.size());assertTrue(s.contains(null));assertTrue(s.remove("a"));assertFalse(s.contains("a"));}
 @Test public void collectionConstructorAndBulkOperations(){HashSet<Integer>s=new HashSet<Integer>(Arrays.asList(1,2,2,3));assertEquals(3,s.size());assertTrue(s.containsAll(Arrays.asList(1,2)));assertTrue(s.addAll(Arrays.asList(3,4)));assertTrue(s.removeAll(Arrays.asList(1,9)));assertTrue(s.retainAll(Arrays.asList(2,4)));assertEquals(new HashSet<Integer>(Arrays.asList(2,4)),s);}
 @Test public void cloneArrayIterationAndClear(){HashSet<Integer>s=new HashSet<Integer>(Arrays.asList(1,2,3));int sum=0;for(Integer n:s)sum+=n;assertEquals(6,sum);assertEquals(3,s.toArray().length);Integer[]a=s.toArray(new Integer[0]);assertEquals(3,a.length);@SuppressWarnings("unchecked") HashSet<Integer>c=(HashSet<Integer>)s.clone();assertEquals(s,c);assertNotSame(s,c);assertNotNull(s.spliterator());s.clear();assertTrue(s.isEmpty());}
 @Test public void removeIfAndForEachWork(){HashSet<Integer>s=new HashSet<Integer>(Arrays.asList(1,2,3,4));assertTrue(s.removeIf(n->n%2==0));final int[]sum={0};s.forEach(n->sum[0]+=n);assertEquals(4,sum[0]);}
}
