package jdk8.util;

import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;

public class UUIDTest {
    @Test public void constructorAndBitAccessorsWork() {
        UUID id=new UUID(0x123456789abcdef0L,0x0fedcba987654321L);
        assertEquals(0x123456789abcdef0L,id.getMostSignificantBits()); assertEquals(0x0fedcba987654321L,id.getLeastSignificantBits());
    }
    @Test public void parsingFormattingEqualityAndOrderingWork() {
        String text="123e4567-e89b-12d3-a456-426655440000"; UUID id=UUID.fromString(text);
        assertEquals(text,id.toString()); assertEquals(id,UUID.fromString(text)); assertEquals(id.hashCode(),UUID.fromString(text).hashCode());
        assertNotEquals(id,new UUID(0L,0L)); assertTrue(new UUID(0L,1L).compareTo(new UUID(0L,2L))<0);
    }
    @Test public void versionVariantAndVersionSpecificFieldsWork() {
        UUID id=UUID.fromString("123e4567-e89b-12d3-a456-426655440000");
        assertEquals(1,id.version()); assertEquals(2,id.variant()); assertTrue(id.timestamp()>0L);
        assertTrue(id.clockSequence()>=0); assertTrue(id.node()>=0L);
    }
    @Test public void nameAndRandomFactoriesSetExpectedVersions() throws Exception {
        assertEquals(3,UUID.nameUUIDFromBytes("name".getBytes("UTF-8")).version());
        UUID random=UUID.randomUUID(); assertEquals(4,random.version()); assertEquals(2,random.variant());
    }
    @Test(expected = IllegalArgumentException.class) public void parsingRejectsMalformedText(){UUID.fromString("bad");}
}
