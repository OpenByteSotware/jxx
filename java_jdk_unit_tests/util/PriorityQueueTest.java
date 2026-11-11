package jdk8.util;

import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;

public class PriorityQueueTest {
    @Test public void naturalOrderDeterminesHead() {
        PriorityQueue<Integer> queue = new PriorityQueue<Integer>();
        queue.add(3); queue.offer(1); queue.add(2);
        assertEquals(Integer.valueOf(1), queue.peek()); assertEquals(Integer.valueOf(1), queue.element());
        assertEquals(Integer.valueOf(1), queue.poll()); assertEquals(Integer.valueOf(2), queue.remove());
        assertEquals(Integer.valueOf(3), queue.poll()); assertNull(queue.poll());
    }
    @Test public void comparatorCanReverseOrder() {
        PriorityQueue<Integer> queue = new PriorityQueue<Integer>(Collections.reverseOrder());
        queue.addAll(Arrays.asList(1,3,2));
        assertEquals(Integer.valueOf(3), queue.poll()); assertNotNull(queue.comparator());
    }
    @Test public void collectionOperationsArraysAndSpliteratorWork() {
        PriorityQueue<String> queue = new PriorityQueue<String>(Arrays.asList("b","a","c"));
        assertTrue(queue.contains("b")); assertTrue(queue.remove("b")); assertEquals(2, queue.size());
        assertEquals(2, queue.toArray().length); assertNotNull(queue.iterator()); assertNotNull(queue.spliterator());
        queue.clear(); assertTrue(queue.isEmpty());
    }
    @Test(expected = NullPointerException.class) public void nullElementsAreRejected() {
        new PriorityQueue<String>().add(null);
    }
}
