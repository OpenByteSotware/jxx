package jdk8.util;

import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;

public class TreeSetTest {
    @Test public void naturalOrderingAndNavigationWork() {
        TreeSet<Integer> set = new TreeSet<Integer>(Arrays.asList(3,1,2));
        assertEquals(Arrays.asList(1,2,3), new ArrayList<Integer>(set));
        assertEquals(Integer.valueOf(1), set.first()); assertEquals(Integer.valueOf(3), set.last());
        assertEquals(Integer.valueOf(1), set.lower(2)); assertEquals(Integer.valueOf(2), set.floor(2));
        assertEquals(Integer.valueOf(2), set.ceiling(2)); assertEquals(Integer.valueOf(3), set.higher(2));
    }
    @Test public void pollingAndDescendingViewsWork() {
        TreeSet<Integer> set = new TreeSet<Integer>(Arrays.asList(1,2,3));
        assertEquals(Integer.valueOf(1), set.pollFirst()); assertEquals(Integer.valueOf(3), set.pollLast());
        set.addAll(Arrays.asList(1,3));
        assertEquals(Arrays.asList(3,2,1), new ArrayList<Integer>(set.descendingSet()));
        assertEquals(Integer.valueOf(3), set.descendingIterator().next());
    }
    @Test public void rangeViewsAreBackedBySet() {
        TreeSet<Integer> set = new TreeSet<Integer>(Arrays.asList(1,2,3,4,5));
        NavigableSet<Integer> sub = set.subSet(2, true, 4, false);
        assertEquals(new TreeSet<Integer>(Arrays.asList(2,3)), sub);
        sub.remove(2); assertFalse(set.contains(2));
        assertEquals(new TreeSet<Integer>(Arrays.asList(1,3)), set.headSet(4));
        assertEquals(new TreeSet<Integer>(Arrays.asList(4,5)), set.tailSet(4));
    }
    @Test public void comparatorCloneAndSpliteratorWork() {
        TreeSet<Integer> set = new TreeSet<Integer>(Collections.reverseOrder()); set.addAll(Arrays.asList(1,2));
        assertNotNull(set.comparator());
        @SuppressWarnings("unchecked") TreeSet<Integer> copy = (TreeSet<Integer>) set.clone();
        assertEquals(set, copy); assertNotSame(set, copy); assertNotNull(set.spliterator());
    }
}
