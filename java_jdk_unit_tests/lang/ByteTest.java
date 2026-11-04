package jdk8.lang;

import static org.junit.Assert.*;
import org.junit.Test;

public class ByteTest {
    @Test public void constantsAreDocumented() {
        assertEquals(-128, Byte.MIN_VALUE);
        assertEquals(127, Byte.MAX_VALUE);
        assertEquals(8, Byte.SIZE);
        assertEquals(1, Byte.BYTES);
        assertEquals(byte.class, Byte.TYPE);
    }

    @Test public void constructorsAndNumberConversionsWork() {
        Byte value = new Byte((byte) 127);
        assertEquals((byte) 127, value.byteValue());
        assertEquals((short) 127, value.shortValue());
        assertEquals(127, value.intValue());
        assertEquals(127L, value.longValue());
        assertEquals(127.0f, value.floatValue(), 0.0f);
        assertEquals(127.0d, value.doubleValue(), 0.0d);
        assertEquals((byte) -12, new Byte("-12").byteValue());
    }

    @Test public void parsingSupportsDecimalAndRadix() {
        assertEquals((byte) 42, Byte.parseByte("42"));
        assertEquals((byte) -128, Byte.parseByte("-128"));
        assertEquals((byte) 127, Byte.parseByte("7f", 16));
        assertEquals((byte) 10, Byte.parseByte("1010", 2));
    }

    @Test(expected = NumberFormatException.class)
    public void parsingRejectsOverflow() { Byte.parseByte("128"); }

    @Test public void valueOfAndDecodeWork() {
        assertEquals(Byte.valueOf((byte) 10), Byte.valueOf("10"));
        assertEquals(Byte.valueOf((byte) 15), Byte.valueOf("f", 16));
        assertEquals(Byte.valueOf((byte) 16), Byte.decode("0x10"));
        assertEquals(Byte.valueOf((byte) 8), Byte.decode("010"));
        assertEquals(Byte.valueOf((byte) -16), Byte.decode("-0X10"));
    }

    @Test public void stringEqualityHashAndComparisonWork() {
        Byte value = Byte.valueOf((byte) 12);
        assertEquals("12", value.toString());
        assertEquals("12", Byte.toString((byte) 12));
        assertEquals(value, Byte.valueOf((byte) 12));
        assertNotEquals(value, Byte.valueOf((byte) 13));
        assertEquals(12, value.hashCode());
        assertEquals(12, Byte.hashCode((byte) 12));
        assertTrue(value.compareTo(Byte.valueOf((byte) 13)) < 0);
        assertTrue(Byte.compare((byte) 13, (byte) 12) > 0);
        assertEquals(0, Byte.compare((byte) 12, (byte) 12));
    }

    @Test public void unsignedConversionsZeroExtend() {
        assertEquals(255, Byte.toUnsignedInt((byte) -1));
        assertEquals(255L, Byte.toUnsignedLong((byte) -1));
        assertEquals(128, Byte.toUnsignedInt(Byte.MIN_VALUE));
    }
}
