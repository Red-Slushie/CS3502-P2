# CS3502-P2
Report

Read Input Strings:
Reads two lines from strings.txt after opening it.
First line → s1 (main string)
Second line → s2 (substring to find)
Clears both strings of newline characters (\n)
Keeps lengths in n1 and n2 in memory.

**Create Threads:**
pthread_create(&threads[t], NULL, calSubStringThread, (void *) (size_t)t);
NUM_THREADS = 10 causes a loop to execute ten times.
By doing this, a thread is created and its ID (t) is sent to the thread function.

**Thread Execution:**
The function for thread execution is calSubStringThread(void *threadid)
Every thread gets its ID.
num_substring(tid) is called
The number of times s2 has been identified in the thread's designated portion of s1 is returned by this method.
The outcome is saved in countArray[tid].

**Counting Substring in a Thread:**
Every thread performs the following actions:
Divide the work - Ten equal pieces are taken from the entire string.
To prevent substring matches from being missed across chunks, the final portion of every chunk overlaps with the next by n2 -1 characters.
Look for Matches- Compare s1[i..i+n2] with s2 at each valid point i.
If every character is the same, increase the local count.
Local Store Count- count[tid] = local_count in an array

**Wait For Threads and Results:**
Returning to main():
Add up all of the values from countArray[] and output the total after all threads have finished (pthread_join()).


![CS3502-P2](https://github.com/user-a#define MAX 10240
#define NUM_THREADS 10ttachments/assets/7bf2de22-ff81-47ad-9892-36376134b2ee)
