package jdk8.lang;

import static org.junit.Assert.*;
import org.junit.Test;

public class BooleanTest {
    @Test public void constructorsAndPrimitiveValue() {
        assertTrue(new Boolean(true).booleanValue());
        assertFalse(new Boolean(false).booleanValue());
        assertTrue(new Boolean("TrUe").booleanValue());
        assertFalse(new Boolean("yes").booleanValue());
        assertFalse(new Boolean((String) null).booleanValue());
    }

    @Test public void constantsHaveDocumentedValues() {
        assertSame(Boolean.TRUE, Boolean.valueOf(true));
        assertSame(Boolean.FALSE, Boolean.valueOf(false));
        assertEquals(boolean.class, Boolean.TYPE);
    }

    @Test public void parseBooleanRecognizesOnlyTrueIgnoringCase() {
        assertTrue(Boolean.parseBoolean("true"));
        assertTrue(Boolean.parseBoolean("TRUE"));
        assertFalse(Boolean.parseBoolean("false"));
        assertFalse(Boolean.parseBoolean("1"));
        assertFalse(Boolean.parseBoolean(null));
    }

    @Test public void valueOfUsesCanonicalInstances() {
        assertSame(Boolean.TRUE, Boolean.valueOf("true"));
        assertSame(Boolean.TRUE, Boolean.valueOf("TrUe"));
        assertSame(Boolean.FALSE, Boolean.valueOf("anything"));
        assertSame(Boolean.FALSE, Boolean.valueOf((String) null));
    }

    @Test public void stringConversionsUseLowercaseWords() {
        assertEquals("true", Boolean.toString(true));
        assertEquals("false", Boolean.toString(false));
        assertEquals("true", Boolean.TRUE.toString());
        assertEquals("false", Boolean.FALSE.toString());
    }

    @Test public void equalityHashAndComparisonFollowBooleanOrdering() {
        assertEquals(Boolean.TRUE, new Boolean(true));
        assertNotEquals(Boolean.TRUE, Boolean.FALSE);
        assertEquals(1231, Boolean.TRUE.hashCode());
        assertEquals(1237, Boolean.FALSE.hashCode());
        assertEquals(1231, Boolean.hashCode(true));
        assertEquals(1237, Boolean.hashCode(false));
        assertTrue(Boolean.FALSE.compareTo(Boolean.TRUE) < 0);
        assertTrue(Boolean.compare(false, true) < 0);
        assertEquals(0, Boolean.compare(true, true));
    }

    @Test public void logicalOperationsMatchTruthTables() {
        assertFalse(Boolean.logicalAnd(true, false));
        assertTrue(Boolean.logicalAnd(true, true));
        assertTrue(Boolean.logicalOr(false, true));
        assertFalse(Boolean.logicalOr(false, false));
        assertTrue(Boolean.logicalXor(true, false));
        assertFalse(Boolean.logicalXor(true, true));
    }

    @Test public void getBooleanReadsSystemProperty() {
        String key = "jdk8.lang.BooleanTest.flag";
        String previous = System.getProperty(key);
        try {
            System.setProperty(key, "TrUe");
            assertTrue(Boolean.getBoolean(key));
            System.setProperty(key, "false");
            assertFalse(Boolean.getBoolean(key));
            System.clearProperty(key);
            assertFalse(Boolean.getBoolean(key));
        } finally {
            if (previous == null) System.clearProperty(key); else System.setProperty(key, previous);
        }
    }
}
