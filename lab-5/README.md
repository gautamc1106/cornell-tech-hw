# Lab 5

## Monitors

We have learned about synchronization using condition variables and locks in class.  While there are many creative ways to use condition variables and locks to produce correct, concurrent code.  However, reasoning about concurrency is hard, and it can get very confusing consumers of your code if they have never seen your particular brand of synchronization.
In order to prevent such confusion, many in the industry have adopted the use of monitors.  A monitor is a thread-safe class that guarantees mutual exclusion while executing any of its methods.  Only one thread may be executing any of the code in a monitor at any given time.  This construct makes it much easier to make sense of concurrent code.
Regarding implementation of monitors, it is easy to see that due to the mutual exclusion requirement, a monitor should have at most one lock.  However, it is often useful to share that lock amongst multiple condition variables.  For example, you may have one lock which guarantees mutual exclusion throughout the object’s methods, but there may be various conditions that could potentially allow a waiting thread to progress.  For each of those conditions, you should construct a condition variable, and they should all share the same lock.

More on monitors can be found at https://en.wikipedia.org/wiki/Monitor_(synchronization).

The lab
For this lab, you will use a threading library for arduino which has support for locks and condition variables.  You will implement the solutions to a couple of problems which require you to write monitors using condition variables and locks, and answer a few questions.
You will note that the problems and the threading library are written in C++.  In fact, the Arduino language is C++ so you’ve been using the language the entire course.  If you haven’t used C++ outside of Arduino platform before, you may want to read up on the basics of classes in C++.  You probably won’t need to though, because the basic class constructs are already there, you need only fill in the methods.
The interface
The code to use the threading library is already in the provided skeleton code.  In order to make a thread, you simply make a object that inherits from Process, and then implement the loop() method.  Each process will repeatedly execute its loop method when it is running, just like the global loop method in Arduino.  To start threads, simply construct an instance of the class.


Following is some information on the condition variables and locks in the threading library:
At the top of the class, you can declare a local lock and condition variable like this:
Lock _l;
Cond _c;
Then inside the class constructor, you must initialize the lock and condition variable like this:
_l = Lock();
_c = Cond(_l);

Note above that the condition variable constructor takes a lock as an argument.  You may declare another condition variable that shares the lock by adding this to the top of the class:

Cond _c2;

And then initializing it with the same lock in the class constructor:
_c2 = Cond(_l);

In order to lock and unlock the lock, you can call:
_l.lock();
_l.unlock();

As a shortcut, you can lock and unlock the lock associated with a condition variable like this:
_c.lock();
_c.unlock();

To wait on condition variable _c:
_c.wait();

To signal the condition variable and potentially wake up waiting threads:
_c.signal();

And to check if any thread is waiting on _c:
_c.waiting(); //returns true if there is anyone waiting

Installing the library

You need to take the Proc folder and put it where your arduino libraries folder is.  On Windows, it’s in Documents/Arduino/libraries.  For other platforms, you will have to read the Arduino documentation on libraries.
