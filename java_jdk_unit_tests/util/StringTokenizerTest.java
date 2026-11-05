package jdk8.util;
import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;
public class StringTokenizerTest {
 @Test public void defaultWhitespaceTokenizationWorks()
 {
	 StringTokenizer t=new StringTokenizer(" one	two
three ");assertEquals(3,t.countTokens());assertTrue(t.hasMoreTokens());
assertEquals("one",t.nextToken());
assertEquals("two",t.nextToken());assertEquals("three",t.nextElement());assertFalse(t.hasMoreElements());}
 @Test public void customDelimitersCanBeReturned(){
	 StringTokenizer t=new StringTokenizer("a,b;c",",;",true);assertEquals("a",t.nextToken());assertEquals(",",t.nextToken());assertEquals("b",t.nextToken());assertEquals(";",t.nextToken());assertEquals("c",t.nextToken());}
 
 
@Test
public void nextTokenMayChangeDelimiters() {
    StringTokenizer t =
            new StringTokenizer("a,b;c", ",");

    assertEquals("a", t.nextToken());
    assertEquals(",b", t.nextToken(";"));
    assertEquals("c", t.nextToken());
}
	 
 @Test(expected=NoSuchElementException.class) public void exhaustionThrows(){StringTokenizer t=new StringTokenizer("");t.nextToken();}
}
