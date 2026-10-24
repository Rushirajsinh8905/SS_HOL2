# EXERCISE NOTES — SS_HOL2
**Author:** Dabhi Rushirajsinh | **Course:** EGC 301P OS Lab | **Date:** 23rd Oct, 2026

---

## Exercise 1 — Pipe Creation and Communication

**Aim:** Demonstrate creating a pipe, writing to it, reading from it, and displaying the content.

**Concept:** A pipe is a unidirectional IPC channel in the kernel. `pipe(fd)` returns two file descriptors: `fd[0]` (read end) and `fd[1]` (write end). Data written to `fd[1]` can be read from `fd[0]`.

**Algorithm:**
1. Call `pipe(fd)` to create the pipe
2. Write a message to `fd[1]`
3. Close `fd[1]` (signal EOF to reader)
4. Read from `fd[0]`
5. Print the message

**Key System Calls:**
- `pipe(int fd[2])` — Creates pipe; returns 0 on success, -1 on error
- `write(fd[1], buf, len)` — Write to write-end
- `read(fd[0], buf, len)` — Read from read-end; returns bytes read

**Execution:** `gcc 1.c -o 1 && ./1`

**Expected Output:** `Message read from pipe: Hello from the pipe!`

**Common Mistakes:** Forgetting to close unused pipe ends; not null-terminating the read buffer.

**Viva Questions:**
1. *What is a pipe?* → A kernel buffer with two FDs: read-end and write-end. One-directional, between related processes.
2. *What happens if you don't close the write-end before reading?* → `read()` blocks indefinitely because the kernel sees no EOF.
3. *What is the default pipe buffer size on Linux?* → 65536 bytes (64 KB) since kernel 2.6.11.

**Demo tips:** Show `pipe(fd)` output, write a string, read it back, and print it.

---

## Exercise 2 — Data Transmission from Parent to Child

**Aim:** Send data from parent process to child process using a pipe.

**Concept:** After `fork()`, both parent and child inherit pipe file descriptors. Parent closes `fd[0]` (read) and writes; child closes `fd[1]` (write) and reads.

**Algorithm:**
1. `pipe(fd)` → Create pipe
2. `fork()` → Create child
3. Parent: close `fd[0]`, write message, close `fd[1]`
4. Child: close `fd[1]`, read message, print it

**Key System Calls:** `pipe()`, `fork()`, `write()`, `read()`, `wait()`

**Key Distinction:** `fork()` duplicates ALL open file descriptors. Both ends of the pipe exist in both processes — you must close the unused end in each process.

**Execution:** `gcc 2.c -o 2 && ./2`

**Expected Output:** `Child received from parent: Hello from Parent!`

**Common Mistakes:** Not closing the unused end → reader never sees EOF → blocks forever.

**Viva Questions:**
1. *Why must the parent close fd[0]?* → Parent doesn't read; keeping it open wastes a FD and is confusing.
2. *What happens if parent doesn't close fd[1] after writing?* → Child's read blocks — it never sees EOF.
3. *What does wait(NULL) do?* → Parent blocks until the child terminates, preventing zombie processes.

---

## Exercise 3 — Two-Way Communication

**Aim:** Bidirectional data exchange between parent and child using two pipes.

**Concept:** A single pipe is one-directional. For two-way communication, create two pipes: `fd1` (parent→child) and `fd2` (child→parent). Each process closes the ends it doesn't use.

**Algorithm:**
1. Create two pipes: `fd1` and `fd2`
2. `fork()` → Create child
3. Parent: write to `fd1[1]`, read from `fd2[0]`
4. Child: read from `fd1[0]`, write to `fd2[1]`

**Key Distinction:** Two pipes needed because pipes are strictly one-directional.

**Execution:** `gcc 3.c -o 3 && ./3`

**Expected Output:**
```
Child received: Hello Child, how are you?
Parent received: I am fine, Parent!
```

**Viva Questions:**
1. *Why can't one pipe handle two-way communication?* → A pipe has one write-end and one read-end; data flows in only one direction.
2. *What does fork() return?* → In parent: child PID (> 0). In child: 0. On error: -1.
3. *Difference between wait() and waitpid()?* → `wait()` waits for any child; `waitpid()` waits for a specific child PID.

