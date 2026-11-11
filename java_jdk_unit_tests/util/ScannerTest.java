package jdk8.util;

import static org.junit.Assert.*;
import java.io.*;
import java.math.*;
import java.util.*;
import java.util.regex.Pattern;
import org.junit.Test;

public class ScannerTest {
    @Test public void tokenAndPrimitiveScanningWork() {
        Scanner s=new Scanner("word true 12 34 5.5");
        assertTrue(s.hasNext()); assertEquals("word",s.next()); assertTrue(s.nextBoolean());
        assertEquals((byte)12,s.nextByte()); assertEquals((short)34,s.nextShort()); assertEquals(5.5,s.nextDouble(),0.0); assertFalse(s.hasNext()); s.close();
    }
    @Test public void integerLongFloatBigNumberAndRadixScanningWork() {
        Scanner s=new Scanner("ff 12345678900 1.25 12345678901234567890 12.50"); s.useRadix(16);
        assertEquals(255,s.nextInt()); s.useRadix(10); assertEquals(12345678900L,s.nextLong()); assertEquals(1.25f,s.nextFloat(),0.0f);
        assertEquals(new BigInteger("12345678901234567890"),s.nextBigInteger()); assertEquals(new BigDecimal("12.50"),s.nextBigDecimal());
    }
    @Test public void delimiterLineAndMatchOperationsWork() {
        Scanner s=new Scanner("a,b,c\nline two").useDelimiter("[,\\n]");
        assertEquals("a",s.next()); assertEquals("b",s.next()); assertEquals("c",s.next()); assertEquals("line two",s.next());
        Scanner lines=new Scanner("first\nsecond"); assertTrue(lines.hasNextLine()); assertEquals("first",lines.nextLine()); assertEquals("second",lines.nextLine());
    }
    @Test public void findSkipLocaleAndResetWork() {
        Scanner s=new Scanner("abc 123 xyz"); assertEquals("123",s.findWithinHorizon("\\d+",0)); assertNotNull(s.match());
        s=new Scanner("abc123"); s.skip(Pattern.compile("abc")); assertEquals(123,s.nextInt());
        s.useLocale(Locale.US).useRadix(16).useDelimiter(","); assertEquals(16,s.radix()); assertEquals(Locale.US,s.locale()); assertEquals(",",s.delimiter().pattern());
        s.reset(); assertEquals(10,s.radix()); assertEquals("\\p{javaWhitespace}+",s.delimiter().pattern());
    }
    @Test public void ioExceptionAndCloseStateAreReported() {
        Scanner s=new Scanner(new Readable(){public int read(java.nio.CharBuffer cb)throws IOException{throw new IOException("failure");}});
        assertFalse(s.hasNext()); assertNotNull(s.ioException()); s.close();
    }
    @Test(expected = InputMismatchException.class) public void wrongPrimitiveTypeThrows(){new Scanner("word").nextInt();}
    @Test(expected = IllegalStateException.class) public void operationsAfterCloseThrow(){Scanner s=new Scanner("x");s.close();s.next();}
}
