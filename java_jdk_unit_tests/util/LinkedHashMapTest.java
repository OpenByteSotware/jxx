package jdk8.util;

import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;

public class LinkedHashMapTest {
    @Test public void insertionOrderIsPreserved() {
        LinkedHashMap<String,Integer> map = new LinkedHashMap<String,Integer>();
        map.put("b",2); map.put("a",1); map.put("c",3); map.put("a",9);
        assertEquals(Arrays.asList("b","a","c"), new ArrayList<String>(map.keySet()));
        assertEquals(Integer.valueOf(9), map.get("a"));
    }
    @Test public void accessOrderMovesAccessedEntryToEnd() {
        LinkedHashMap<String,Integer> map = new LinkedHashMap<String,Integer>(16,0.75f,true);
        map.put("a",1); map.put("b",2); map.put("c",3); map.get("a");
        assertEquals(Arrays.asList("b","c","a"), new ArrayList<String>(map.keySet()));
    }
    @Test public void viewsCloneAndJava8OperationsWork() {
        LinkedHashMap<String,Integer> map = new LinkedHashMap<String,Integer>();
        map.put("a",1); map.putIfAbsent("b",2); map.compute("a",(k,v)->v+1);
        assertEquals(Arrays.asList(2,2), new ArrayList<Integer>(map.values()));
        @SuppressWarnings("unchecked") LinkedHashMap<String,Integer> copy = (LinkedHashMap<String,Integer>) map.clone();
        assertEquals(map, copy); assertNotSame(map, copy);
    }
    @Test public void removeEldestEntryCanImplementBoundedMap() {
        LinkedHashMap<Integer,Integer> map = new LinkedHashMap<Integer,Integer>() {
            protected boolean removeEldestEntry(Map.Entry<Integer,Integer> eldest) { return size() > 2; }
        };
        map.put(1,1); map.put(2,2); map.put(3,3);
        assertEquals(Arrays.asList(2,3), new ArrayList<Integer>(map.keySet()));
    }
}