---

## Exercise 4 — Executing `ls -l | wc` using dup/dup2/fcntl

**Aim:** Redirect stdout/stdin using dup, dup2, and fcntl to implement shell-level pipe behavior.

**Concept:** Shell pipes redirect stdout of one process to stdin of another. This is done by: (1) creating a pipe, (2) redirecting stdout of the first child to the pipe write-end, (3) redirecting stdin of the second child to the pipe read-end.

**Three methods:**
- `dup()`: Closes a fd, then duplicates to lowest available. `close(1); dup(fd[1]);` → stdout becomes pipe write-end.
- `dup2(oldfd, newfd)`: Directly duplicates `oldfd` to `newfd`. `dup2(fd[1], 1)` → stdout becomes pipe.
- `fcntl(fd, F_DUPFD, n)`: Duplicates fd to lowest fd >= n.

**Execution:** `gcc 4.c -o 4 && ./4 dup` or `./4 dup2` or `./4 fcntl`

**Common Mistakes:** `dup()` doesn't let you choose the target fd; `dup2()` does. Forgetting `close(1)` before `dup()` fails the redirect.

**Viva Questions:**
1. *Difference between dup and dup2?* → `dup()` uses the lowest available fd; `dup2(old, new)` explicitly targets `new`.
2. *What is F_DUPFD in fcntl?* → Finds the lowest available fd ≥ the third argument and duplicates `oldfd` to it.
3. *Why fork twice?* → One child for `ls -l` (writes to pipe), another for `wc` (reads from pipe).

---

## Exercise 5 — Counting Directories with dup2

**Aim:** Implement `ls -l | grep ^d | wc` using only dup2, counting directories.

**Concept:** Three-stage pipe requires two intermediate pipes. Three children are forked; each redirects its stdin/stdout using `dup2`.

**Execution:** `gcc 5.c -o 5 && ./5`

**Expected Output:** A number (total directories in cwd)

**Viva Questions:**
1. *Why do we use two pipes for three commands?* → `ls -l` → pipe1 → `grep ^d` → pipe2 → `wc -l`. Two pipes connect three processes.
2. *What does grep ^d do?* → Matches lines beginning with 'd' (directory entries in `ls -l`).
3. *Why must each child close all unused pipe ends?* → To prevent the read from blocking due to open write-end references.

---

## Exercise 6 — FIFO File Creation

**Aim:** Create FIFO (named pipe) using mknod command, mkfifo command, strace comparison, mknod syscall, and mkfifo library function.

**Concept:** A FIFO is a named pipe — it has a filesystem entry and can be used by unrelated processes. Shell: `mknod f p` or `mkfifo f`. C: `mkfifo(name, 0666)` or `mknod(name, S_IFIFO|0666, 0)`.

**strace conclusion:** Both `mknod` and `mkfifo` commands internally invoke the `mknod(2)` system call. `mkfifo` is a C library wrapper. Prefer `mkfifo()` for clarity.

**Execution:** `gcc 6.c -o 6 && ./6`

**Viva Questions:**
1. *Difference between pipe and FIFO?* → Pipe: anonymous, between related processes (same parent). FIFO: named, exists on filesystem, any two processes can use it.
2. *What file type indicator does ls -l show for a FIFO?* → `p` (e.g., `prw-r--r--`)
3. *Which is preferred: mknod or mkfifo for creating a FIFO?* → `mkfifo` — clearer intent and portable.

---

## Exercise 7 — FIFO One-Way Communication

**Aim:** Communicate between two unrelated processes using a FIFO (one-way).

**Concept:** Reader opens FIFO with `O_RDONLY` (blocks until writer opens). Writer opens with `O_WRONLY`. Data flows one way.

**Execution (two terminals):**
```bash
gcc 7_reader.c -o 7_reader && gcc 7_writer.c -o 7_writer
# Terminal 1: ./7_reader
# Terminal 2: ./7_writer
```

**Viva Questions:**
1. *Why does the reader block on open()?* → `open()` on a FIFO for reading blocks until another process opens it for writing.
2. *What happens if writer exits without writing?* → Reader gets 0 bytes (EOF).
3. *Can a FIFO work between unrelated processes?* → Yes — that's the key difference from anonymous pipes.

