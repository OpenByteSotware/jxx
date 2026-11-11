package jdk8.util;

import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;

public class ArrayDequeTest {
    @Test public void dequeOperationsPreserveOrder() {
        ArrayDeque<String> deque = new ArrayDeque<String>();
        deque.addLast("b"); deque.addFirst("a"); deque.offerLast("c");
        assertEquals("a", deque.getFirst()); assertEquals("c", deque.getLast());
        assertEquals("a", deque.removeFirst()); assertEquals("c", deque.removeLast());
        assertEquals("b", deque.element());
    }
    @Test public void stackAndOccurrenceMethodsWork() {
        ArrayDeque<String> deque = new ArrayDeque<String>();
        deque.add("a"); deque.add("b"); deque.add("a");
        assertTrue(deque.removeFirstOccurrence("a"));
        assertTrue(deque.removeLastOccurrence("a"));
        deque.push("top"); assertEquals("top", deque.pop());
    }
    @Test public void iterationCloneArrayAndClearWork() {
        ArrayDeque<Integer> deque = new ArrayDeque<Integer>(Arrays.asList(1,2,3));
        assertEquals(Integer.valueOf(1), deque.iterator().next());
        assertEquals(Integer.valueOf(3), deque.descendingIterator().next());
        assertArrayEquals(new Integer[] {1,2,3}, deque.toArray(new Integer[0]));
        ArrayDeque<Integer> copy = deque.clone();
        assertEquals(deque.toString(), copy.toString()); assertNotSame(deque, copy);
        assertNotNull(deque.spliterator()); deque.clear(); assertTrue(deque.isEmpty());
    }
    @Test(expected = NullPointerException.class) public void nullElementsAreRejected() {
        new ArrayDeque<String>().add(null);
    }
    @Test public void emptyNonThrowingMethodsReturnNull() {
        ArrayDeque<String> deque = new ArrayDeque<String>();
        assertNull(deque.peek()); assertNull(deque.poll());
        assertNull(deque.peekFirst()); assertNull(deque.peekLast());
    }
}
