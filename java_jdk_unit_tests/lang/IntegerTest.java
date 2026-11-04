package jdk8.lang;

import static org.junit.Assert.*;
import org.junit.Test;

public class IntegerTest {
    @Test public void constantsAreDocumented() {
        assertEquals(-2147483648, Integer.MIN_VALUE);
        assertEquals(2147483647, Integer.MAX_VALUE);
        assertEquals(32, Integer.SIZE);
        assertEquals(4, Integer.BYTES);
        assertEquals(int.class, Integer.TYPE);
    }

    @Test public void constructorsAndNumberConversionsWork() {
        Integer value = new Integer(258);
        assertEquals((byte) 2, value.byteValue());
        assertEquals((short) 258, value.shortValue());
        assertEquals(258, value.intValue());
        assertEquals(258L, value.longValue());
        assertEquals(258.0f, value.floatValue(), 0.0f);
        assertEquals(258.0d, value.doubleValue(), 0.0d);
        assertEquals(42, new Integer("42").intValue());
    }

    @Test public void parsingValueOfAndDecodeWork() {
        assertEquals(123, Integer.parseInt("123"));
        assertEquals(255, Integer.parseInt("ff", 16));
        assertEquals(-10, Integer.parseInt("-1010", 2));
        assertEquals(123, Integer.valueOf("123").intValue());
        assertEquals(255, Integer.valueOf("ff", 16).intValue());
        assertEquals(16, Integer.decode("0x10").intValue());
        assertEquals(8, Integer.decode("010").intValue());
        assertEquals(-16, Integer.decode("-#10").intValue());
    }

    @Test(expected = NumberFormatException.class)
    public void parseIntRejectsOverflow() { Integer.parseInt("2147483648"); }

    @Test public void unsignedParsingAndFormattingWork() {
        assertEquals(-1, Integer.parseUnsignedInt("4294967295"));
        assertEquals(-1, Integer.parseUnsignedInt("ffffffff", 16));
        assertEquals("4294967295", Integer.toUnsignedString(-1));
        assertEquals("ffffffff", Integer.toUnsignedString(-1, 16));
    }

    @Test public void formattingSupportsCommonRadices() {
        assertEquals("123", Integer.toString(123));
        assertEquals("ff", Integer.toString(255, 16));
        assertEquals("ff", Integer.toHexString(255));
        assertEquals("377", Integer.toOctalString(255));
        assertEquals("11111111", Integer.toBinaryString(255));
    }

    @Test public void equalityHashAndComparisonWork() {
        Integer value = Integer.valueOf(12);
        assertEquals("12", value.toString());
        assertEquals(value, Integer.valueOf(12));
        assertNotEquals(value, Integer.valueOf(13));
        assertEquals(12, value.hashCode());
        assertEquals(12, Integer.hashCode(12));
        assertTrue(value.compareTo(Integer.valueOf(13)) < 0);
        assertEquals(0, Integer.compare(12, 12));
        assertTrue(Integer.compareUnsigned(-1, 0) > 0);
    }

    @Test public void systemPropertyAccessUsesDefaultAndRadixPrefixes() {
        String key = "jdk8.lang.IntegerTest.number";
        String previous = System.getProperty(key);
        try {
            System.setProperty(key, "0x20");
            assertEquals(Integer.valueOf(32), Integer.getInteger(key));
            System.clearProperty(key);
            assertNull(Integer.getInteger(key));
            assertEquals(Integer.valueOf(7), Integer.getInteger(key, 7));
            assertEquals(Integer.valueOf(9), Integer.getInteger(key, Integer.valueOf(9)));
        } finally {
            if (previous == null) System.clearProperty(key); else System.setProperty(key, previous);
        }
    }

    @Test public void bitOperationsMatchThirtyTwoBitRepresentation() {
        assertEquals(32, Integer.numberOfLeadingZeros(0));
        assertEquals(31, Integer.numberOfLeadingZeros(1));
        assertEquals(32, Integer.numberOfTrailingZeros(0));
        assertEquals(3, Integer.numberOfTrailingZeros(8));
        assertEquals(8, Integer.highestOneBit(10));
        assertEquals(2, Integer.lowestOneBit(10));
        assertEquals(8, Integer.bitCount(0xF0F0));
        assertEquals(0x23456781, Integer.rotateLeft(0x12345678, 4));
        assertEquals(0x81234567, Integer.rotateRight(0x12345678, 4));
        assertEquals(0x78563412, Integer.reverseBytes(0x12345678));
        assertEquals(0x1E6A2C48, Integer.reverse(0x12345678));
        assertEquals(-1, Integer.signum(-100));
        assertEquals(0, Integer.signum(0));
        assertEquals(1, Integer.signum(100));
    }

    @Test public void unsignedArithmeticTreatsBitsAsUnsigned() {
        assertEquals(2147483647, Integer.divideUnsigned(-1, 2));
        assertEquals(1, Integer.remainderUnsigned(-1, 2));
        assertEquals(4294967295L, Integer.toUnsignedLong(-1));
    }

    @Test public void arithmeticHelpersWork() {
        assertEquals(20, Integer.sum(8, 12));
        assertEquals(12, Integer.max(8, 12));
        assertEquals(8, Integer.min(8, 12));
    }
}