---

## Exercise 8 — FIFO Two-Way Communication

**Aim:** Bidirectional FIFO communication between two processes using two FIFOs (pipe1, pipe2).

**Concept:** Process A writes to `pipe1` and reads from `pipe2`. Process B reads from `pipe1` and writes to `pipe2`. Both alternate.

**Execution:** Run `./8a` in Terminal 1, `./8b` in Terminal 2. Type messages alternately.

**Viva Questions:**
1. *Why do we need two FIFOs for two-way communication?* → Each FIFO is unidirectional; two are needed for each direction.
2. *What happens if one side crashes?* → The other side's open() or read()/write() will get EPIPE or error.
3. *How does the protocol end?* → Type "exit" — both programs detect it and break the loop.

---

## Exercise 9 — Waiting for Data in FIFO (select)

**Aim:** Use `select()` to wait for data in a FIFO with a 10-second timeout.

**Concept:** `select()` monitors multiple file descriptors and returns when one is ready or when the timeout expires. The FIFO must be opened with `O_NONBLOCK` to avoid blocking on `open()`.

**Key System Call:** `select(nfds, &readfds, NULL, NULL, &timeout)` → returns 0 on timeout, >0 if fd is ready.

**Execution:** `gcc 9.c -o 9 && gcc 9_writer.c -o 9_writer`  
Terminal 1: `./9` | Terminal 2: `./9_writer` (within 10 seconds)

**Viva Questions:**
1. *What does FD_ZERO, FD_SET do?* → `FD_ZERO` clears the fd set. `FD_SET` adds a fd to it.
2. *What is O_NONBLOCK for?* → Opens FIFO without blocking; open() returns immediately even if no writer.
3. *What does select() return on timeout?* → 0.

---

## Exercise 10 — Process File Limit and Pipe Size

**Aim:** Print max open files (RLIMIT_NOFILE) and pipe circular buffer size.

**Concept:** `getrlimit(RLIMIT_NOFILE, &rl)` returns soft/hard limits on open files. `F_GETPIPE_SZ` (Linux fcntl) returns pipe buffer size (default 65536 bytes = 64KB on Linux).

**Execution:** `gcc 10.c -o 10 && ./10`

**Viva Questions:**
1. *What is RLIMIT_NOFILE?* → Max number of open file descriptors per process.
2. *What is the default pipe buffer size on Linux?* → 65536 bytes (64 KB).
3. *Difference between soft and hard limits?* → Soft = current enforced limit; hard = maximum the soft limit can be raised to by non-root.

---

## Exercise 11 — Message Queue Creation

**Aim:** Create a SysV message queue and print its key and ID.

**Concept:** `ftok(path, id)` generates a unique key from a file's inode. `msgget(key, flags)` creates/opens a message queue and returns its ID (like a file descriptor for queues).

**Execution:** `touch progfile && gcc 11.c -o 11 && ./11`

**Viva Questions:**
1. *What is ftok?* → Converts a pathname + project ID to a System V IPC key (key_t).
2. *What flags mean 0666 | IPC_CREAT?* → Create queue if not exists, with read/write permissions for all.
3. *How do you verify the queue exists?* → `ipcs -q`

---

## Exercise 12 — Message Queue Information

**Aim:** Print message queue metadata using `msqid_ds` and `ipc_perm` structures via `msgctl(IPC_STAT)`.

**Key fields:** `msg_perm.mode`, `msg_perm.uid/gid`, `msg_stime`, `msg_rtime`, `msg_ctime`, `msg_qbytes`, `msg_qnum`, `msg_lspid`, `msg_lrpid`.

**Execution:** `touch progfile && gcc 12.c -o 12 && ./12`

**Viva Questions:**
1. *What does IPC_STAT do in msgctl?* → Copies current queue metadata into a `msqid_ds` structure.
2. *What is msg_qbytes?* → Maximum number of bytes allowed in the queue.
3. *What is msg_lspid?* → PID of the process that last called `msgsnd()`.

---

## Exercise 13 — Sending Messages to Message Queue

**Aim:** Send a message to a SysV message queue and verify with `ipcs -q`.

