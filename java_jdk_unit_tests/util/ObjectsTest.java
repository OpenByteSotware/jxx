package jdk8.util;

import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;

public class ObjectsTest {
    @Test public void equalityAndHashMethodsWork() {
        assertTrue(Objects.equals(null,null)); assertFalse(Objects.equals("a",null));
        assertTrue(Objects.deepEquals(new int[]{1,2},new int[]{1,2}));
        assertEquals("x".hashCode(),Objects.hashCode("x")); assertEquals(0,Objects.hashCode(null));
        assertEquals(Arrays.hashCode(new Object[]{"a",1}),Objects.hash("a",1));
    }
    @Test public void stringAndComparisonMethodsWork() {
        assertEquals("null",Objects.toString(null)); assertEquals("fallback",Objects.toString(null,"fallback"));
        assertEquals("x",Objects.toString("x")); assertTrue(Objects.compare(1,2,Comparator.<Integer>naturalOrder())<0);
    }
    @Test public void nullPredicatesAndRequirementsWork() {
        assertTrue(Objects.isNull(null)); assertFalse(Objects.nonNull(null));
        assertEquals("x",Objects.requireNonNull("x"));
        assertEquals("x",Objects.requireNonNull("x","message"));
        assertEquals("x",Objects.requireNonNull("x",()->"message"));
    }
    @Test(expected = NullPointerException.class) public void requireNonNullRejectsNull() { Objects.requireNonNull(null,"missing"); }
}
