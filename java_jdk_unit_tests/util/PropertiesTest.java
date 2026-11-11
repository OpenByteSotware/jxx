package jdk8.util;

import static org.junit.Assert.*;
import java.io.*;
import java.util.*;
import org.junit.Test;

public class PropertiesTest {
    @Test public void defaultsAndStringPropertiesWork() {
        Properties defaults=new Properties(); defaults.setProperty("default","d"); Properties p=new Properties(defaults);
        assertNull(p.setProperty("key","value")); assertEquals("value",p.getProperty("key"));
        assertEquals("d",p.getProperty("default")); assertEquals("fallback",p.getProperty("missing","fallback"));
        assertTrue(p.stringPropertyNames().containsAll(Arrays.asList("key","default")));
    }
    @Test public void loadAndStoreTextRoundTrip() throws Exception {
        Properties p=new Properties(); p.setProperty("a","1"); p.setProperty("space key","hello world");
        ByteArrayOutputStream out=new ByteArrayOutputStream(); p.store(out,"test");
        Properties loaded=new Properties(); loaded.load(new ByteArrayInputStream(out.toByteArray()));
        assertEquals("1",loaded.getProperty("a")); assertEquals("hello world",loaded.getProperty("space key"));
    }
    @Test public void readerWriterAndXmlRoundTripsWork() throws Exception {
        Properties p=new Properties(); p.setProperty("unicode","caf\u00e9");
        StringWriter writer=new StringWriter(); p.store(writer,"test"); Properties text=new Properties(); text.load(new StringReader(writer.toString()));
        assertEquals("caf\u00e9",text.getProperty("unicode"));
        ByteArrayOutputStream xml=new ByteArrayOutputStream(); p.storeToXML(xml,"test","UTF-8");
        Properties loaded=new Properties(); loaded.loadFromXML(new ByteArrayInputStream(xml.toByteArray())); assertEquals(p,loaded);
    }
    @Test public void enumerationAndListingWork() throws Exception {
        Properties p=new Properties(); p.setProperty("a","1"); assertTrue(p.propertyNames().hasMoreElements());
        ByteArrayOutputStream bytes=new ByteArrayOutputStream(); p.list(new PrintStream(bytes,"UTF-8"));
        assertTrue(bytes.toString("UTF-8").contains("a=1"));
    }
}
