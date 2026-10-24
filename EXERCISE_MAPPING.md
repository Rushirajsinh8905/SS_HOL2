# EXERCISE MAPPING — SS_HOL2
**Author:** Dabhi Rushirajsinh | **Date:** 23rd Oct, 2026  
**Syllabus:** EGC 301P OS Lab — Questions 31–64 mapped to Folders 1–34

> The syllabus document `List of OS Lab Exercises.docx` numbers exercises 31–64.
> Each folder number N corresponds to syllabus question (N + 30).

---

## Section III — System V IPC Mechanisms (Folders 1–22 → Q31–Q52)

| Folder | Q# | Syllabus Description | File(s) | Sub-questions | Reference |
|--------|----|---------------------|---------|---------------|-----------|
| 1  | Q31 | Pipe creation and communication | `1.c` | Single program | Que14/14.c |
| 2  | Q32 | Data transmission from parent to child | `2.c` | Single program | Que15/15.c |
| 3  | Q33 | Two-way communication (parent ↔ child) | `3.c` | Single program | Que16/16.c |
| 4  | Q34 | `ls -l \| wc` using dup, dup2, fcntl | `4.c` | 3 methods via argv | Que17/17.c |
| 5  | Q35 | Count directories using dup2 | `5.c` | Single program | Que18/18.c |
| 6  | Q36 | FIFO creation (mknod cmd, mkfifo cmd, strace, mknod syscall, mkfifo fn) | `6.c` | All 5 sub-parts | Que19/19.c |
| 7  | Q37 | FIFO one-way communication | `7_reader.c`, `7_writer.c` | 2 programs | Que20/ |
| 8  | Q38 | FIFO two-way communication | `8a.c`, `8b.c` | 2 programs | Que21/ |
| 9  | Q39 | FIFO + select timeout (10 seconds) | `9.c`, `9_writer.c` | 2 programs | Que22/ |
| 10 | Q40 | Max open files and pipe circular buffer size | `10.c` | Single program | Que23/23.c |
| 11 | Q41 | Message queue creation (key + ID) | `11.c` | Single program | Que24/24.c |
| 12 | Q42 | Message queue information (8 fields) | `12.c` | All 8 fields | Que25/25.c |
| 13 | Q43 | Sending messages to message queue | `13.c` | Single program | Que26/26.c |
| 14 | Q44 | Receive messages (flag=0, IPC_NOWAIT) | `14a.c`, `14b.c`, `14_sender.c` | 2 sub-programs | Que27/ |
| 15 | Q45 | Change message queue permissions | `15.c`, `15_sender.c` | IPC_SET | Que28/ |
| 16 | Q46 | Remove message queue | `16.c`, `16_sender.c` | IPC_RMID | Que29/29.c |
| 17 | Q47 | Shared memory (write, rdonly, detach, remove) | `17.c` | All 4 sub-parts | Que30/30.c |
| 18 | Q48 | Semaphore creation: binary + counting | `18a.c`, `18b.c` | 2 sub-programs | Que31/ |
| 19 | Q49 | Semaphore implementation (4 sub-tasks) | `19a.c`, `19a_db.c`, `19b.c`, `19c.c`, `19d.c` | All 4 sub-tasks | Que32/ |
| 20 | Q50 | Deadlock using semaphores | `20.c` | Circular-wait demo | Que32d |
| 21 | Q51 | Inter-machine socket communication | `21_server.c`, `21_client.c` | 2 programs | Que33/ |
| 22 | Q52 | Concurrent server (fork + pthread_create) | `22a.c`, `22b.c` | 2 sub-programs | Que34/ |

---

## Section IV — Timers, Resource Limits, Multithreading, Signals (Folders 23–34 → Q53–Q64)

| Folder | Q# | Syllabus Description | File(s) | Sub-questions | Reference |
|--------|----|---------------------|---------|---------------|-----------|
| 23 | Q53 | Interval timer programming (10s + 10µs) | `23a.c`, `23b.c`, `23c.c` | REAL/VIRTUAL/PROF | Que1/ |
| 24 | Q54 | System resource limits (getrlimit) | `24.c` | 7+ resources | Que2/2.c |
| 25 | Q55 | Setting system resource limit (setrlimit) | `25.c` | Core size demo | Que3/3.c |
| 26 | Q56 | Execution time measurement (timestamp counter) | `26.c` | 100 getppid() calls | Que4/4.c |
| 27 | Q57 | System limitations (sysconf — 7 values) | `27.c` | All 7 sysconf values | Que5/5.c |
| 28 | Q58 | Multithreading: 3 threads, print IDs | `28.c` | pthread_create x3 | Que7/7.c |
| 29 | Q59 | Signal handling via signal() — 7 signals | `29a.c`–`29g.c` | SIGSEGV/SIGINT/SIGFPE/SIGALRM(2)/SIGVTALRM/SIGPROF | Que8/ |
| 30 | Q60 | Ignore + reset SIGINT via signal() | `30.c` | SIG_IGN then SIG_DFL | Que9/9.c |
| 31 | Q61 | Signal handling via sigaction() — 3 signals | `31a.c`, `31b.c`, `31c.c` | SIGSEGV/SIGINT/SIGFPE | Que10/ |
| 32 | Q62 | Ignore + reset SIGINT via sigaction() | `32.c` | SIG_IGN then SIG_DFL | Que11/11.c |
| 33 | Q63 | Orphan process (child sends SIGKILL to parent) | `33.c` | kill() + orphan | Que12/12.c |
| 34 | Q64 | SIGSTOP: receiver + sender (cannot be caught) | `34_receiver.c`, `34_sender.c` | 2 programs | Que13/ |

