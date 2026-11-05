package jdk8.util;
import static org.junit.Assert.*;
import java.util.*;
import java.util.concurrent.atomic.AtomicBoolean;
import org.junit.Test;
public class OptionalTest {
 @Test public void creationPresenceAndAccessWork(){
	 Optional<String>e=Optional.empty();
	 assertFalse(e.isPresent());
	 Optional<String>p=Optional.of("x");
	 assertTrue(p.isPresent());
	 assertEquals("x",p.get());assertFalse(Optional.ofNullable(null).isPresent());assertEquals(Optional.of("x"),Optional.ofNullable("x"));}
 @Test(expected=NullPointerException.class) public void ofRejectsNull(){Optional.of(null);}
 @Test(expected=NoSuchElementException.class) public void getRejectsEmpty(){Optional.empty().get();}
 @Test public void conditionalAndTransformingOperationsWork(){Optional<String>p=Optional.of("abc");assertEquals(Optional.of("abc"),p.filter(v->v.length()==3));assertEquals(Optional.empty(),p.filter(v->false));assertEquals(Optional.of(3),p.map(String::length));assertEquals(Optional.of("ABC"),p.flatMap(v->Optional.of(v.toUpperCase())));AtomicBoolean called=new AtomicBoolean(false);p.ifPresent(v->called.set(true));assertTrue(called.get());}
 @Test public void fallbackOperationsWork(){Optional<String>e=Optional.empty();assertEquals("d",e.orElse("d"));assertEquals("s",e.orElseGet(()->"s"));assertEquals("x",Optional.of("x").orElse("d"));}
 @Test(expected=IllegalStateException.class) public void orElseThrowUsesSupplier(){Optional.empty().orElseThrow(()->new IllegalStateException("missing"));}
 @Test public void equalityHashAndStringWork(){assertEquals(Optional.empty(),Optional.empty());assertEquals(Optional.of("x"),Optional.of("x"));assertNotEquals(Optional.of("x"),Optional.of("y"));assertEquals("x".hashCode(),Optional.of("x").hashCode());assertEquals(0,Optional.empty().hashCode());assertTrue(Optional.of("x").toString().contains("x"));}
}
