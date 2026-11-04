import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class SynchronizedLockExampleTest {

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