**Key System Call:** `msgsnd(msgid, &msg, size, flags)` — sends message. `flags=0` blocks if queue is full.

**Execution:** `touch progfile && gcc 13.c -o 13 && ./13 && ipcs -q`

**Viva Questions:**
1. *What is the structure of a message buffer?* → First field must be `long msg_type` (> 0); rest is user-defined data.
2. *What does the flags parameter in msgsnd do?* → `0` = block if full; `IPC_NOWAIT` = return EAGAIN if full.
3. *How do you clean up after testing?* → `ipcrm -q <msqid>`

---

## Exercise 14 — Receiving Messages from Message Queue

**Aim:** Receive messages using flag=0 (blocking) and IPC_NOWAIT (non-blocking).

**Key System Call:** `msgrcv(msgid, &msg, size, type, flags)` — receives message.

**Sub-questions:**
- `14a.c` — flag=0: blocks until message arrives
- `14b.c` — IPC_NOWAIT: returns immediately with ENOMSG if no message

**Execution:** `./14_sender && ./14a` or `./14b`

**Viva Questions:**
1. *What is ENOMSG?* → Errno set when `msgrcv` with `IPC_NOWAIT` finds no matching message.
2. *What does the type parameter in msgrcv do?* → 0 = get first message; >0 = get first message of that type; <0 = get lowest type ≤ |type|.
3. *Difference between blocking and non-blocking msgrcv?* → Blocking waits; non-blocking returns an error immediately.

---

## Exercise 15 — Changing Message Queue Permissions

**Aim:** Change queue permissions using `msgctl(IPC_SET)` with `msqid_ds`.

**Algorithm:** Get current info via `IPC_STAT` → modify `msg_perm.mode` → apply with `IPC_SET`.

**Execution:** `touch progfile && gcc 15.c -o 15 && ./15`

**Viva Questions:**
1. *What is IPC_SET?* → `msgctl` command that sets attributes from a `msqid_ds` buffer into the kernel.
2. *Can a non-owner change queue permissions?* → No; only the owner or root can.
3. *What mode value means 0644?* → Owner read+write, group read-only, others read-only.

---

## Exercise 16 — Removing Message Queue

**Aim:** Remove a message queue using `msgctl(IPC_RMID)`.

**Execution:** `touch progfile && ./16_sender && ./16 && ipcs -q`

**Viva Questions:**
1. *What happens to blocked processes when a queue is removed?* → They receive EIDRM error.
2. *What is the IPC_RMID command?* → Immediately destroys the message queue and its data.
3. *How else can you remove a queue?* → `ipcrm -q <msqid>` from shell.

---

## Exercise 17 — Shared Memory Operations

**Aim:** Create shared memory, write to it, attach read-only and test overwrite, detach, and remove.

**Key System Calls:**
- `shmget(key, size, flags)` → Create/get shared memory
- `shmat(shmid, NULL, flags)` → Attach (0=rw, SHM_RDONLY=ro)
- `shmdt(ptr)` → Detach
- `shmctl(shmid, IPC_RMID, NULL)` → Remove

**Execution:** `touch shmfile && gcc 17.c -o 17 && ./17`

**Viva Questions:**
1. *What happens if you write to a read-only shmat attachment?* → SIGSEGV (segmentation fault).
2. *Difference between shmdt and shmctl IPC_RMID?* → `shmdt` only detaches from the process's address space; `IPC_RMID` deletes the shared memory segment.
3. *What is the key to shared memory?* → Generated by `ftok()` or `IPC_PRIVATE` for private use.

---

## Exercise 18 — Semaphore Creation and Initialization

**Aim:** Create a binary semaphore (value=1) and a counting semaphore (value=N).

**Key System Calls:** `semget()`, `semctl(semid, 0, SETVAL, arg)`

**Sub-questions:**
- `18a.c` — Binary semaphore: value=1 (acts as mutex)
- `18b.c` — Counting semaphore: value=N (number of resources)

**Execution:** `touch semfile && gcc 18a.c -o 18a && ./18a`

**Viva Questions:**
1. *What is a binary semaphore?* → Semaphore with value 0 or 1; used as a mutual exclusion lock (mutex).
2. *What is a counting semaphore?* → Semaphore with value N representing N available resources.
3. *What is semctl SETVAL?* → Sets the value of a semaphore in the set.

