package jdk8.lang;

import static org.junit.Assert.*;
import org.junit.Test;

public class LongTest {
    @Test public void constantsAreDocumented() {
        assertEquals(Long.MIN_VALUE, -9223372036854775807L - 1L);
        assertEquals(Long.MAX_VALUE, 9223372036854775807L);
        assertEquals(64, Long.SIZE);
        assertEquals(8, Long.BYTES);
        assertEquals(long.class, Long.TYPE);
    }

    @Test public void constructorsAndNumberConversionsWork() {
        Long value = new Long(258L);
        assertEquals((byte) 2, value.byteValue());
        assertEquals((short) 258, value.shortValue());
        assertEquals(258, value.intValue());
        assertEquals(258L, value.longValue());
        assertEquals(258.0f, value.floatValue(), 0.0f);
        assertEquals(258.0d, value.doubleValue(), 0.0d);
        assertEquals(42L, new Long("42").longValue());
    }

    @Test public void parsingValueOfAndDecodeWork() {
        assertEquals(123L, Long.parseLong("123"));
        assertEquals(255L, Long.parseLong("ff", 16));
        assertEquals(-10L, Long.parseLong("-1010", 2));
        assertEquals(123L, Long.valueOf("123").longValue());
        assertEquals(255L, Long.valueOf("ff", 16).longValue());
        assertEquals(16L, Long.decode("0x10").longValue());
        assertEquals(8L, Long.decode("010").longValue());
    }

    @Test(expected = NumberFormatException.class)
    public void parseLongRejectsOverflow() { Long.parseLong("9223372036854775808"); }

    @Test public void unsignedParsingAndFormattingWork() {
        assertEquals(-1L, Long.parseUnsignedLong("18446744073709551615"));
        assertEquals(-1L, Long.parseUnsignedLong("ffffffffffffffff", 16));
        assertEquals("18446744073709551615", Long.toUnsignedString(-1L));
        assertEquals("ffffffffffffffff", Long.toUnsignedString(-1L, 16));
    }

    @Test public void formattingSupportsCommonRadices() {
        assertEquals("123", Long.toString(123L));
        assertEquals("ff", Long.toString(255L, 16));
        assertEquals("ff", Long.toHexString(255L));
        assertEquals("377", Long.toOctalString(255L));
        assertEquals("11111111", Long.toBinaryString(255L));
    }

    @Test public void equalityHashAndComparisonWork() {
        Long value = Long.valueOf(12L);
        assertEquals("12", value.toString());
        assertEquals(value, Long.valueOf(12L));
        assertNotEquals(value, Long.valueOf(13L));
        assertEquals(12, value.hashCode());
        assertEquals(12, Long.hashCode(12L));
        assertTrue(value.compareTo(Long.valueOf(13L)) < 0);
        assertEquals(0, Long.compare(12L, 12L));
        assertTrue(Long.compareUnsigned(-1L, 0L) > 0);
    }

    @Test public void systemPropertyAccessUsesDefault() {
        String key = "jdk8.lang.LongTest.number";
        String previous = System.getProperty(key);
        try {
            System.setProperty(key, "0x20");
            assertEquals(Long.valueOf(32L), Long.getLong(key));
            System.clearProperty(key);
            assertNull(Long.getLong(key));
            assertEquals(Long.valueOf(7L), Long.getLong(key, 7L));
            assertEquals(Long.valueOf(9L), Long.getLong(key, Long.valueOf(9L)));
        } finally {
            if (previous == null) System.clearProperty(key); else System.setProperty(key, previous);
        }
    }

    @Test public void bitOperationsMatchSixtyFourBitRepresentation() {
        assertEquals(64, Long.numberOfLeadingZeros(0L));
        assertEquals(63, Long.numberOfLeadingZeros(1L));
        assertEquals(64, Long.numberOfTrailingZeros(0L));
        assertEquals(3, Long.numberOfTrailingZeros(8L));
        assertEquals(8L, Long.highestOneBit(10L));
        assertEquals(2L, Long.lowestOneBit(10L));
        assertEquals(8, Long.bitCount(0xF0F0L));
        assertEquals(0x23456789ABCDEF01L, Long.rotateLeft(0x123456789ABCDEF0L, 4));
        assertEquals(0x0123456789ABCDEFL, Long.rotateRight(0x123456789ABCDEF0L, 4));
        assertEquals(0x0807060504030201L, Long.reverseBytes(0x0102030405060708L));
        assertEquals(-1, Long.signum(-100L));
        assertEquals(0, Long.signum(0L));
        assertEquals(1, Long.signum(100L));
    }

    @Test public void unsignedArithmeticTreatsBitsAsUnsigned() {
        assertEquals(Long.MAX_VALUE, Long.divideUnsigned(-1L, 2L));
        assertEquals(1L, Long.remainderUnsigned(-1L, 2L));
    }

    @Test public void arithmeticHelpersWork() {
        assertEquals(20L, Long.sum(8L, 12L));
        assertEquals(12L, Long.max(8L, 12L));
        assertEquals(8L, Long.min(8L, 12L));
    }
}
