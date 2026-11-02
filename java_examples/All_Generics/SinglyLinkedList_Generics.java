package All_Generics;

import java.util.Objects;

public class SinglyLinkedList_Generics<T> {

    private long size;
    private Node<T> head;
    private Node<T> tail;

    public void add1(T value) {
        addFirst(new Node<>(value));
    }

    public void add2(T value) {
        addLast(new Node<>(value));
    }

    private void addFirst(Node<T> node) {
        node.setNext(head);
        head = node;
        size++;

        if (size == 1) {
            tail = head;
        }
    }

    private void addLast(Node<T> node) {
        node.setNext(null);

        if (size == 0) {
            head = node;
        } else {
            tail.setNext(node);
        }

        tail = node;
        size++;
    }

    public T getHead() {
        return head == null ? null : head.getValue();
    }

    public T getTail() {
        return tail == null ? null : tail.getValue();
    }

    public void remove1() {
        if (size == 0) {
            return;
        }

        Node<T> oldHead = head;
        head = head.getNext();
        oldHead.setNext(null);
        size--;

        if (size == 0) {
            tail = null;
        }
    }

    public void remove2() {
        if (size == 0) {
            return;
        }

        if (size == 1) {
            head = null;
            tail = null;
            size = 0;
            return;
        }

        Node<T> current = head;

        while (current.getNext() != tail) {
            current = current.getNext();
        }

        current.setNext(null);
        tail = current;
        size--;
    }

    public T get1() {
        return getHead();
    }

    /*
     * Four scenarios:
     * 1. Empty list: do nothing.
     * 2. Single node: previous is null.
     * 3. Multiple nodes:
     *    a. The node to remove is the first node.
     *    b. The node to remove is in the middle or at the end.
     */
    public boolean remove(T value) {
        Node<T> previous = null;
        Node<T> current = head;

        while (current != null) {
            if (Objects.equals(current.getValue(), value)) {
                if (previous == null) {
                    remove1();
                } else {
                    previous.setNext(current.getNext());

                    if (current == tail) {
                        tail = previous;
                    }

                    current.setNext(null);
                    size--;
                }

                return true;
            }

            previous = current;
            current = current.getNext();
        }

        return false;
    }

    public long getSize() {
        return size;
    }

    public void print() {
        System.out.print("Total elements: " + size + " -> ");

        Node<T> current = head;

        while (current != null) {
            System.out.print(String.valueOf(current.getValue()) + " ");
            current = current.getNext();
        }

        System.out.println();
    }

    public void clear() {
        Node<T> current = head;

        while (current != null) {
            Node<T> next = current.getNext();
            current.setNext(null);
            current.setValue(null);
            current = next;
        }

        head = null;
        tail = null;
        size = 0;
    }

    private static class Node<E> {
        private E value;
        private Node<E> next;

        Node(E value) {
            this.value = value;
        }

        E getValue() {
            return value;
        }

        void setValue(E value) {
            this.value = value;
        }

        Node<E> getNext() {
            return next;
        }

        void setNext(Node<E> next) {
            this.next = next;
        }
    }
}

class LinkedListDemo {

    public static void main(String[] args) {
        SinglyLinkedList_Generics<Integer> integers =
                new SinglyLinkedList_Generics<>();

        integers.add1(4);
        integers.add1(3);
        integers.add1(2);
        integers.add1(1);

        integers.print();

        System.out.println("\nRemove first and last elements:");
        integers.remove1();
        integers.remove2();

        integers.print();

        System.out.println("\nAdd elements at the end:");
        integers.add2(5);
        integers.add2(6);
        integers.add2(7);

        integers.print();

        SinglyLinkedList_Generics<String> strings =
                new SinglyLinkedList_Generics<>();

        strings.add1("171-15-8737");
        strings.add1("\tDIU ID -");
        strings.add1("Kabir");
        strings.add1("Sharafat");
        strings.add1("Redwan");

        System.out.println("\nAdd String elements:");

        strings.print();
    }
}