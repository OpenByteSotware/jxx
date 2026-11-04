package jdk8.lang;

import static org.junit.Assert.*;
import org.junit.Test;

public class ShortTest {
    @Test public void constantsAreDocumented() {
        assertEquals(-32768, Short.MIN_VALUE);
        assertEquals(32767, Short.MAX_VALUE);
        assertEquals(16, Short.SIZE);
        assertEquals(2, Short.BYTES);
        assertEquals(short.class, Short.TYPE);
    }

    @Test public void constructorsAndNumberConversionsWork() {
        Short value = new Short((short) 12345);
        assertEquals((byte) 57, value.byteValue());
        assertEquals((short) 12345, value.shortValue());
        assertEquals(12345, value.intValue());
        assertEquals(12345L, value.longValue());
        assertEquals(12345.0f, value.floatValue(), 0.0f);
        assertEquals(12345.0d, value.doubleValue(), 0.0d);
        assertEquals((short) -12, new Short("-12").shortValue());
    }

    @Test public void parsingSupportsDecimalAndRadix() {
        assertEquals((short) 1234, Short.parseShort("1234"));
        assertEquals((short) 32767, Short.parseShort("7fff", 16));
        assertEquals((short) 10, Short.parseShort("1010", 2));
    }

    @Test(expected = NumberFormatException.class)
    public void parsingRejectsOverflow() { Short.parseShort("32768"); }

    @Test public void valueOfAndDecodeWork() {
        assertEquals(Short.valueOf((short) 10), Short.valueOf("10"));
        assertEquals(Short.valueOf((short) 15), Short.valueOf("f", 16));
        assertEquals(Short.valueOf((short) 16), Short.decode("0x10"));
        assertEquals(Short.valueOf((short) 8), Short.decode("010"));
    }

    @Test public void stringEqualityHashAndComparisonWork() {
        Short value = Short.valueOf((short) 1200);
        assertEquals("1200", value.toString());
        assertEquals("1200", Short.toString((short) 1200));
        assertEquals(value, Short.valueOf((short) 1200));
        assertNotEquals(value, Short.valueOf((short) 1201));
        assertEquals(1200, value.hashCode());
        assertEquals(1200, Short.hashCode((short) 1200));
        assertTrue(value.compareTo(Short.valueOf((short) 1201)) < 0);
        assertTrue(Short.compare((short) 1201, (short) 1200) > 0);
    }

    @Test public void reverseBytesSwapsBothBytes() {
        assertEquals((short) 0x3412, Short.reverseBytes((short) 0x1234));
    }

    @Test public void unsignedConversionsZeroExtend() {
        assertEquals(65535, Short.toUnsignedInt((short) -1));
        assertEquals(65535L, Short.toUnsignedLong((short) -1));
        assertEquals(32768, Short.toUnsignedInt(Short.MIN_VALUE));
    }
}
