public class SynchronizedExample {

    private int counter = 0;

    /*
     * The entire method is synchronized.
     *
     * A thread must acquire this object's monitor before entering
     * the method. The monitor is released when the method returns
     * or throws an exception.
     */
    public synchronized void increment() {
        counter++;
    }

    /*
     * Only the section that modifies shared state is synchronized.
     *
     * The validation and other non-shared work occur without holding
     * the object's monitor.
     */
    public void add(int amount) {
        if (amount <= 0) {
            return;
        }

        System.out.println(
                Thread.currentThread().getName()
                        + " is adding "
                        + amount);

        synchronized (this) {
            counter += amount;
        }
    }

    /*
     * Synchronize the read so that its value is visible across threads.
     */
    public synchronized int getCounter() {
        return counter;
    }

    public static void main(String[] args) throws InterruptedException {
        final SynchronizedExample example = new SynchronizedExample();

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

        System.out.println("Final counter: " + example.getCounter());
    }
}