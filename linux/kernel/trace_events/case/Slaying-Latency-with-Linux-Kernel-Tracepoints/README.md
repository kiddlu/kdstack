Slaying Latency with Linux Kernel Tracepoints

Knowing what tools are at your disposal when looking to make a program faster or keep it operating smoothly is critical to your success as a programmer. We recently solved a performance problem using Linux Kernel Tracepoints so we thought we’d share the process with you so that you can see for yourself if this tool makes sense in your toolkit.

Our challenge: the database writes in a customer’s application normally took under 5ms but occasionally spiked to over 150ms, a huge variance that was noticeable by the user. Because the database writes were tied to user action, those latency spikes made the UX feel sluggish and non-responsive. We suspected that the problem was some type of I/O latency but wanted to verify this and determine what was happening to see what we could do about it.

The database engine we used was PostgreSQL; a quick check in the source code revealed that each SQL update did a write() followed by fsync(). To see if we could duplicate the problem without the PostgreSQL engine obfuscating things, we wrote a minimal test case on our embedded platform that looped write() and fsync() operations, aborting if it encountered any I/O latency spikes. Thankfully, this duplicated our database latency problem about every 200 writes or so – the same frequency of our PostgreSQL application – which confirmed that it was an I/O issue.

Test case source code:

```
qint32 main(qint32 argc, char *argv[]){
   const int file = open("test.dat", O_RDWR | O_CREAT | O_TRUNC);

   for (int i = 0;; i++) {
       lseek(file, 0, SEEK_SET);
       QByteArray data;
       const int dataSize = 8 * 1024;
       data.fill((char)rand(), dataSize);
     
       QElapsedTimer writeTimer;
       writeTimer.start();
       write(file, data.data(), dataSize);
       fsync (file);
       const int elapsed = (int)writeTimer.elapsed();
     
       if (i > 10 && elapsed > 150) {
           qDebug("Write %d > 150ms, took %d ms", i, elapsed);
           exit(2);
       }
   }
   close(file);
}
```

This confirmation wasn’t very satisfying because we couldn’t understand much more once we entered the C file library calls. Or could we? This is where tracepoints come in because they allow us to enable tracing within the kernel. Kernel tracepoints (or “static tracepoints”) are specific events within a kernel that you can selectively enable to show the execution path and characteristics of your software.

Thankfully, we can turn on tracing independently by module — if we turn it on for everything the kernel does, the system would grind to a halt and we’d need to wade through megabytes of irrelevant data to find our problem. Because we suspected an I/O problem, we were able to narrow in on the subsystems related to disk I/O: block (block device driver), ext4 (file system), jdb2 (journaling system), and mmc (hardware interface).

To enable these tracepoints, we needed to tweak a couple of variables in the kernel through the virtual filesystem exported by debugfs. This sounds complicated but it’s actually quite easily done using a command shell:

```
cd /sys/kernel/debug/tracing
echo 1 > events/block/enable
echo 1 > events/ext4/enable
echo 1 > events/jbd2/enable
echo 1 > events/mmc/enable
echo 1 > tracing_on
```

Running the test case a second time, the file /sys/kernel/debug/tracing/trace contained a text log of all enabled tracepoints. For ease of execution, we wrapped the tracing and the test case together into a script so we could easily limit logging to the test.

So what did the trace look like for our test case? Logging. Lots and lots of logging. (You can see a relevant portion in the attached file trace-io.txt.) We found the latency events that we’re interested in by looking for large gaps in the time code. The first time that occurred (over 274ms) was between ext4_sync_file_enter and ext4_sync_file_exit. That length matched what the test program printed, so we knew this was the right occurrence. Looking at the events between those two tracepoints, we saw two writes:

1) Writing the actual file data, between the first block_rq_issue and block_rq_complete. At only 2ms, this clearly wasn’t our issue.

2) Writing the journal, between jbd2_start_commit and jbd2_end_commit. This took 272ms. The culprit was between mmc_request_start and mmc_request_done, which took nearly 270ms. Here is the latency-producing event (formatting added for clarity):
```
mmcqd/0-89   [000] d..2   474.223046: block_rq_issue: 179,0 WFS 0 () 4717976 + 8 [mmcqd/0]

mmcqd/0-89   [000] ...1   474.223072: mmc_request_start: mmc0: start struct mmc_request
[c669d954]: cmd_opcode=25 cmd_arg=0x47fd98 cmd_flags=0xb5 cmd_retries=0 stop_opcode=12 
stop_arg=0x0 stop_flags=0x49d stop_retries=0 sbc_opcode=23 sbc_arg=0x80000008 sbc_flags=0x15 
sbc_retires=0 blocks=8 block_size=512 data_flags=0x100 can_retune=0 doing_retune=0 
retune_now=0 need_retune=0 hold_retune=1 retune_period=0

<idle>-0     [000] ..s2   474.492701: mmc_request_done: mmc0: end struct mmc_request
[c669d954]: cmd_opcode=25 cmd_err=0 cmd_resp=0x900 0x0 0x0 0x0 cmd_retries=0 stop_opcode=12 
stop_err=0 stop_resp=0x0 0x0 0x0 0x0 stop_retries=0 sbc_opcode=23 sbc_err=0 sbc_resp=0x0 
0x0 0x0 0x0 sbc_retries=0 bytes_xfered=4096 data_err=0 can_retune=0 doing_retune=0 
retune_now=0 need_retune=0 hold_retune=1 retune_period=0
```

