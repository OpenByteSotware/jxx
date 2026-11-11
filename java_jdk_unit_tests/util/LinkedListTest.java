package jdk8.util;

import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;

public class LinkedListTest {
    @Test public void listAndDequeOperationsWork() {
        LinkedList<String> list = new LinkedList<String>();
        list.add("b"); list.addFirst("a"); list.addLast("c");
        assertEquals(Arrays.asList("a", "b", "c"), list);
        assertEquals("a", list.getFirst()); assertEquals("c", list.getLast());
        assertEquals("a", list.removeFirst()); assertEquals("c", list.removeLast());
    }
    @Test public void queueStackAndOccurrenceOperationsWork() {
        LinkedList<String> list = new LinkedList<String>(Arrays.asList("a", "b", "a"));
        assertTrue(list.removeFirstOccurrence("a"));
        assertTrue(list.removeLastOccurrence("a"));
        list.offer("tail"); list.offerFirst("head"); list.offerLast("last");
        assertEquals("head", list.peekFirst()); assertEquals("last", list.peekLast());
        list.push("top"); assertEquals("top", list.pop()); assertEquals("head", list.poll());
    }
    @Test public void indexIterationCloneAndArraysWork() {
        LinkedList<Integer> list = new LinkedList<Integer>(Arrays.asList(1, 2, 3, 2));
        assertEquals(1, list.indexOf(2)); assertEquals(3, list.lastIndexOf(2));
        Iterator<Integer> descending = list.descendingIterator();
        assertEquals(Integer.valueOf(2), descending.next());
        assertArrayEquals(new Integer[] {1,2,3,2}, list.toArray(new Integer[0]));
        @SuppressWarnings("unchecked") LinkedList<Integer> copy = (LinkedList<Integer>) list.clone();
        assertEquals(list, copy); assertNotSame(list, copy); assertNotNull(list.spliterator());
    }
    @Test public void emptyDequeMethodsReturnNull() {
        LinkedList<String> list = new LinkedList<String>();
        assertNull(list.peek()); assertNull(list.poll()); assertNull(list.peekFirst());
        assertNull(list.peekLast()); assertNull(list.pollFirst()); assertNull(list.pollLast());
    }
    @Test(expected = NoSuchElementException.class) public void elementRejectsEmptyList() {
        new LinkedList<String>().element();
    }
}