---

## Exercise 23 — Interval Timer Programming

**Aim:** Set interval timer using setitimer for three time domains: REAL, VIRTUAL, PROF.

**Sub-questions:**
- `23a.c` — ITIMER_REAL: wall-clock time, delivers SIGALRM
- `23b.c` — ITIMER_VIRTUAL: user-mode CPU time only, delivers SIGVTALRM
- `23c.c` — ITIMER_PROF: user + kernel CPU time, delivers SIGPROF

**Key API:** `setitimer(ITIMER_REAL, &timer, NULL)` where `timer.it_value` = first expiry, `timer.it_interval` = repeat interval.

**Distinction:** `alarm()` vs `setitimer()` — `alarm()` only sets a one-shot timer; `setitimer()` supports microsecond precision and repeating intervals.

**Execution:** `gcc 23a.c -o 23a && ./23a` (waits 10 seconds per alarm)

**Viva Questions:**
1. *What signal does ITIMER_REAL deliver?* → SIGALRM.
2. *Why does ITIMER_VIRTUAL need a busy loop?* → It only counts when the process is executing in user mode — `pause()` or `sleep()` don't consume user CPU.
3. *What is the difference between ITIMER_VIRTUAL and ITIMER_PROF?* → VIRTUAL counts only user time; PROF counts user + kernel (system call) time.

---

## Exercise 24 — System Resource Limits (getrlimit)

**Aim:** Print all system resource limits for the current process.

**Key System Call:** `getrlimit(resource, &rl)` — fills `rlimit` struct with `rlim_cur` (soft) and `rlim_max` (hard).

**Execution:** `gcc 24.c -o 20 && ./20`

**Viva Questions:**
1. *What is RLIMIT_CPU?* → Maximum CPU time (seconds) the process may consume before getting SIGXCPU.
2. *What is RLIM_INFINITY?* → A constant meaning no limit.
3. *Can a non-root process raise the hard limit?* → No; only root can increase hard limits. Non-root can lower hard limits or raise soft ≤ hard.

---

## Exercise 25 — Setting System Resource Limit (setrlimit)

**Aim:** Set a resource limit using `setrlimit`.

**Key System Call:** `setrlimit(resource, &rl)` — sets soft and hard limits.

**Execution:** `gcc 25.c -o 21 && ./21`

**Viva Questions:**
1. *What is the difference between soft and hard limits?* → Soft is enforced; hard is the ceiling. Non-root can raise soft up to hard but cannot increase hard.
2. *What happens if setrlimit sets core size to 0?* → No core dump files are generated on crash.
3. *Which resource limit governs open files?* → `RLIMIT_NOFILE`.

---

## Exercise 26 — Execution Time Measurement

**Aim:** Measure time for 100 `getppid()` system calls using the CPU timestamp counter.

**Concept:** On ARM64: `mrs %0, cntvct_el0` reads the hardware virtual counter. On x86: `rdtsc`. Elapsed cycles = end - start; average = elapsed / 100.

**Execution:** `gcc 26.c -o 22 && ./22`

**Viva Questions:**
1. *What is a timestamp counter (TSC)?* → A hardware register that increments every clock cycle; used for fine-grained timing.
2. *Why not use time() or clock()?* → Too coarse (second/millisecond resolution); TSC gives nanosecond resolution.
3. *What does getppid() do?* → Returns the PID of the parent process; chosen because it's a simple, fast system call.

---

## Exercise 27 — System Limitation Exploration (sysconf)

**Aim:** Print 7 system limitations using `sysconf()`.

**Key System Call:** `sysconf(int name)` — queries system configuration values.

**Execution:** `gcc 27.c -o 23 && ./23`

**Linux-only note:** `_SC_PHYS_PAGES` and `_SC_AVPHYS_PAGES` are not available on macOS (guarded with `#ifdef`).

**Viva Questions:**
1. *What does _SC_CLK_TCK return?* → Clock ticks per second (jiffies); typically 100 on Linux.
2. *What is a jiffy?* → The time for one clock tick; 1/HZ seconds (typically 10 ms if HZ=100).
3. *What is _SC_PAGESIZE?* → Memory page size in bytes; typically 4096 on x86_64, 16384 on Apple Silicon.

