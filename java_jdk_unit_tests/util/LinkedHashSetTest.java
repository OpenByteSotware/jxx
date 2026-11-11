package jdk8.util;

import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;

public class LinkedHashSetTest {
    @Test public void insertionOrderIsPreserved() {
        LinkedHashSet<String> set = new LinkedHashSet<String>();
        set.add("b"); set.add("a"); set.add("c"); set.add("a");
        assertEquals(Arrays.asList("b","a","c"), new ArrayList<String>(set));
    }
    @Test public void collectionOperationsCloneAndSpliteratorWork() {
        LinkedHashSet<Integer> set = new LinkedHashSet<Integer>(Arrays.asList(1,2,3));
        assertTrue(set.contains(2)); assertTrue(set.remove(2)); assertTrue(set.addAll(Arrays.asList(3,4)));
        assertArrayEquals(new Integer[] {1,3,4}, set.toArray(new Integer[0]));
        @SuppressWarnings("unchecked") LinkedHashSet<Integer> copy = (LinkedHashSet<Integer>) set.clone();
        assertEquals(set, copy); assertNotSame(set, copy); assertNotNull(set.spliterator());
    }
}
