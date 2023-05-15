package com.example;

public class HelloWorld {
    public static void main(String[] args) {
        System.out.println("Hello, world!");
        /* While loop to busy wait */
        while (true) {
            /* Sleep */
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
