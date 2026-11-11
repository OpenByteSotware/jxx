package jdk8.util;

import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;

public class CollectionsTest {
    @Test public void sortingSearchingReversingAndRotatingWork() {
        List<Integer> list = new ArrayList<Integer>(Arrays.asList(3,1,2));
        Collections.sort(list); assertEquals(Arrays.asList(1,2,3), list);
        assertEquals(1, Collections.binarySearch(list,2));
        Collections.reverse(list); assertEquals(Arrays.asList(3,2,1), list);
        Collections.rotate(list,1); assertEquals(Arrays.asList(1,3,2), list);
        Collections.swap(list,0,2); assertEquals(Arrays.asList(2,3,1), list);
        Collections.shuffle(list, new Random(1)); assertEquals(3, list.size());
    }
    @Test public void extremaFrequencyAndDisjointWork() {
        List<Integer> list = Arrays.asList(2,1,2,3);
        assertEquals(Integer.valueOf(1), Collections.min(list)); assertEquals(Integer.valueOf(3), Collections.max(list));
        assertEquals(2, Collections.frequency(list,2));
        assertTrue(Collections.disjoint(list, Arrays.asList(8,9)));
        assertFalse(Collections.disjoint(list, Arrays.asList(3,9)));
    }
    @Test public void fillCopyReplaceAndSublistSearchWork() {
        List<String> destination = new ArrayList<String>(Arrays.asList("x","x","x"));
        Collections.copy(destination, Arrays.asList("a","b")); assertEquals(Arrays.asList("a","b","x"), destination);
        Collections.fill(destination,"z"); assertEquals(Arrays.asList("z","z","z"), destination);
        assertTrue(Collections.replaceAll(destination,"z","q"));
        assertEquals(1, Collections.indexOfSubList(Arrays.asList(1,2,3,2,3), Arrays.asList(2,3)));
        assertEquals(3, Collections.lastIndexOfSubList(Arrays.asList(1,2,3,2,3), Arrays.asList(2,3)));
    }
    @Test public void immutableAndSingletonFactoriesWork() {
        assertTrue(Collections.emptyList().isEmpty()); assertTrue(Collections.emptySet().isEmpty()); assertTrue(Collections.emptyMap().isEmpty());
        assertEquals(Arrays.asList("x"), Collections.singletonList("x"));
        assertEquals(Collections.singleton("x"), new HashSet<String>(Arrays.asList("x")));
        assertEquals(Integer.valueOf(1), Collections.singletonMap("x",1).get("x"));
        assertEquals(Arrays.asList("x","x","x"), Collections.nCopies(3,"x"));
    }
    @Test public void checkedSynchronizedAndUnmodifiableViewsWork() {
        List<String> source = new ArrayList<String>();
        List<String> checked = Collections.checkedList(source,String.class); checked.add("x");
        List<String> synchronizedList = Collections.synchronizedList(source); assertEquals(1,synchronizedList.size());
        List<String> unmodifiable = Collections.unmodifiableList(source); assertEquals("x",unmodifiable.get(0));
    }
    @Test(expected = UnsupportedOperationException.class) public void unmodifiableListRejectsMutation() {
        Collections.unmodifiableList(new ArrayList<String>()).add("x");
    }
    @Test public void reverseOrderComparatorsWork() {
        Comparator<Integer> reverse = Collections.reverseOrder(); assertTrue(reverse.compare(1,2)>0);
        Comparator<Integer> natural = Comparator.naturalOrder();
        assertTrue(Collections.reverseOrder(natural).compare(1,2)>0);
        assertNotNull(Collections.enumeration(Arrays.asList("a")));
        assertEquals(Arrays.asList("a","b"), Collections.list(Collections.enumeration(Arrays.asList("a","b"))));
    }
}