---

## Exercise 28 — Multithreading Exploration

**Aim:** Create 3 POSIX threads and print their IDs.

**Key System Calls:** `pthread_create(&tid, NULL, func, arg)`, `pthread_join(tid, NULL)`, `pthread_self()`.

**Compilation:** `gcc 28.c -o 24 -lpthread`

**Viva Questions:**
1. *What is pthread_self()?* → Returns the thread ID of the calling thread.
2. *Difference between process and thread?* → Threads share address space, heap, and file descriptors; processes have separate address spaces.
3. *What is pthread_join?* → Makes the calling thread wait for the specified thread to finish.

---

## Exercise 29 — Signal Handling (signal system call)

**Aim:** Catch 7 different signals: SIGSEGV, SIGINT, SIGFPE, SIGALRM (alarm), SIGALRM (setitimer), SIGVTALRM, SIGPROF.

**Key System Call:** `signal(signum, handler)` — registers a handler. Returns old handler or SIG_ERR.

**Sub-files:** `29a.c`–`29g.c`

**Viva Questions:**
1. *Difference between signal() and sigaction()?* → `sigaction()` is more portable and reliable; allows signal masking during handler execution; `signal()` behavior varies across UNIX implementations.
2. *What signals CANNOT be caught?* → SIGKILL and SIGSTOP.
3. *What is SIG_IGN and SIG_DFL?* → `SIG_IGN`: ignore the signal. `SIG_DFL`: restore default action (usually terminate).

---

## Exercise 30 — Ignoring and Resetting Signals (signal)

**Aim:** Ignore SIGINT then reset to default using `signal()`.

**Execution:** `gcc 30.c -o 26 && ./26` → Ctrl+C ignored for 5s, then terminates.

**Viva Questions:**
1. *What does SIG_IGN do?* → Tells the kernel to discard the signal; no handler runs.
2. *Can SIGKILL be ignored?* → No. Only SIGKILL and SIGSTOP cannot be caught or ignored.
3. *What is the default action of SIGINT?* → Terminate the process.

---

## Exercise 31 — Signal Handling with sigaction

**Aim:** Use `sigaction()` to catch SIGSEGV, SIGINT, SIGFPE.

**Key System Call:**
```c
struct sigaction sa;
sa.sa_handler = handler;
sigemptyset(&sa.sa_mask);
sa.sa_flags = 0;
sigaction(SIGINT, &sa, NULL);
```

**Viva Questions:**
1. *Advantage of sigaction over signal?* → Portable, allows blocking other signals during handler via `sa_mask`, supports extended info via `SA_SIGINFO`.
2. *What is sigemptyset?* → Initializes the signal mask to empty (no signals blocked).
3. *What does SA_RESTART flag do?* → Automatically restarts system calls interrupted by a signal.

---

## Exercise 32 — Ignore and Reset SIGINT via sigaction

**Aim:** Use `sigaction()` with `SIG_IGN` to ignore SIGINT, then `SIG_DFL` to restore.

**Viva Questions:**
1. *How is this different from Exercise 26?* → Uses `sigaction()` instead of `signal()` — more portable and consistent.
2. *What happens to pending SIGINTs while it is ignored?* → They are discarded.
3. *Can you restore a previously saved handler?* → Yes: save old handler via third argument of `sigaction()`, restore later.

---

## Exercise 33 — Creating an Orphan Process

**Aim:** Child sends SIGKILL to parent using `kill()`, making itself an orphan.

**Concept:** An orphan process is one whose parent has died. Linux re-parents it to PID 1 (init/systemd).

**Execution:** `gcc 33.c -o 29 && ./29`

**Viva Questions:**
1. *What is an orphan process?* → A child process whose parent has terminated; it gets re-parented to PID 1.
2. *What is a zombie process?* → A process that has exited but whose parent hasn't called wait() to collect its exit status.
3. *What does kill(pid, SIGKILL) do?* → Sends SIGKILL to the specified process; it cannot be caught or ignored.

---

## Exercise 34 — Signal SIGSTOP Handling

**Aim:** Demonstrate that SIGSTOP cannot be caught by any program.

