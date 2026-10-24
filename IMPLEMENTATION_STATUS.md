# IMPLEMENTATION STATUS — SS_HOL2
**Author:** Dabhi Rushirajsinh | **Date:** 23rd Oct, 2026  
**Course:** EGC 301P — Operating Systems Lab  
**Syllabus:** Questions 31–64 → Folders 1–34

---

## Environment

| Environment | Usage |
|-------------|-------|
| macOS (ARM64 / Apple Silicon) | Writing, compiling, portable runtime tests |
| Linux VM (QEMU) | Runtime tests for SysV IPC, `/proc`, Linux-specific sysconf |

---

## Complete Status Table

| Folder | Q# | Topic | Files | Compiled (macOS) | Subparts Complete | Linux Required |
|--------|----|-------|-------|:----------------:|:-----------------:|:--------------:|
| 1  | Q31 | Pipe create/write/read | `1.c` | ✓ | ✓ | No |
| 2  | Q32 | Parent→child pipe | `2.c` | ✓ | ✓ | No |
| 3  | Q33 | Two-way pipe | `3.c` | ✓ | ✓ | No |
| 4  | Q34 | ls -l \| wc (dup/dup2/fcntl) | `4.c` | ✓ | ✓ (3 methods) | No |
| 5  | Q35 | Count dirs with dup2 | `5.c` | ✓ | ✓ | No |
| 6  | Q36 | FIFO creation (5 sub-parts) | `6.c` | ✓ | ✓ (all 5) | No |
| 7  | Q37 | FIFO one-way | `7_reader.c`, `7_writer.c` | ✓ | ✓ (2 progs) | No |
| 8  | Q38 | FIFO two-way | `8a.c`, `8b.c` | ✓ | ✓ (2 progs) | No |
| 9  | Q39 | FIFO + select | `9.c`, `9_writer.c` | ✓ | ✓ (2 progs) | No |
| 10 | Q40 | Max files + pipe size | `10.c` | ✓ | ✓ | Preferred (F_GETPIPE_SZ) |
| 11 | Q41 | MsgQ creation | `11.c` | ✓ | ✓ | Preferred |
| 12 | Q42 | MsgQ info (8 fields) | `12.c` | ✓ | ✓ (all 8) | Preferred |
| 13 | Q43 | MsgQ send | `13.c` | ✓ | ✓ | Preferred |
| 14 | Q44 | MsgQ receive (flag=0 + IPC_NOWAIT) | `14a.c`, `14b.c`, `14_sender.c` | ✓ | ✓ (both) | Preferred |
| 15 | Q45 | MsgQ permissions | `15.c`, `15_sender.c` | ✓ | ✓ | Preferred |
| 16 | Q46 | MsgQ remove | `16.c`, `16_sender.c` | ✓ | ✓ | Preferred |
| 17 | Q47 | Shared memory (4 ops) | `17.c` | ✓ | ✓ (all 4) | Preferred |
| 18 | Q48 | Semaphore create+init | `18a.c`, `18b.c` | ✓ | ✓ (binary+counting) | Preferred |
| 19 | Q49 | Semaphore implementation (4 tasks) | `19a.c`, `19a_db.c`, `19b.c`, `19c.c`, `19d.c` | ✓ | ✓ (all 4) | Preferred |
| 20 | Q50 | Deadlock (intentional hang) | `20.c` | ✓ | ✓ | No |
| 21 | Q51 | Socket inter-machine comm | `21_server.c`, `21_client.c` | ✓ | ✓ (2 progs) | No |
| 22 | Q52 | Concurrent server | `22a.c`, `22b.c` | ✓ | ✓ (fork+pthread) | No |
| 23 | Q53 | Interval timers (REAL/VIRTUAL/PROF) | `23a.c`, `23b.c`, `23c.c` | ✓ | ✓ (all 3) | No |
| 24 | Q54 | getrlimit (resource limits) | `24.c` | ✓ | ✓ | No |
| 25 | Q55 | setrlimit | `25.c` | ✓ | ✓ | No |
| 26 | Q56 | Timestamp counter (100 getppid) | `26.c` | ✓ | ✓ | No |
| 27 | Q57 | sysconf — 7 system limitations | `27.c` | ✓ | ✓ (all 7, 2 guarded) | Partial |
| 28 | Q58 | 3 threads with IDs | `28.c` | ✓ (-lpthread) | ✓ | No |
| 29 | Q59 | signal() — 7 signal programs | `29a.c`–`29g.c` | ✓ (all 7) | ✓ (all 7) | No |
| 30 | Q60 | Ignore + reset SIGINT (signal) | `30.c` | ✓ | ✓ | No |
| 31 | Q61 | sigaction() — 3 signals | `31a.c`, `31b.c`, `31c.c` | ✓ (all 3) | ✓ (all 3) | No |
| 32 | Q62 | Ignore + reset SIGINT (sigaction) | `32.c` | ✓ | ✓ | No |
| 33 | Q63 | Orphan process (child SIGKILL parent) | `33.c` | ✓ | ✓ | No |
| 34 | Q64 | SIGSTOP (cannot be caught — 2 progs) | `34_receiver.c`, `34_sender.c` | ✓ (both) | ✓ (both) | No |

---

## Summary

| Metric | Count |
|--------|-------|
| Total exercises (Q31–Q64) | **34** |
| Folders created | **34** |
| Total `.c` source files | **59** |
| Compiled on macOS (confirmed) | **59 / 59** |
| Exercises needing Linux VM for full runtime | **9** (Q40–Q48) |
| Sub-questions complete | **All** |

---

## Quick Compilation Reference

```bash
# All standard programs (no special flags)
gcc <N>/<file>.c -o <N>/<outname>

# pthread required (Q52 concurrent server, Q58 threads)
gcc 22/22b.c -o 22/22b -lpthread
gcc 28/28.c  -o 28/28  -lpthread

# Key-file creation for SysV IPC (Q41-Q49)
touch 11/progfile 12/progfile 13/progfile 14/progfile \
      15/progfile 16/progfile 17/shmfile 18/semfile 19/semfile

# Q49 semaphore implementation: initialize DB first
cd 19 && gcc 19a_db.c -o 19a_db && ./19a_db   # creates 'database' file
         gcc 19a.c -o 19a && ./19a              # ticket number demo

# Q50 deadlock: intentionally hangs — Ctrl+C to stop, then:
ipcs -s         # find semaphore IDs
ipcrm -s <ID>   # clean up

# Q53 timers: each takes 10+ seconds to fire alarms
# Q59 signal demos: 29b.c needs Ctrl+C; 29d/e/f/g need time
```

---

## Linux-Specific Notes

| Folder/Q# | Feature | Note |
|-----------|---------|------|
| 10 / Q40 | `F_GETPIPE_SZ` | Linux fcntl only; guarded with `#ifdef` |
| 11–19 / Q41–Q49 | SysV IPC (`msgget`, `semget`, `shmget`) | Works on macOS too but `ipcs`/`ipcrm` differ |
| 27 / Q57 | `_SC_PHYS_PAGES`, `_SC_AVPHYS_PAGES` | Linux only; guarded with `#ifdef`; macOS prints "Not available" |

---

## Backup Location

Original pre-reorder files backed up at:  
`_backup_reorder/` (folders 19–34 in their old incorrect order)
