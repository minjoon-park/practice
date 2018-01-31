#ifndef SOLUTIONS_H
#define SOLUTIONS_H

#include <iostream>
#include <cstdint>
#include <math.h>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <map>
#include <queue>
#include <vector>
#include <algorithm>
#include <set>


using namespace std;

class P15 {
 public:
  long long Answer(int row, int column);
  long long AnswerDP(int row, int column);
};


class P54 {
 public:
  int Answer();

};


class P67 {
 public:
  int Answer();
};


class P81 {
 public:
  int Answer();

 private:
  int Min(int a, int b) {
    return (a > b ? b : a);
  }
};


class P443 {
 public:
  long long Answer(long long input);

 private:
  long long GCD(long long a, long long b);
};


/*class P262 {
 public:
  double Answer();

 private:
  double height(int x, int y) {
    return (5000 - 0.005 * (x * x + y * y + x * y) + 12.5 * (x + y))
            * exp(-abs(0.000001 * (x * x + y * y) - 0.0015 * (x + y) + 0.7));
  }

};
*/

class P230 {
 public:
  void Answer();

 private:
  string A_str, B_str;
  int size_A, size_B;

  long long Fibonacci(int n);
  char DAB(long long input);
};



class P243 {
 public:
  long long Answer();
};






class MinStepsToOne {
 public:
  int Answer(int val);

};



class N_Body {
 public:
  N_Body(double t_intv, int sim_duration);

  void LeapFrog();

 private:
  double t_intv, sph_size;
  int sim_duration, num_ptls;


};


class Genome {
 public:
  Genome(string seq_A, string seq_B);
  int GSA();

 private:
  string seq_A, seq_B;
};




class Disk {
 public:
  Disk(int id_val, int file_size);
  int id;
  vector<int> items;
  int remaining;
};

class CompareDisks {
 public:
  bool operator()(Disk& d1, Disk& d2) {
    if (d1.remaining < d2.remaining)
      return true;
    return false;
  }
};

class BinPacking {
 public:
  void PackBins();

 private:
  priority_queue<Disk, vector<Disk>, CompareDisks> bins;

};




class Vertex {
 public:
  Vertex(string actor_name);

  string name;
  char color;
  Vertex* parent;
  int dist;
};

class KevinBacon {
 public:
  ~KevinBacon();

  void BuildAdjListAndVertices();
  void BuildBFT(string root_name);
  void BFS(string name);

 private:
  map< string, map< string, set<string> > > adj_list;
  map<string, Vertex*> actor_name_map;
};




class City {
 public:
  City(int i);

  int id;
  City* parent;
  double d_upper_bnd;
};

class CityComp {
 public:
  bool operator()(City* i, City* j) {
    return (i->d_upper_bnd <= j->d_upper_bnd);
  }
};

class Routing {
 public:
  ~Routing();

  void BuildAdjListAndVertices();
  void Dijkstra(int source_city);
  void GetRoute(int i);

 private:
  int num_of_cities, num_of_roads, source, sink;
  map< int, pair<int, int> > city_loc;
  map< int, map<int, double> > adj_list;
  map<int, City*> id_city_map;

  bool Relax(int i, int j);
  void PrintRoute(int i );
};




/* Find the first character in a stream which is not repeated in the rest of the stream. Please note that you are being provided a stream as a source for these characters. The stream is guaranteed to eventually terminate (i.e. return false from a call to the hasNext() method), though it could be very long. You will access this stream through the provided interface methods. A call to hasNext() will return whether the stream contains any more characters to process. A call to getNext() will return the next character to be processed in the stream. It is not possible to restart the stream. Example: Input: aAbBABac Output: b In this example, the character ¡®b¡¯ is the first character in the stream which is not repeated in the stream.
*/



/* Every athlete is characterized by his mass 'm' (in kg) and strength 's' (in kg). You are to find the maximum number of athletes that can form a tower standing one upon another. An athlete can hold a tower of athletes with total mass equal to his strength or less than his strength. Input contains the number of athletes n and their parameters. These inputs can be assumed to be passed as arguments (Integer n and List> parameterList) appropriate for your language of choice.
*/

class Amazon2 {
 public:
  Amazon2();

  void Tower(vector< pair<int, int> >::iterator it);

  vector< pair<int, int> > athletes;
  int height, tot_mass, max_height;
};






