package jdk8.util;

import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;

public class RandomTest {
    @Test public void sameSeedProducesSameSequence() {
        Random a=new Random(12345L), b=new Random(12345L);
        assertEquals(a.nextInt(),b.nextInt()); assertEquals(a.nextLong(),b.nextLong());
        assertEquals(a.nextBoolean(),b.nextBoolean()); assertEquals(a.nextFloat(),b.nextFloat(),0.0f);
        assertEquals(a.nextDouble(),b.nextDouble(),0.0); assertEquals(a.nextGaussian(),b.nextGaussian(),0.0);
    }
    @Test public void boundedAndByteGenerationStayInContract() {
        Random r=new Random(1L); byte[] bytes=new byte[16]; r.nextBytes(bytes);
        assertEquals(16,bytes.length); for(int i=0;i<100;i++){int v=r.nextInt(7);assertTrue(v>=0&&v<7);}
    }
    @Test public void setSeedResetsSequence() {
        Random r=new Random(9L); int first=r.nextInt(); r.nextInt(); r.setSeed(9L); assertEquals(first,r.nextInt());
    }
    @Test public void java8StreamsRespectSizeAndBounds() {
        Random r=new Random(1L);
        assertEquals(5,r.ints(5).count()); assertTrue(r.ints(100,3,8).allMatch(v->v>=3&&v<8));
        assertEquals(5,r.longs(5).count()); assertTrue(r.longs(100,3,8).allMatch(v->v>=3&&v<8));
        assertEquals(5,r.doubles(5).count()); assertTrue(r.doubles(100,3.0,8.0).allMatch(v->v>=3.0&&v<8.0));
    }
    @Test(expected = IllegalArgumentException.class) public void nextIntRejectsNonPositiveBound(){new Random().nextInt(0);}
}
