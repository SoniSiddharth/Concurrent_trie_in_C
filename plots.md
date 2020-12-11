# Plots and Anaysis

## Read-write 50-50 workload

![alt text](images/read_write.PNG?raw=true)

**Size of trie** - 50000 words and word length = 12

---

![alt text](images/read_write2.PNG?raw=true)

**Size of trie** - 10000 words and word length = 12

- Hand over hand lock takes least time to complete the task followed by single mutex locks. 
- It is beacause in single locking the complete trie is blocked by a thread while reading or writing whereas in HOH lock only a node is not available for the other threads. 
- Threads can accesss the trie simultaneously and hence leading to faster completion of task.


## Read Intensive workload

![alt text](images/read_intensive.PNG?raw=true)

**Size of trie** - 50000 words and word length = 12

---

![alt text](images/read_intensive2.PNG?raw=true)

**Size of trie** - 10000 words and word length = 12

- Time consumed by single mutex lock is lesser than other locks. The time taken by a lock depends a lot upon the type of workload generated (in terms of words).
- The time difference between Hand over hand lock and single locking. Readers-writer lock takes clearly longer time because of starvation of writing threads.
- The maximum peak acheived by the rw lock is about 5-6 seconds with number of threads being 80 (approx).

## Write Intensive workload

![alt text](images/write_intensive.PNG?raw=true)

**Size of trie** - 50000 words and word length = 12

---

![alt text](images/write_intensive2.PNG?raw=true)

**Size of trie** - 10000 words and word length = 12

- Hand over hand locking again takes least time to complete the task.
- Maximum peak taken by the Readers-writer lock is crossing 11 seconds with number of threads being 80 (approx).
- It is greater than the Read Intensive workload timimg because this time writer threads were more which implies more starving and hence taking more time.