/*
Problem 1.1
         *
         * Re-implement {@link AppleExercise.Problem1.SampleCache} as a singleton which implements the Cache interface.
         * Do not use any @SuppressWarnings annotations, and ensure that your implementation compiles without warnings
         * under Java 1.6.
         *
         * Name it {@link AppleExercise.Problem1.SingletonCache}
         *
         * Solution: A single-element enum implementation of a singleton
         *
         * Rational : This approach implements the singleton by taking advantage of Java's guarantee that any enum value is
         *            instantiated only once in a Java program.
         *
         * Pros: It uses a simple enumeration, and is therefore simple to implement.
         * 	     It has no special needs for serialization, i.e. no need to implement a readResolve() method.
         *       It is immune to reflection based attacks.
         *       Since Java enum values are globally accessible, so is the singleton.
         *
         * Cons: The drawback is that the enum type is somewhat inflexible; for example, it does not allow lazy initialization
         *       or subclassing.
         *
         * References : Joshua Bloch: Effective Java 2nd edition, ISBN 978-0-321-35668-0, 2008, p. 18



Problem 1.2
         *
         * The HashMap class backing the {@link AppleExercise.Problem1.SampleCache} class is not thread-safe.
         * Duplicate and modify your {@link AppleExercise.Problem1.SingletonCache} implementation to ensure
         * that both Cache.put and Cache.get methods are safe to be called by multi-threads.
         * Do not use any @SuppressWarnings annotations, and ensure that your implementation compiles
         * without warnings under Java 1.6.
         *
         * Name it {@link AppleExercise.Problem1.ThreadSafeCache}
         *
         * Solution: Leverage the {@code java.util.concurrent.ConcurrentHashMap}'s built-in Thread safety mechanisms
         *           by using that class as the underlying dataStore for the cache implementation. (See {@codeSingletonCache}
         *           for singleton implementation rational.)
         *
         * Rational: Achieving thread-safety without introducing performance bottlenecks is tricky and requires a deep
         *           understanding of the details of the Java Memory Model, and since these issues have been addressed
         *           in the {@code java.util.concurrent.ConcurrentHashMap} class, we will just leverage their solution.
         *
         * Pros: Provides true thread-safety, scalability, and reduced lock granularity.
         *       ConcurrentHashMap is designed  to optimize retrieval operations; "successful get() operations usually succeed with no
         *       locking at all." and since in a the typical cache workload retrievals are much more common than updates, a cache should
         *       be designed to offer very good get() performance.
         *
         * Cons: The memory footprint of a ConcurrentHashMap (at the instance level) is somewhat larger than a HashMap.
         *
         * References: Brian Goetz, "Java theory and practice: Concurrent collections classes",
         * 			   http://www.ibm.com/developerworks/java/library/j-jtp07233/index.html


Problem 1.3
         *
         * Use Java Generics to implement a non-singleton GenericCache class to improve compile-time type checking.
         *
         * Name it {@link AppleExercise.Problem1.GenericCache}
         *
         * Assumptions: The instructions infer that we can only use 1 class to implement this, so no helper classes, interfaces,
         *              or methods are allowed.
         *
         * Solution: A parameterized class that implements the methods defined in the Cache interface.
         *
         * Rational: Based on the requirements, this seems to be the simplest solution, so following the "Keep it simple" principle.
         *
         * Pros: Least complex solution, easiest to understand, maintain, etc.
         *
         * Cons: Doesn't explicitly implement the Cache interface defined above, since that interface isn't parameterized
         *       and we cannot override the interface method definitions in a sub-interface.



Problem 2.1
         *
         * Return an Iterator that iterates through of each String in the order that they are provided,
         * but skips {@code null} values.
         *
         * @param strings an array of nullable values
         * @return an iterator of strings
         *
         * Solution: Provide a thread-safe variant of Iterator that uses a reference to the state of the array at the point that the iterator
         * 			 was created, by making a fresh copy of the data passed in. This 'snapshot' array never changes during the lifetime of
         * 			 the {@code Iterator}, so interference is impossible and the iterator is guaranteed not to throw ConcurrentModificationException
         * 			 or reflect any changes to the original array during the course of it's traversal of the array.
         *
         * Rational: We want to mimic the fail-fast behavior of the {@code Iterator} interface, but are given an Array object, which
         *           is NOT part of the {@java.util.Collection} family, so we are still vulnerable to all mutative operations on the
         *           Array itself (even is the array is wrapped in a {@code Collections.synchronizedList}. Therefore, we need to make
         *           a 'snapshot' of the data as described above.
         *
         * Pros: Doesn't throw any Exception if underlying data is modified structurally while user is Iterating over it.
         * 		 No need to synchronize traversals, yet still thread-safe.
         *
         * Cons: Keeping a copy of the original contents increases memory usage.
         *       Mutations to the underlying Array are not reflected in the Iterator.
         *
         * Note: The iterator will not reflect additions, removals, or changes to the list since the iterator was created.
         *       Element-changing operations on iterator itself (remove, set, and add) are not supported and will throw UnsupportedOperationException.



Problem 2.2
         *
         * Return a single Iterator<String> that chains together the array of Iterator<String>s in the order
         * that they are provided, but skips {@code null} values.
         *
         * @param stringIterators an array of nullable {@code Iterator<String>}s of nullable {@code String}s
         * @return an {@link java.util.Iterator} of {@code String}s
         *
         * Solution: Provide a fail-fast variant of Iterator that keeps a list of the Iterators it is passed in. These iterators
         * 			 will be traversed, with the outer Iterator implementation delegating to the internal Iterators for basic Iterator
         * 			 methods (hasNext(), next(), and remove()). Some additional bookkeeping will be performed by the outer Iterator in
         * 			 order to traverse the list of internal Iterators, check for null String values inside the internal Iterators, etc.
         *
         * Rational: Since the Iterator<String> implementation is fail-fast, we should provide a fail-fast Iterator<String>
         * 			 as well since we are just combining the Iterators into a larger collection. Presumably, this method would
         * 			 be an alternative to Iterating each iterator individually within a for loop.
         *
         * Pros: Uses less memory since it does not make a 'snapshot' of the data.
         * 		 Conforms to the expected fail-fast behavior that would occur if the user had used the Iterators
         * 		 provided within a loop rather than using our method.
         *
         * Cons: Requires the user to synchronize traversals in order to be thread-safe as mutations to ANY of the passed in Iterators
         * 	     underlying collection will result in a ConcurrentModificationException.
         *
         * 		 Worse yet, mutations won't at the 'far end' of the array won't be detected until that particular Iterator is processed
         * 		 potentially delaying a fatal condition caused by a ConcurrentModificationException, resulting in lots of unnecessary work,
         * 		 by not failing fast enough.



Problem 2.3
         *
         * Return an Iterator that iterates through strings of each String[] in the order that they are
         * provided, but skips {@code null} values.
         *
         * @param stringArrays an array of nullable {@code String[}s of nullable {@code String}s
         * @return an {@link java.util.Iterator} of {@code String}s
         *
         * Solution: Provide a thread-safe variant of Iterator that uses a reference to the state of the array at the point that the iterator
         * 			 was created, by making a fresh copy of the data passed in. This 'snapshot' array never changes during the lifetime of
         * 			 the {@code Iterator}, so interference is impossible and the iterator is guaranteed not to throw ConcurrentModificationException
         * 			 or reflect any changes to the original array during the course of it's traversal of the array.
         *
         * Rational: We want to mimic the fail-fast behavior of the {@code Iterator} interface, but are given an Array object, which
         *           is NOT part of the {@java.util.Collection} family, so we are still vulnerable to all mutative operations on the
         *           Array itself (even is the array is wrapped in a {@code Collections.synchronizedList}. Therefore, we need to make
         *           a 'snapshot' of the data as described above.
         *
         * Pros: Doesn't throw any Exception if underlying data is modified structurally while user is Iterating over it.
         * 		 No need to synchronize traversals, yet still thread-safe.
         *
         * Cons: Keeping a copy of the original contents increases memory usage.
         *       Mutations to the underlying Array are not reflected in the Iterator.
         *
         * Note: The iterator will not reflect additions, removals, or changes to the list since the iterator was created.
         *       Element-changing operations on iterator itself (remove, set, and add) are not supported and will throw UnsupportedOperationException.
         *



Problem 2.4
         *
         * Return an Iterator that iterates through all Integers within each two-dimensional Integer array in
         * the order that they are provided, but skips {@code null} values.
         *
         * @param twoDimensionalIntArrays an array of nullable {@code Integer[][]}s of nullable values
         * @return an {@link java.util.Iterator} of {@code Integer}s
         *
         * Solution: Provide a thread-safe variant of Iterator that uses a reference to the state of the array at the point that the iterator
         * 			 was created, by making a fresh copy of the data passed in. This 'snapshot' array never changes during the lifetime of
         * 			 the {@code Iterator}, so interference is impossible and the iterator is guaranteed not to throw ConcurrentModificationException
         * 			 or reflect any changes to the original array during the course of it's traversal of the array.
         *
         * Rational: We want to mimic the fail-fast behavior of the {@code Iterator} interface, but are given an Array object, which
         *           is NOT part of the {@java.util.Collection} family, so we are still vulnerable to all mutative operations on the
         *           Array itself (even is the array is wrapped in a {@code Collections.synchronizedList}. Therefore, we need to make
         *           a 'snapshot' of the data as described above.
         *
         * Pros: Doesn't throw any Exception if underlying data is modified structurally while user is Iterating over it.
         * 		 No need to synchronize traversals, yet still thread-safe.
         *
         * Cons: Keeping a copy of the original contents increases memory usage.
         *       Mutations to the underlying Array are not reflected in the Iterator.
         *
         * Note: The iterator will not reflect additions, removals, or changes to the list since the iterator was created.
         *       Element-changing operations on iterator itself (remove, set, and add) are not supported and will throw UnsupportedOperationException.
         *







*/












#endif // SOLUTIONS_H