**Execution:**
- Terminal 1: `./30_receiver` — tries to register handler → fails with EINVAL
- Terminal 2: `./30_sender <PID>` — sends SIGSTOP
- Result: process is stopped by kernel, handler never runs

**Viva Questions:**
1. *Can SIGSTOP be caught?* → No. The kernel handles SIGSTOP directly; it cannot be caught, blocked, or ignored.
2. *What is the difference between SIGSTOP and SIGTSTP?* → SIGSTOP is unblockable; SIGTSTP (Ctrl+Z) can be caught.
3. *How to resume a stopped process?* → Send SIGCONT: `kill -CONT <pid>`

---

## Exercise 19 — Semaphore Implementation

**Aim:** Implement semaphores for 4 use cases.

**Sub-questions:**
- `23a.c` + `23a_db.c` — Ticket number using binary semaphore
- `23b.c` — Protect shared memory from concurrent writes
- `23c.c` — Protect 2 pseudo-resources using counting semaphore
- `23d.c` — Remove a semaphore

**Key operations:**
- `sem_p()` (wait/P/lock): `sembuf op = {0, -1, 0}; semop(...)`
- `sem_v()` (signal/V/unlock): `sembuf op = {0, +1, 0}; semop(...)`

**Execution:** `./31a_db && ./23a` (for ticket demo)

**Viva Questions:**
1. *What does semop with -1 do?* → P operation: decrements semaphore; blocks if value is 0.
2. *What does semop with +1 do?* → V operation: increments semaphore; unblocks waiting process.
3. *What is the purpose of a semaphore in the critical section?* → Ensures mutual exclusion — only one process at a time in the critical section.

---

## Exercise 20 — Deadlock

**Aim:** Intentionally create deadlock using two semaphores and two processes.

**Concept (circular wait):**
- Process A: locks sem1 → tries to lock sem2 (waits)
- Process B: locks sem2 → tries to lock sem1 (waits)
- Both wait forever → deadlock

**Execution:** `gcc 24.c -o 32 && ./32` — program hangs; use Ctrl+C then `ipcs -s && ipcrm -s <id>` to clean up.

**Viva Questions:**
1. *Four necessary conditions for deadlock?* → Mutual exclusion, Hold-and-wait, No preemption, Circular wait.
2. *How can this deadlock be prevented?* → Resource ordering (always acquire sem1 before sem2 in all processes).
3. *How to detect this deadlock?* → Resource allocation graph with a cycle; or timeout + retry logic.

---

## Exercise 21 — Inter-Machine Socket Communication

**Aim:** Client-server chat using TCP sockets.

**Key System Calls:**
- Server: `socket()`, `bind()`, `listen()`, `accept()`, `read()`, `send()`
- Client: `socket()`, `connect()`, `send()`, `read()`

**Execution:** Terminal 1: `./33_server` | Terminal 2: `./33_client`

**Viva Questions:**
1. *What is AF_INET, SOCK_STREAM?* → AF_INET = IPv4 address family; SOCK_STREAM = TCP (connection-oriented, reliable).
2. *What does bind() do?* → Associates a socket with a local IP address and port.
3. *Difference between send() and write() for sockets?* → `send()` supports flags (e.g., MSG_NOSIGNAL); `write()` is equivalent with flags=0.

---

## Exercise 22 — Concurrent Server Creation

**Aim:** Handle multiple clients simultaneously using fork (26a) and pthread (26b).

**Sub-questions:**
- `26a.c` — `fork()`: each client gets a child process (heavy but isolated)
- `26b.c` — `pthread_create()`: each client gets a thread (lightweight, shared memory)

**Compilation:**
```bash
gcc 26a.c -o 26a
gcc 26b.c -o 26b -lpthread
```
**Test:** `./26a &` then `nc localhost 8080` in another terminal.

**Viva Questions:**
1. *Fork vs pthread for concurrent server?* → `fork`: separate memory, more overhead, simpler; `pthread`: shared memory, lighter, needs synchronization.
2. *What is pthread_detach?* → Tells the system to automatically reclaim the thread's resources when it exits (no need to join).
3. *What is SO_REUSEADDR?* → Socket option that allows reusing a port that's in TIME_WAIT state (common after quick server restart).

