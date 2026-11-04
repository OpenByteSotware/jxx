import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class SynchronizedLockExampleTest {

    /*
     * Class under test, contained entirely within this test file.
     */
    private static class SynchronizedLockExample {

        private int counter = 0;
        private final Object counterLock = new Object();

        public void increment() {
            synchronized (counterLock) {
                counter++;
            }
        }

        public void add(int amount) {
            if (amount <= 0) {
                return;
            }

            synchronized (counterLock) {
                counter += amount;
            }
        }

        public int getCounter() {
            synchronized (counterLock) {
                return counter;
            }
        }
    }

    @Test
    public void twoThreadsShouldProduceCountOf2020()
            throws InterruptedException {

        final SynchronizedLockExample example =
                new SynchronizedLockExample();

        Runnable task = new Runnable() {
            @Override
            public void run() {
                for (int i = 0; i < 1000; i++) {
                    example.increment();
                }

                example.add(10);
            }
        };

        Thread firstThread = new Thread(task, "FirstThread");
        Thread secondThread = new Thread(task, "SecondThread");

        firstThread.start();
        secondThread.start();

        firstThread.join();
        secondThread.join();

        assertEquals(2020, example.getCounter());
    }
}
