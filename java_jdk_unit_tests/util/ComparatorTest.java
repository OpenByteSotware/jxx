package jdk8.util;

import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;

public class ComparatorTest {
    private static class Person { final String name; final int age; Person(String n,int a){name=n;age=a;} }
    @Test public void comparingAndThenComparingWork() {
        Comparator<Person> comparator = Comparator.comparing((Person p)->p.name).thenComparingInt(p->p.age);
        assertTrue(comparator.compare(new Person("a",2), new Person("b",1)) < 0);
        assertTrue(comparator.compare(new Person("a",1), new Person("a",2)) < 0);
    }
    @Test public void primitiveComparatorsAndReversalWork() {
        Comparator<Person> byAge = Comparator.comparingInt(p->p.age);
        assertTrue(byAge.compare(new Person("a",1),new Person("b",2))<0);
        assertTrue(byAge.reversed().compare(new Person("a",1),new Person("b",2))>0);
        assertTrue(Comparator.comparingLong((Person p)->p.age).compare(new Person("a",1),new Person("b",2))<0);
        assertTrue(Comparator.comparingDouble((Person p)->p.age).compare(new Person("a",1),new Person("b",2))<0);
    }
    @Test public void naturalReverseAndNullComparatorsWork() {
        assertTrue(Comparator.<Integer>naturalOrder().compare(1,2)<0);
        assertTrue(Comparator.<Integer>reverseOrder().compare(1,2)>0);
        assertTrue(Comparator.nullsFirst(Comparator.<Integer>naturalOrder()).compare(null,1)<0);
        assertTrue(Comparator.nullsLast(Comparator.<Integer>naturalOrder()).compare(null,1)>0);
    }
}