---

## Sub-question Coverage Details

### Q36 / Folder 6 — FIFO Creation (all 5 sub-parts)
1. `mknod` shell command — shown in sample output comment
2. `mkfifo` shell command — shown in sample output comment
3. `strace` comparison — explained in comment: both call `mknod(2)` internally; `mkfifo` preferred
4. `mknod()` C system call — implemented in `6.c`
5. `mkfifo()` C library function — implemented in `6.c`

### Q42 / Folder 12 — Message Queue Information (all 8 fields)
1. Access permission (`msg_perm.mode`)
2. UID, GID (`msg_perm.uid/gid`)
3. Time of last send (`msg_stime`)
4. Time of last receive (`msg_rtime`)
5. Time of last change (`msg_ctime`)
6. Size of queue (`msg_qbytes`)
7. Number of messages (`msg_qnum`)
8. Max bytes allowed (`msg_qbytes`)
9. PID of msgsnd/msgrcv (`msg_lspid`, `msg_lrpid`)

### Q47 / Folder 17 — Shared Memory (all 4 operations)
1. Write data
2. Attach SHM_RDONLY + overwrite attempt (causes SIGSEGV — demonstrated in comment)
3. Detach (`shmdt`)
4. Remove (`shmctl IPC_RMID`)

### Q48 / Folder 18 — Semaphore Creation
- `18a.c` — Binary semaphore (value=1, mutex)
- `18b.c` — Counting semaphore (value=N, via argv)

### Q49 / Folder 19 — Semaphore Implementation (all 4 sub-tasks)
- `19a.c` + `19a_db.c` — Ticket number creation using binary semaphore
- `19b.c` — Protect shared memory from concurrent write
- `19c.c` — Protect pseudo resources using counting semaphore
- `19d.c` — Remove the semaphore

### Q53 / Folder 23 — Interval Timers (all 3 domains)
- `23a.c` — ITIMER_REAL (delivers SIGALRM, wall-clock time)
- `23b.c` — ITIMER_VIRTUAL (delivers SIGVTALRM, user CPU time)
- `23c.c` — ITIMER_PROF (delivers SIGPROF, user+kernel CPU time)

### Q57 / Folder 27 — System Limitations (all 7)
1. Max exec argument length (`_SC_ARG_MAX`)
2. Max simultaneous processes per UID (`_SC_CHILD_MAX`)
3. Clock ticks per second (`_SC_CLK_TCK`)
4. Max open files (`_SC_OPEN_MAX`)
5. Page size (`_SC_PAGESIZE`)
6. Total physical pages (`_SC_PHYS_PAGES` — Linux only, guarded with `#ifdef`)
7. Available physical pages (`_SC_AVPHYS_PAGES` — Linux only, guarded with `#ifdef`)

### Q59 / Folder 29 — Signal Handling via signal() (all 7)
- `29a.c` — SIGSEGV
- `29b.c` — SIGINT
- `29c.c` — SIGFPE
- `29d.c` — SIGALRM via alarm()
- `29e.c` — SIGALRM via setitimer
- `29f.c` — SIGVTALRM via setitimer
- `29g.c` — SIGPROF via setitimer

### Q61 / Folder 31 — sigaction() Signals (3 programs)
- `31a.c` — SIGSEGV
- `31b.c` — SIGINT
- `31c.c` — SIGFPE

### Q64 / Folder 34 — SIGSTOP (2 programs)
- `34_receiver.c` — Tries to catch SIGSTOP; demonstrates it cannot be caught (returns SIG_ERR); then waits for SIGSTOP from sender (process freezes — kernel handles it directly)
- `34_sender.c` — Sends SIGSTOP to the receiver process via kill()

---

## Gap / Conflict Notes

| Item | Status |
|------|--------|
| HOL1 content in folders 2, 3, 4 | Overwritten as per user approval |
| HOL2 Que6 (not in syllabus) | Skipped — not referenced in Q31–Q64 |
| macOS SysV IPC | Compiles OK; Linux VM recommended for runtime testing |
| `_SC_PHYS_PAGES` / `_SC_AVPHYS_PAGES` | Guarded with `#ifdef` for portability |
| `F_GETPIPE_SZ` in 10.c | Linux-only; handled with `#ifdef` |
| Exercise 20 (Deadlock) | Intentionally hangs — Ctrl+C to stop, then `ipcrm -s` to clean up |
