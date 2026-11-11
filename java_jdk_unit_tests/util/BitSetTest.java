package jdk8.util;

import static org.junit.Assert.*;
import java.nio.*;
import java.util.*;
import org.junit.Test;

public class BitSetTest {
    @Test public void setClearFlipAndQueriesWork() {
        BitSet bits = new BitSet(); bits.set(1); bits.set(3,6); bits.flip(4); bits.clear(5);
        assertTrue(bits.get(1)); assertTrue(bits.get(3)); assertFalse(bits.get(4)); assertFalse(bits.get(5));
        assertEquals(2,bits.cardinality()); assertEquals(4,bits.length()); assertFalse(bits.isEmpty());
    }
    @Test public void navigationAndRangeOperationsWork() {
        BitSet bits = new BitSet(); bits.set(2); bits.set(5);
        assertEquals(2,bits.nextSetBit(0)); assertEquals(3,bits.nextClearBit(2));
        assertEquals(5,bits.previousSetBit(8)); assertEquals(4,bits.previousClearBit(5));
        assertEquals(bits.get(2,6),BitSet.valueOf(new long[]{36L}));
    }
    @Test public void logicalOperationsAndIntersectionWork() {
        BitSet a=BitSet.valueOf(new long[]{0b1010}); BitSet b=BitSet.valueOf(new long[]{0b1100});
        assertTrue(a.intersects(b)); BitSet c=(BitSet)a.clone(); c.and(b); assertEquals(BitSet.valueOf(new long[]{0b1000}),c);
        c=(BitSet)a.clone(); c.or(b); assertEquals(BitSet.valueOf(new long[]{0b1110}),c);
        c=(BitSet)a.clone(); c.xor(b); assertEquals(BitSet.valueOf(new long[]{0b0110}),c);
        c=(BitSet)a.clone(); c.andNot(b); assertEquals(BitSet.valueOf(new long[]{0b0010}),c);
    }
    @Test public void conversionsCloneStreamAndStringWork() {
        BitSet bits=BitSet.valueOf(new byte[]{5}); assertArrayEquals(new byte[]{5},bits.toByteArray());
        assertArrayEquals(new long[]{5L},bits.toLongArray()); assertEquals(Arrays.asList(0,2),bits.stream().collect(ArrayList<Integer>::new,List::add,List::addAll));
        BitSet copy=(BitSet)bits.clone(); assertEquals(bits,copy); assertNotSame(bits,copy); assertEquals("{0, 2}",bits.toString());
        assertEquals(bits,BitSet.valueOf(ByteBuffer.wrap(new byte[]{5}))); assertEquals(bits,BitSet.valueOf(LongBuffer.wrap(new long[]{5L})));
    }
}