What was the kernel trace showing us? The key was in the cmd_opcode, which we translated via the system header file mmc.h as MMC_WRITE_MULTIPLE_BLOCK. So it looked like the journaling subsystem was incurring our major delay by writing data through the MMC driver. What if we disabled journaling? As you might expect, this did help the issue – the I/O latency spike became much more rare – however, it still occurred. Besides, we couldn’t really disable journaling for our application, so it was not a workable solution.

We wondered: Is it possible that there is a scheduling issue … that it’s not the MMC controller itself? What if the kernel isn’t scheduling the thread that issues the MMC commands in time or putting it to sleep while it does other tasks? To double-check this, we enabled sched and workqueue tracepoints to trace every task switch, like so:
```
echo 1 > events/sched/enable
echo 1 > events/workqueue/enable
```

(The relevant portion of this trace is provided as trace-io+sched.txt.) Since mmcqd is running on CPU 0, all other CPUs could be ignored and have been removed for clarity.
```
#                              _-----=> irqs-off
#                             / _----=> need-resched
#                           | / _---=> hardirq/softirq
#                           || / _--=> preempt-depth
#                           ||| /     delay
#          TASK-PID   CPU# ||||   TIMESTAMP FUNCTION
#             | |       |   ||||       |         |
(#1)     mmcqd/0-89   [000] ...1 1384.282406: mmc_request_start: mmc0: start struct mmc_request[c669d954]: cmd_opcode=25 cmd_arg=0x480dc0 cmd_flags=0xb5 cmd_retries=0 stop_opcode=12 stop_arg=0x0 stop_flags=0x49d stop_retries=0 sbc_opcode=23 sbc_arg=0x80000008 sbc_flags=0x15 sbc_retires=0 blocks=8 block_size=512 data_flags=0x100 can_retune=0 doing_retune=0 retune_now=0 need_retune=0 hold_retune=1 retune_period=0
         mmcqd/0-89   [000] d..3 1384.282424: sched_stat_runtime: comm=mmcqd/0 pid=89 runtime=53666 [ns] vruntime=40997151704 [ns]
(#2)     mmcqd/0-89   [000] d..3 1384.282432: sched_switch: prev_comm=mmcqd/0 prev_pid=89 prev_prio=120 prev_state=S ==> next_comm=swapper/0 next_pid=0 next_prio=120
(#3)     <idle>-0     [000] d.s3 1384.466583: workqueue_queue_work: work struct=c63c21ac function=fb_flashcursor workqueue=c60a8100 req_cpu=4 cpu=0
         <idle>-0     [000] d.s3 1384.466585: workqueue_activate_work: work struct c63c21ac
(#4)     <idle>-0     [000] dns5 1384.466592: sched_wakeup: comm=kworker/0:1 pid=1572 prio=120 success=1 target_cpu=000
         <idle>-0     [000] d..3 1384.466612: sched_switch: prev_comm=swapper/0 prev_pid=0 prev_prio=120 prev_state=R ==> next_comm=kworker/0:1 next_pid=1572 next_prio=120
     kworker/0:1-1572 [000] ...1 1384.466616: workqueue_execute_start: work struct c63c21ac: function fb_flashcursor
     kworker/0:1-1572 [000] ...1 1384.466621: workqueue_execute_end: work struct c63c21ac
     kworker/0:1-1572 [000] d..3 1384.466626: sched_stat_runtime: comm=kworker/0:1 pid=1572 runtime=36000 [ns] vruntime=40988187704 [ns]
     kworker/0:1-1572 [000] d..3 1384.466630: sched_switch: prev_comm=kworker/0:1 prev_pid=1572 prev_prio=120 prev_state=S ==> next_comm=swapper/0 next_pid=0 next_prio=120
(#5)     <idle>-0     [000] d.s3 1384.506577: workqueue_queue_work: work struct=c64cf1f8 function=phy_state_machine workqueue=c60a8100 req_cpu=0 cpu=0
         <idle>-0     [000] d.s3 1384.506580: workqueue_activate_work: work struct c64cf1f8
         <idle>-0     [000] dns5 1384.506585: sched_wakeup: comm=kworker/0:1 pid=1572 prio=120 success=1 target_cpu=000
         <idle>-0     [000] d..3 1384.506602: sched_switch: prev_comm=swapper/0 prev_pid=0 prev_prio=120 prev_state=R ==> next_comm=kworker/0:1 next_pid=1572 next_prio=120
     kworker/0:1-1572 [000] ...1 1384.506606: workqueue_execute_start: work struct c64cf1f8: function phy_state_machine
     kworker/0:1-1572 [000] ...1 1384.506610: workqueue_execute_end: work struct c64cf1f8
     kworker/0:1-1572 [000] d..3 1384.506614: sched_stat_runtime: comm=kworker/0:1 pid=1572 runtime=31333 [ns] vruntime=40988219037 [ns]
     kworker/0:1-1572 [000] d..3 1384.506618: sched_switch: prev_comm=kworker/0:1 prev_pid=1572 prev_prio=120 prev_state=S ==> next_comm=swapper/0 next_pid=0 next_prio=120
(#6)     <idle>-0     [000] ..s2 1384.550637: mmc_request_done: mmc0: end struct mmc_request[c669d954]: cmd_opcode=25 cmd_err=0 cmd_resp=0x900 0xc7a4023c 0x0 0x0 cmd_retries=0 stop_opcode=12 stop_err=0 stop_resp=0x0 0x0 0x0 0x0 stop_retries=0 sbc_opcode=23 sbc_err=0 sbc_resp=0x0 0x0 0x0 0x0 sbc_retries=0 bytes_xfered=4096 data_err=0 can_retune=0 doing_retune=0 retune_now=0 need_retune=0 hold_retune=1 retune_period=0
         <idle>-0     [000] dns5 1384.550644: sched_wakeup: comm=mmcqd/0 pid=89 prio=120 success=1 target_cpu=000
         <idle>-0     [000] d..3 1384.550657: sched_switch: prev_comm=swapper/0 prev_pid=0 prev_prio=120 prev_state=R ==> next_comm=mmcqd/0 next_pid=89 next_prio=120 
```
This required a bit more interpretation – bear with me. We discovered that after mmcqd issues the MMC command (#1), the task goes to sleep indicated by the sched_switch flag showing prev_state=S (#2). The next task to get scheduled is swapper, which is just a confusing name for the idle task, so CPU0 is idle. After 184ms, CPU0 is woken up to flash the framebuffer cursor (fb_flashcursor at step #3), which is executed in the kworker task (#4). Later CPU0 is woken up again briefly for phy_state_machine to service the network driver (#5), and finally CPU0 is woken up to compile the MMC command (#6).

We concluded that CPU0 was only handling two short tasks in the interim that did not take up the majority of the delay latency. Meaning that the CPU was primarily idle, letting us rule out a CPU-bound problem and reaffirming that the MMC controller is the slow path.

We then researched a bit deeper into the eMMC interface to see if there was anything else we could learn. eMMC stands for embedded multimedia card, which is basically an SD card soldered directly onto your embedded target’s motherboard. It’s pretty inexpensive and is a standard for built-in storage on all types of portable electronic devices from cell phones through tablets to digital cameras. eMMC has a built-in controller that mimics a harddisk-like interface (the Flash translation layer or FTL), so the driver software doesn’t need to worry about the internal NAND flash configuration.

The FTL controller-like abstraction also means that the eMMC chip is a black box, so we couldn’t know what was actually going on inside the hardware. Wear-leveling algorithms, garbage collection, read/write contention, faulty cell avoidance, NAND memory aging, or any number of other factors in the hardware implementation could have been causing the sporadic delays. (This IEEE paper talks about how to fix the delays in most FTL implementations and this thesis paper gives you a real sampling of the performance challenges inherent to NAND.) With the complex layer of firmware that turns NAND into a “harddisk”, we felt it was reasonable to expect latency spikes as the underlying hardware rearranges bits based on your requests.

So we definitively confirmed our problem was in the MMC component and it looked like something we couldn’t change. So what could we do about it? One solution was to move the PostgreSQL updates off the main thread. In general, to ensure consistently responsive behavior, it really would have been best to have all blocking I/O activities on a thread separate from the thread that’s handling the user interface (typically the main thread). Of course, that also meant the addition of a lot of thread-safe code to allow data to be transferred between the threads.

In our case, a multi-threaded solution turned out to be a bit of overkill. We were able to achieve much of the same result by disabling the PostgreSQL option for synchronous_commit. That prevented the UX thread from stalling at the point of database updates, which hides the latency spikes from the QtSQL API. Of course, those long latencies are still occurring but because they’re happening in the background and not directly tied to user input, they aren’t noticed by the user. This solution is simple and works well enough for our purpose without overly complicating the code with thread synchronization logic.

Linux Kernel Tracepoints were very handy in identifying our problem and confirming a course of action. While in this post we just used the raw trace output, if you’re not enthusiastic about picking through megabytes of raw event logs, there are a number of open source tools available to help translate these logs into an easier-to-interpret timeline visualization. For example, LTTng provides a way to collect traces, interleave kernel- and user-space traces together, and works with TraceCompass to display them like this:

TraceCompass view of the kernel tracepoints
TraceCompass view of the kernel tracepoints

(We may even cover using this tool in a future blog…)

 

If you’re interested in learning more about tracepoints than we cover in this blog, check out some of these helpful guides:

https://www.kernel.org/doc/Documentation/trace/tracepoint-analysis.txt
http://www.brendangregg.com/perf.html
http://netsplit.com/tracing-on-linux
(And if you’re using an OS other than Linux, other operating systems may provide similar kernel investigation capabilities, like the QNX System Profiler.)