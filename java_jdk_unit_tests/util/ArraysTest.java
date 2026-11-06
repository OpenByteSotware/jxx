package jdk8.util;
import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;
public class ArraysTest {
 @Test public void sortAndParallelSortOrderValues(){int[]a={3,1,2};
 Arrays.sort(a);
 assertArrayEquals(new int[]{1,2,3},a);int[]b={5,4,3,2,1};
 Arrays.parallelSort(b,1,4);
 assertArrayEquals(new int[]{5,2,3,4,1},b);
 String[]s={"b","a","c"};
 Arrays.sort(s,Collections.reverseOrder());
 assertArrayEquals(new String[]{"c","b","a"},s);}
 
 @Test public void binarySearchFindsValuesAndInsertionPoints(){int[]a={1,3,5,7};assertEquals(2,Arrays.binarySearch(a,5));assertEquals(-3,Arrays.binarySearch(a,4));assertEquals(1,Arrays.binarySearch(a,1,3,3));}
 
 @Test public void equalityDeepEqualityAndHashingWork(){int[]a={1,2};int[]b={1,2};assertTrue(Arrays.equals(a,b));assertEquals(Arrays.hashCode(a),Arrays.hashCode(b));Object[]x={new int[]{1,2},new String[]{"a"}};Object[]y={new int[]{1,2},new String[]{"a"}};assertTrue(Arrays.deepEquals(x,y));assertEquals(Arrays.deepHashCode(x),Arrays.deepHashCode(y));}
 
 @Test public void fillCopyAndRangeCopyWork(){int[]a=new int[4];Arrays.fill(a,7);assertArrayEquals(new int[]{7,7,7,7},a);Arrays.fill(a,1,3,9);assertArrayEquals(new int[]{7,9,9,7},a);assertArrayEquals(new int[]{7,9,9,7,0,0},Arrays.copyOf(a,6));assertArrayEquals(new int[]{9,9},Arrays.copyOfRange(a,1,3));}
 
 @Test public void listAndStringRepresentationsWork(){String[]a={"a","b"};List<String>list=Arrays.asList(a);list.set(0,"x");assertEquals("x",a[0]);assertEquals("[1, 2]",Arrays.toString(new int[]{1,2}));assertEquals("[[1, 2]]",Arrays.deepToString(new Object[]{new int[]{1,2}}));}
 
 @Test public void setAllParallelSetAllAndPrefixWork(){int[]a=new int[4];Arrays.setAll(a,i->i*2);assertArrayEquals(new int[]{0,2,4,6},a);Arrays.parallelSetAll(a,i->i+1);assertArrayEquals(new int[]{1,2,3,4},a);Arrays.parallelPrefix(a,(x,y)->x+y);assertArrayEquals(new int[]{1,3,6,10},a);}
 
 @Test public void streamsCoverFullAndPartialRanges(){assertEquals(6,Arrays.stream(new int[]{1,2,3}).sum());assertEquals(5,Arrays.stream(new int[]{1,2,3,4},1,3).sum());assertNotNull(Arrays.spliterator(new String[]{"a"}));}
}
