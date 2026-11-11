package jdk8.util;

import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;

public class TreeMapTest {
    @Test public void orderingAndNavigationWork() {
        TreeMap<Integer,String> map = new TreeMap<Integer,String>();
        map.put(3,"c"); map.put(1,"a"); map.put(2,"b");
        assertEquals(Arrays.asList(1,2,3), new ArrayList<Integer>(map.keySet()));
        assertEquals(Integer.valueOf(1), map.firstKey()); assertEquals(Integer.valueOf(3), map.lastKey());
        assertEquals(Integer.valueOf(1), map.lowerKey(2)); assertEquals(Integer.valueOf(2), map.floorKey(2));
        assertEquals(Integer.valueOf(2), map.ceilingKey(2)); assertEquals(Integer.valueOf(3), map.higherKey(2));
        assertEquals(Integer.valueOf(1), map.firstEntry().getKey()); assertEquals(Integer.valueOf(3), map.lastEntry().getKey());
    }
    @Test public void pollingAndDescendingViewsWork() {
        TreeMap<Integer,String> map = new TreeMap<Integer,String>();
        map.put(1,"a"); map.put(2,"b"); map.put(3,"c");
        assertEquals(Integer.valueOf(1), map.pollFirstEntry().getKey());
        assertEquals(Integer.valueOf(3), map.pollLastEntry().getKey());
        map.put(1,"a"); map.put(3,"c");
        assertEquals(Arrays.asList(3,2,1), new ArrayList<Integer>(map.descendingKeySet()));
    }
    @Test public void rangeMapsAreBackedByMap() {
        TreeMap<Integer,String> map = new TreeMap<Integer,String>();
        for (int i=1;i<=5;i++) map.put(i, String.valueOf(i));
        NavigableMap<Integer,String> sub = map.subMap(2,true,4,false);
        assertEquals(Arrays.asList(2,3), new ArrayList<Integer>(sub.keySet()));
        sub.remove(2); assertFalse(map.containsKey(2));
        assertEquals(Arrays.asList(1,3), new ArrayList<Integer>(map.headMap(4).keySet()));
        assertEquals(Arrays.asList(4,5), new ArrayList<Integer>(map.tailMap(4).keySet()));
    }
    @Test public void comparatorCloneAndJava8MethodsWork() {
        TreeMap<Integer,Integer> map = new TreeMap<Integer,Integer>(Collections.reverseOrder());
        map.put(1,1); assertNotNull(map.comparator());
        assertEquals(Integer.valueOf(2), map.computeIfAbsent(2, k -> k));
        assertEquals(Integer.valueOf(3), map.merge(2,1,(a,b)->a+b));
        @SuppressWarnings("unchecked") TreeMap<Integer,Integer> copy = (TreeMap<Integer,Integer>) map.clone();
        assertEquals(map, copy); assertNotSame(map, copy);
    }
}
